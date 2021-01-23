

#include <string>
#include "test.h"
#include "commlib/app/MacroAssemble.h"

int main()
{
	auto instance = CWSLib::CommSingleton<CWSTest::ClientQueue>::instance();
	instance.Exec();
	CWSTest::ExecResult& result = instance.Result();
	NORMAL_LOG("======== TEST RESULT ========");
	NORMAL_LOG(">>>>> SUCC %4d", result.succ_num);
	NORMAL_LOG(">>>>> FAIL %4d", result.fail_num);
	return 0;
}
