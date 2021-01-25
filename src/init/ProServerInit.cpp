

#include "frame/CwsServer.h"

#include "src/proj_comm/ProJobImpl.h"
#include "src/user_service/methods/UserInfoRegist.h"

#include "ProServerInit.h"

int ServerInit()
{
    // init jobs
    auto& jobsFactory = *CwsFrame::CJobFactory::instance();
    jobsFactory.join("pro_job_impl", []() { return new ProJobImpl; }, true);

    // init services and methods
    auto& server = *CServerSingleton::instance();
    CwsFrame::Service* userService = new CwsFrame::Service("UserService");
    userService->AddMethod("user_info_regist", []() { return std::shared_ptr<UserInfoRegist>(new UserInfoRegist); });
    server.AddService("UserService", userService);

    return 0;
}
