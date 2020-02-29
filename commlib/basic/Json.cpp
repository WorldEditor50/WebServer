

#include "Json.h"

#include <sstream>
#include <iostream>

CWSLib::Json::Json()
{
	mRootNode.reset(new JsonNode);
	mRootNode->setLevel(0);
}

int CWSLib::Json::parse(const std::string& jsonString)
{
	std::vector<char> symbolStack;
	std::string nameStack;
	std::vector<std::string> keyStack;
	std::vector<std::unique_ptr<JValue>> nodeStack;

	size_t length = jsonString.size();
	for (size_t i = 0; i < length; ++i)
	{
		char ch = jsonString.at(i);
		//std::cout << "[" << i << "]" << ch << "\n";
		switch (ch)
		{
		case '{':
			if (!symbolStack.empty() && symbolStack.back() == '\"')
			{
				nameStack.push_back(ch);
			}
			else
			{
				symbolStack.push_back(ch);
				nodeStack.push_back(std::unique_ptr<JValue>(new JsonNode));
			}
			break;

		case '[':
			if (!symbolStack.empty() && symbolStack.back() == '\"')
			{
				nameStack.push_back(ch);
			}
			else
			{
				symbolStack.push_back(ch);
				nodeStack.push_back(std::unique_ptr<JValue>(new JListValue));
			}
			break;

		case '}':
		case ']':
			if (concludeNode(ch, symbolStack, keyStack, nodeStack) != 0)
			{
				return -1;
			}
			break;

		case ':':
			// it's a seperator, so check previus stacks
			if (!symbolStack.empty() && symbolStack.back() == '\"')
			{
				return -1;
			}
			symbolStack.push_back(ch);
			break;

		case ',':
			if (symbolStack.empty())
			{
				return -1;
			}
			if (symbolStack.back() == '\"')
			{
				// just a part of a key or value
				nameStack.push_back(',');
			}
			else if(!nameStack.empty() && !nodeStack.empty())
			{
				if (!keyStack.empty())
				{
					if (addNumericToNode(nameStack, keyStack.back(), nodeStack) != 0)
					{
						return -1;
					}
					keyStack.pop_back();
					if (symbolStack.back() == ':')
					{
						symbolStack.pop_back();
					}
				}
				else
				{
					if (addNumericToList(nameStack, nodeStack) != 0)
					{
						return -1;
					}
				}
				nameStack.clear();
			}
			break;

		case '\\':
			// escape charactor
			if (i + 1 == length || jsonString.at(i + 1) != '\"')
			{
				return -1;
			}
			nameStack.push_back('\\');
			nameStack.push_back('\"');
			i++;
			break;

		case '\"':
			if (symbolStack.empty())
			{
				return -1;
			}
			if (symbolStack.back() == '\"')
			{
				symbolStack.pop_back();	// pop last '"'
				if (!symbolStack.empty() && symbolStack.back() == ':')
				{
					symbolStack.pop_back();	// pop ':'
					if (keyStack.empty() || nodeStack.empty())
					{
						return -1;
					}
					JValue* currNode = nodeStack.back().get();
					dynamic_cast<JsonNode*>(currNode)->addElement(keyStack.back(), nameStack);
					keyStack.pop_back();
					nameStack.clear();
				}
				else if (i + 1 < length && jsonString.at(i + 1) == ':')
				{
					keyStack.push_back(nameStack);
					nameStack.clear();
				}
				else if (i + 1 < length &&
					(jsonString.at(i + 1) == ']' || jsonString.at(i + 1) == ','))
				{
					JValue* currNode = nodeStack.back().get();
					dynamic_cast<JListValue*>(currNode)->addElement(nameStack);
					nameStack.clear();
				}
			}
			else
			{
				symbolStack.push_back('\"');
			}
			break;

		default:
			nameStack.push_back(ch);
			if (((ch >= '0' && ch <= '9') || ch == 'e' ) && i + 1 < length)
			{
				if (jsonString.at(i + 1) == ']')
				{
					if (addNumericToList(nameStack, nodeStack) != 0)
					{
						return -1;
					}
					nameStack.clear();
				}
				else if (jsonString.at(i + 1) == '}')
				{
					if (!keyStack.empty() && !symbolStack.empty() && symbolStack.back() == ':')
					{
						if (addNumericToNode(nameStack, keyStack.back(), nodeStack) != 0)
						{
							return -1;
						}
						nameStack.clear();
						keyStack.pop_back();
						symbolStack.pop_back();
					}
					else
					{
						return -1;
					}
				}
			}
			break;
		}
	}

	if (nodeStack.empty())
	{
		return -1;
	}
	mRootNode.reset(nodeStack.front().release());
	mRootNode->setLevel(0);
	return 0;
}

