

#ifndef __JSON_H__
#define __JSON_H__

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace CWSLib
{
	enum class JNodeType
	{
		NUMERIC,
		BOOL,
		STRING,
		LIST,
		STRUCT,
	};

	class JValue
	{
	public:
		virtual ~JValue() {}

		virtual std::string pack() = 0;
		virtual std::string packFmt() = 0;
		virtual JNodeType type() = 0;

		virtual void setLevel(int level) { mLevel = level; }
		int getLevel() { return mLevel; }

		static std::string getTabStr(int level);

	private:
		int mLevel;
	};

	// for numerical value
	template<typename T>
	class JNumValue : public JValue
	{
	public:
		JNumValue(T num) : mVal(num) {}
		virtual std::string pack()
		{
			return std::to_string(mVal);
		}
		virtual std::string packFmt()
		{
			return pack();
		}
		virtual JNodeType type()
		{
			return JNodeType::NUMERIC;
		}

	private:
		T mVal;
	};

	class JBoolValue : public JValue
	{
	public:
		JBoolValue(bool num) : mVal(num) {}
		virtual std::string pack()
		{
			return mVal ? "true" : "false";
		}
		virtual std::string packFmt()
		{
			return pack();
		}
		virtual JNodeType type()
		{
			return JNodeType::BOOL;
		}

	private:
		bool mVal;
	};

	class JStringValue : public JValue
	{
	public:
		JStringValue(const std::string& val) : mVal(val) {}
		virtual std::string pack()
		{
			return "\"" + mVal + "\"";
		}
		virtual std::string packFmt()
		{
			return pack();
		}
		virtual JNodeType type()
		{
			return JNodeType::STRING;
		}

	private:
		std::string mVal;
	};

	class JListValue;
	class JsonNode;
	template<typename Struct>
	void addNodeToList(Struct& item, JListValue* list);

	class JListValue : public JValue
	{
	public:
		virtual void setLevel(int level);
		virtual std::string pack();
		virtual std::string packFmt();
		virtual JNodeType type()
		{
			return JNodeType::LIST;
		}

		void addElement(int item)       { addNumeric(item); }
		void addElement(long long item) { addNumeric(item); }
		void addElement(double item)    { addNumeric(item); }
		void addElement(bool item);
		void addElement(std::string& item);
		void addElement(JValue* node);

		template<typename Struct>
		void addElement(Struct& item)
		{
			addNodeToList(item, this);
		}

		template<typename ElemType>
		void addElement(std::vector<ElemType>& list)
		{
			JListValue* listVal = new JListValue;
			listVal->setLevel(this->getLevel() + 1);
			for (auto& item : list)
			{
				listVal->addElement(item);
			}
			mList.push_back(std::unique_ptr<JValue>(listVal));
		}

	private:
		template<typename NumericType>
		void addNumeric(NumericType item)
		{
			JNumValue<NumericType>* node = new JNumValue<NumericType>(item);
			node->setLevel(this->getLevel() + 1);
			mList.push_back(std::unique_ptr<JValue>(node));
		}

	private:
		std::vector<std::unique_ptr<JValue>> mList;
	};

	class JsonNode : public JValue
	{
	public:
		void addElement(const std::string& key, int item)           { addNumeric(key, item); }
		void addElement(const std::string& key, long long item)     { addNumeric(key, item); }
		void addElement(const std::string& key, double item)        { addNumeric(key, item); }
		void addElement(const std::string& key, bool item);
		void addElement(const std::string& key, std::string& item);
		void addElement(const std::string& key, JValue* node);

		template<typename Struct>
		void addElement(const std::string& key, Struct& item)
		{
			auto itor = mJsonMap.find(key);
			if (itor != mJsonMap.end() && itor->second)
			{
				mJsonMap.erase(itor);
			}
			JsonNode* node = new JsonNode;
			node->setLevel(this->getLevel() + 1);
			item.toJson(*node);
			mJsonMap.insert(std::make_pair(key, std::unique_ptr<JValue>(node)));
		}

		template<typename ElemType>
		void addElement(const std::string& key, std::vector<ElemType>& list)
		{
			auto itor = mJsonMap.find(key);
			if (itor != mJsonMap.end() && itor->second)
			{
				mJsonMap.erase(itor);
			}
			JListValue* listVal = new JListValue;
			listVal->setLevel(this->getLevel() + 1);
			for (auto& item : list)
			{
				listVal->addElement(item);
			}
			mJsonMap.insert(std::make_pair(key, std::unique_ptr<JValue>(listVal)));
		}

		virtual std::string pack();     // pack parameter of structure
		virtual std::string packFmt();  // pack parameter of structure in format
		virtual void setLevel(int level);
		virtual JNodeType type()
		{
			return JNodeType::STRUCT;
		}

	private:
		template<typename NumericType>
		void addNumeric(const std::string& key, NumericType item)
		{
			auto itor = mJsonMap.find(key);
			if (itor != mJsonMap.end() && itor->second)
			{
				mJsonMap.erase(itor);
			}
			JNumValue<NumericType>* node = new JNumValue<NumericType>(item);
			node->setLevel(this->getLevel() + 1);
			mJsonMap.insert(std::make_pair(key, std::unique_ptr<JValue>(node)));
		}

	private:
		std::map<std::string, std::unique_ptr<JValue>> mJsonMap;
	};

	class Json
	{
	public:
		Json();

		// return -1 if failed, return 0 if ok
		int parse(const std::string& jsonString);
		std::string toString();
		std::string toFmtString();

		template<typename T>
		void operator<<(std::vector<T>& list)
		{
			JListValue* node = new JListValue;
			mRootNode.reset(node);
			for (auto& item : list)
			{
				node->addElement(item);
			}
		}

		template<typename T>
		void operator<<(T& item)
		{
			JsonNode* node = new JsonNode;
			mRootNode.reset(node);
			item.toJson(*node);
		}

		static bool isDouble(const std::string& input);
		static bool isLonglong(const std::string& input);
		static bool isInt(const std::string& input);
		static bool isBool(const std::string& input);

	private:
		int addNumericToNode(const std::string& numStr, const std::string& key,
			std::vector<std::unique_ptr<JValue>>& nodeStack);
		int addNumericToList(const std::string& numStr, std::vector<std::unique_ptr<JValue>>& nodeStack);
		int concludeNode(char input,
			std::vector<char>& symbolStack,
			std::vector<std::string>& keyStack,
			std::vector<std::unique_ptr<JValue>>& nodeStack);

	private:
		std::unique_ptr<JValue> mRootNode;
	};

	template<typename Struct>
	void addNodeToList(Struct& item, JListValue* list)
	{
		JsonNode* node = new JsonNode;
		node->setLevel(list->getLevel() + 1);
		item.toJson(*node);
		list->addElement(dynamic_cast<JValue*>(node));
	}
}

#endif // !__JSON_H__

