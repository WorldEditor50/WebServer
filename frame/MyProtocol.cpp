
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
	std::unique_ptr<BaseTask> task(CTaskFactory::instance().create(data.apiName));
	task->parseRequest(inparam);
	task->process();
	task->packResponse(outParam);
	
	resp.errcode = outParam.getInt("error_code");
	resp.errorMsg = outParam.getString("error_msg");
	resp.paramList = outParam.toQueryString("&", "=");
}
