

#ifndef __PARAM_H__
#define __PARAM_H__

#include <string>
#include <map>

namespace CWSLib
{
	class Param
	{
	public:
		// setters
		void setString(const std::string& key, const std::string& value);
		void setValue(const std::string& key, int32_t value);
		void setValue(const std::string& key, int64_t value);
		void setValue(const std::string& key, double value);

		// getters
		// ���ֵ�����ڻ������ʹ��󣬷��ظ����͵�Ĭ��ֵ
		// If value does not exist or type is wrong, return default value of that type
		int32_t getInt(const std::string& key);
		int64_t getLong(const std::string& key);
		double getDouble(const std::string& key);
		std::string getString(const std::string& key);

		// ����key value����bigSep�Ǽ�ֵ��֮��ķָ��� smallSep�Ǽ�ֵ���ڲ��ķָ���
		std::string toQueryString(const std::string& bigSep, const std::string& smallSep);

		// ����key value����bigSep�Ǽ�ֵ��֮��ķָ��� smallSep�Ǽ�ֵ���ڲ��ķָ���
		void parseQueryString(const std::string& rawString, const std::string& bigSep,
			const std::string& smallSep);

		// ������м�ֵ�Բ��ͷ��ڴ�
		void clear();

	private:

	private:
		std::map<std::string, std::string> mKvMap;
	};
}

#endif // !__PARAM_H__

