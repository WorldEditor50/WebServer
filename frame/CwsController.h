

#ifndef __CWS_CONTROLLER_H__
#define __CWS_CONTROLLER_H__

#include <google/protobuf/service.h>

namespace CwsFrame
{
	class Controller : public google::protobuf::RpcController
	{
	public:
		void Reset() override;
		bool Failed() const override;
		std::string ErrorText() const override;
		void StartCancel() override;
		void SetFailed(const std::string& reason) override;
		bool IsCanceled() const override;
		void NotifyOnCancel(google::protobuf::Closure* callback) override;
	};
}

#endif // !__CWS_CONTROLLER_H__