std::string CWSLib::Json::toString()
{
	return mRootNode->pack();
}

std::string CWSLib::Json::toFmtString()
{
	return mRootNode->packFmt();
}

bool CWSLib::Json::isDouble(const std::string& input)
{
	bool hasPoint = false;
	for (auto item : input)
	{
		if (item == '.')
		{
			hasPoint = true;
		}
		else if (item > '9' || item < '0')
		{
			return false;
		}
	}
	return hasPoint;
}

bool CWSLib::Json::isLonglong(const std::string& input)
{
	if (input.size() < 10)
	{
		return false;
	}
	for (auto item : input)
	{
		if (item > '9' || item < '0')
		{
			return false;
		}
	}
	return true;
}

bool CWSLib::Json::isInt(const std::string& input)
{
	if (input.size() >= 10)
	{
		return false;
	}
	for (auto item : input)
	{
		if (item > '9' || item < '0')
		{
			return false;
		}
	}
	return true;
}

bool CWSLib::Json::isBool(const std::string& input)
{
	return (input == "true") || (input == "false");
}

int CWSLib::Json::addNumericToNode(const std::string& numStr, const std::string& key,
	std::vector<std::unique_ptr<JValue>>& nodeStack)
{
	JsonNode* node = dynamic_cast<JsonNode*>(nodeStack.back().get());
	if (isDouble(numStr))
	{
		node->addElement(key, std::stod(numStr));
	}
	else if (isLonglong(numStr))
	{
		node->addElement(key, std::stoll(numStr));
	}
	else if (isInt(numStr))
	{
		node->addElement(key, std::stoi(numStr));
	}
	else if (isBool(numStr))
	{
		node->addElement(key, numStr == "true");
	}
	else
	{
		return -1;
	}
	return 0;
}

int CWSLib::Json::addNumericToList(
	const std::string& numStr,
	std::vector<std::unique_ptr<JValue>>& nodeStack)
{
	JListValue* node = dynamic_cast<JListValue*>(nodeStack.back().get());
	if (isDouble(numStr))
	{
		node->addElement(std::stod(numStr));
	}
	else if (isLonglong(numStr))
	{
		node->addElement(std::stoll(numStr));
	}
	else if (isInt(numStr))
	{
		node->addElement(std::stoi(numStr));
	}
	else if (isBool(numStr))
	{
		node->addElement(numStr == "true");
	}
	else
	{
		return -1;
	}
	return 0;
}

int CWSLib::Json::concludeNode(char input,
	std::vector<char>& symbolStack,
	std::vector<std::string>& keyStack,
	std::vector<std::unique_ptr<JValue>>& nodeStack)
{
	char checkChar = 0;
	if (input == '}') checkChar = '{';
	else if (input == ']') checkChar = '[';
	else return -1;

	if (symbolStack.empty() || symbolStack.back() != checkChar)
	{
		return -1;
	}
	symbolStack.pop_back();
	if (nodeStack.empty())
	{
		return -1;
	}
	size_t nodeSize = nodeStack.size();
	if (nodeSize >= 2)
	{
		JValue* papaNode = nodeStack[nodeSize - 2].get();
		JValue* lastNode = nodeStack[nodeSize - 1].release();
		if (papaNode->type() == JNodeType::LIST)
		{
			dynamic_cast<JListValue*>(papaNode)->addElement(lastNode);
		}
		else if (papaNode->type() == JNodeType::STRUCT && !keyStack.empty())
		{
			dynamic_cast<JsonNode*>(papaNode)->addElement(keyStack.back(), lastNode);
			if (!symbolStack.empty() || symbolStack.back() == ':')
			{
				symbolStack.pop_back();
			}
		}
		else
		{
			return -1;
		}
		nodeStack.pop_back();
	}
	return 0;
}




/////////**************** JSON NODE *****************//////////

