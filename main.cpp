

#include <iostream>

#include "frame/CwsServer.h"
#include "src/user_service/UserServicePB.h"

int main(int argc, char** argv)
{
	CwsFrame::Server server;
	server.AddService("UserService", new UserServicePB);
	server.Run();
	return 0;
}

