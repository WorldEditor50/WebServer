

#ifndef __USER_SERVICE_H__
#define __USER_SERVICE_H__

#include <build/stubs/user_service.pb.h>

class UserService : public cws::user::UserService
{
    void user_info_regist(google::protobuf::RpcController* controller,
        const ::cws::user::user_info_regist_rqst* request,
        ::cws::user::user_info_regist_resp* response,
        ::google::protobuf::Closure* done) override;
    void user_info_login(google::protobuf::RpcController* controller,
        const ::cws::user::user_info_login_rqst* request,
        ::cws::user::user_info_login_resp* response,
        ::google::protobuf::Closure* done) override;
};

#endif // !__USER_SERVICE_H__

