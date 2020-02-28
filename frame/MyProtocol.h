

#ifndef __MY_PROTOCOL_H__
#define __MY_PROTOCOL_H__

#include <string>

#include "commlib/basic/Json.h"
#include "commlib/net/EventDispatcher.h"
#include "commlib/thread_pool/ThreadPool.h"
#include "commlib/thread_pool/BaseJob.h"
#include "commlib/basic/CommonSingleton.h"

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

struct RespData
{
	int32_t errcode;
	std::string errorMsg;
	std::string paramList;

	void toJson(CWSLib::JsonNode& node)
	{
		node.addElement("error_code", errcode);
		node.addElement("error_msg", errorMsg);
		node.addElement("parameters", paramList);
	}
};

class BusinessJob : public CWSLib::BaseJob
{
public:
	BusinessJob(const ProtoData& data);

	virtual void excute() override;

private:

private:
	ProtoData mData;
};

class MyProtocol
{
public:
	void process(CWSLib::CbContext& content);

private:
	ProtoData data;
};

#endif // !__MY_PROTOCOL_H__

