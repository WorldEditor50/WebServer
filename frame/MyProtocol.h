

#ifndef __MY_PROTOCOL_H__
#define __MY_PROTOCOL_H__

#include <string>

#include "commlib/basic/Json.h"

/*
Protocol Introduction:
This is a test protocol for my frame.
You can use your own protocol to parse data;
Fromat:Json
Example:
	{"interface":"regist_user_info","parameters":"user_name=Alex&sex=Male"}
*/

struct ProtoData
{
	std::string apiName;
	std::string paramList;

	void toJson(CWSLib::JsonNode& node)
	{
		node.addElement("interface", apiName);
		node.addElement("parameters", paramList);
	}
};

class MyProtocol
{
public:
	void parse(const char* content);

private:
	ProtoData data;
};

#endif // !__MY_PROTOCOL_H__

