

#include "CwsServer.h"
#include "commlib/app/MacroAssemble.h"
#include "commlib/thread_pool/ThreadPool.h"

namespace CwsFrame
{
    int32_t Server::AddService(const std::string& serviceName, google::protobuf::Service* servicePtr)
    {
        if (m_serviceMap.find(serviceName) != m_serviceMap.end())
        {
            NORMAL_LOG("Adding service[%s]", serviceName.c_str());
            m_serviceMap.insert(std::make_pair(serviceName, std::shared_ptr<google::protobuf::Service>(servicePtr)));
            return 0;
        }
        else
        {
            ERROR_LOG("Init server failed--duplicated service name[%s].", serviceName.c_str());
            return -1;
        }
    }

    void Server::Run()
    {
        dispatcher.init(9091, [](CWSLib::Socket& sock, const std::string& input) {
            ///TODO...
            });
        CWSLib::ThreadPool& thdPool = CWSLib::CommSingleton<CWSLib::ThreadPool>::instance();
        thdPool.init(5, 100, 20);
        NORMAL_LOG("Finish init server");
        while (true)
        {
            if (dispatcher.tick() < 0)
            {
                break;
            }
        }
    }

}
