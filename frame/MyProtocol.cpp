

#include <memory>

#include "MyProtocol.h"
#include "BaseTask.h"

void MyProtocol::process(CWSLib::CbContext& content)
{
	CWSLib::Json json;
	json.parse(content.data);
	data.apiName = json["interface"].asString();
	data.paramList = json["parameters"].asString();
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
	std::unique_ptr<BaseTask> task(CTaskFactory::instance().create(mData.apiName));
	task->parseRequest(inparam);
	task->process();
	task->packResponse(outParam);

	RespData resp;
	resp.errcode = outParam.getInt("error_code");
	resp.errorMsg = outParam.getString("error_msg");
	resp.paramList = outParam.toQueryString("&", "=");
}
