

#include "commlib/app/MacroAssemble.h"

#include "UserInfoRegist.h"

UserInfoRegist::UserInfoRegist()
{
    m_request.reset(new cws::user::user_info_regist_rqst);
    m_response.reset(new cws::user::user_info_regist_resp);
}

UserInfoRegist::~UserInfoRegist()
{
}

int32_t UserInfoRegist::Execute()
{
    NORMAL_LOG("User name[%s], password[%s]", m_request->user_name().c_str(), m_request->password().c_str());
    /// TODO...
    
    m_response->mutable_ret_info()->set_err_code(0);
    m_response->mutable_ret_info()->set_err_msg("OK");
    m_response->set_user_id("654321");
    return 0;
}

std::shared_ptr<google::protobuf::Message> UserInfoRegist::GetRequest()
{
    return m_request;
}

std::shared_ptr<google::protobuf::Message> UserInfoRegist::GetResponse()
{
    return m_response;
}

//REG_SRV_METHOD(UserInfoRegist, user_service, user_info_regist);

