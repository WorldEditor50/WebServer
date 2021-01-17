

#include "RegistUserInfo.h"

void RegistUserInfo::parseRequest(CWSLib::Param& input)
{
	///TODO...
}

void RegistUserInfo::process()
{
	///TODO...
}

void RegistUserInfo::packResponse(CWSLib::Param& output)
{
	output.setValue("error_code", 0);
	output.setString("error_msg", "OK");
}

REG_TYPE(RegistUserInfo, regist_user_info);
