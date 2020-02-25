

#include "MyProtocol.h"
#include "commlib/basic/Param.h"

void MyProtocol::parse(const char* content)
{
	CWSLib::Json json;
	json.parse(content);
	data.apiName = json["interface"].asString();
	data.paramList = json["parameters"].asString();
	CWSLib::Param param;
	param.parseQueryString(data.paramList, "&", "=");
}