void CWSLib::JsonNode::addElement(const std::string& key, bool item)
{
	auto itor = mJsonMap.find(key);
	if (itor != mJsonMap.end() && itor->second)
	{
		mJsonMap.erase(itor);
	}
	JBoolValue* node = new JBoolValue(item);
	node->setLevel(this->getLevel() + 1);
	mJsonMap.insert(std::make_pair(key, std::unique_ptr<JValue>(node)));
}

void CWSLib::JsonNode::addElement(const std::string& key, std::string& item)
{
	auto itor = mJsonMap.find(key);
	if (itor != mJsonMap.end() && itor->second)
	{
		mJsonMap.erase(itor);
	}
	JStringValue* node = new JStringValue(item);
	node->setLevel(this->getLevel() + 1);
	mJsonMap.insert(std::make_pair(key, std::unique_ptr<JValue>(node)));
}

void CWSLib::JsonNode::addElement(const std::string& key, JValue* node)
{
	mJsonMap.insert(std::make_pair(key, std::unique_ptr<JValue>(node)));
}

std::string CWSLib::JsonNode::pack()
{
	if (mJsonMap.empty())
	{
		return "{}";
	}

	std::stringstream sspack;
	sspack << "{";
	for (auto& item : mJsonMap)
	{
		sspack
			<< "\"" << item.first << "\":"
			<< item.second->pack() << ",";
	}
	std::string strpack = sspack.str();
	size_t length = strpack.size();
	return strpack.substr(0, length - 1) + "}";
}

std::string CWSLib::JsonNode::packFmt()
{
	if (mJsonMap.empty())
	{
		return "{}";
	}

	std::stringstream sspack;
	sspack << "{";
	for (auto& item : mJsonMap)
	{
		sspack
			<< "\n" << JValue::getTabStr(item.second->getLevel()) << "\"" << item.first << "\":"
			<< item.second->packFmt() << ",";
	}
	std::string strpack = sspack.str();
	size_t length = strpack.size();
	std::stringstream outpack;
	outpack << strpack.substr(0, length - 1) << "\n" << JValue::getTabStr(this->getLevel()) << "}";
	return outpack.str();
}

void CWSLib::JsonNode::setLevel(int level)
{
	JValue::setLevel(level);
	for (auto& item : mJsonMap)
	{
		item.second->setLevel(level + 1);
	}
}

CWSLib::JValue* CWSLib::JsonNode::getElement(const std::string& key)
{
	auto itor = mJsonMap.find(key);
	if (itor == mJsonMap.end())
	{
		return nullptr;
	}
	return itor->second.get();
}

void CWSLib::JListValue::setLevel(int level)
{
	JValue::setLevel(level);
	for (auto& item : mList)
	{
		item->setLevel(level + 1);
	}
}

std::string CWSLib::JListValue::pack()
{
	if (mList.empty())
	{
		return "[]";
	}

	std::stringstream sspack;
	sspack << "[";
	for (auto& item : mList)
	{
		sspack << item->pack() << ",";
	}
	std::string strpack = sspack.str();
	size_t length = strpack.size();
	return strpack.substr(0, length - 1) + "]";
}

std::string CWSLib::JListValue::packFmt()
{
	if (mList.empty())
	{
		return "[]";
	}

	std::stringstream sspack;
	sspack << "[";
	for (auto& item : mList)
	{
		sspack << "\n" << JValue::getTabStr(item->getLevel()) << item->packFmt() << ",";
	}
	std::string strpack = sspack.str();
	size_t length = strpack.size();
	std::stringstream outpack;
	outpack << strpack.substr(0, length - 1) << "\n" << JValue::getTabStr(this->getLevel()) << "]";
	return outpack.str();
}

void CWSLib::JListValue::addElement(bool item)
{
	JBoolValue* node = new JBoolValue(item);
	node->setLevel(this->getLevel() + 1);
	mList.push_back(std::unique_ptr<JValue>(node));
}

void CWSLib::JListValue::addElement(std::string& item)
{
	JStringValue* node = new JStringValue(item);
	node->setLevel(this->getLevel() + 1);
	mList.push_back(std::unique_ptr<JValue>(node));
}

void CWSLib::JListValue::addElement(JValue* node)
{
	mList.push_back(std::unique_ptr<JValue>(node));
}

std::string CWSLib::JValue::getTabStr(int level)
{
	std::string outString;
	for (int i = 0; i < level; ++i)
	{
		outString.append("    ");
	}
	return std::move(outString);
}
