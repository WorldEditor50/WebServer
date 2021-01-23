

#ifndef __PRO_JOB_IMPL_H__
#define __PRO_JOB_IMPL_H__

#include "frame/CwsJobImpl.h"

#include "build/stubs/common_define.pb.h"
#include "build/stubs/user_service.pb.h"

class ProJobImpl : public CwsFrame::JobImpl
{
public:
	ProJobImpl() {}
	~ProJobImpl() {}

	void execute() override;

private:
	void ParseInputContent();
	void CallMethod();
	void Response();

private:
	cws::common::RpcContext m_context;
	std::shared_ptr<CwsFrame::Method> m_method;
};

#endif // !__PRO_JOB_IMPL_H__

