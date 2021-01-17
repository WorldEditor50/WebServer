

#include <memory>

#include "MyProtocol.h"
#include "BaseTask.h"

void MyProtocol::process(CWSLib::CbContext& content)
{
	CWSLib::Json json;
	if (json.parse(content.data) == -1)
	{
		char retMsg[20] = "wrong msg";
		CWSLib::writeAll(content.socketfd, retMsg, sizeof(retMsg));
		return;
	}

	data.apiName = json["interface"].asString();
	data.paramList = json["parameters"].asString();
	data.sockFd = content.socketfd;
	CWSLib::Param inparam, outParam;
	inparam.parseQueryString(data.paramList, "&", "=");
	
	CWSLib::ThreadPool& thdPool = CWSLib::CommSingleton<CWSLib::ThreadPool>::instance();
	thdPool.addTask(new BusinessJob(data));
}


BusinessJob::BusinessJob(const ProtoData& data)
{
	mData = data;
}

void BusinessJob::excute()
{
	CWSLib::Param inparam, outParam;
	inparam.parseQueryString(mData.paramList, "&", "=");
	DEBUG_LOG("Api name[%s]", mData.apiName.c_str());
	BaseTask* taskPtr = CTaskFactory::instance().create(mData.apiName);
	if (taskPtr == nullptr)
	{
		outParam.setValue("error_code", -1);
		outParam.setString("error_msg", "fail to create task instance");
	}
	else
	{
		std::unique_ptr<BaseTask> task(taskPtr);
		task->parseRequest(inparam);
		task->process();
		task->packResponse(outParam);
	}

	RespData resp;
	resp.errcode = outParam.getInt("error_code");
	resp.errorMsg = outParam.getString("error_msg");
	resp.paramList = outParam.toQueryString("&", "=");
	CWSLib::Json outJson;
	outJson << resp;
	std::string outStrData = outJson.toString();
	CWSLib::writeAll(mData.sockFd, const_cast<char*>(outStrData.c_str()), outStrData.length());
}
