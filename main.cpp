

#include <iostream>

#include "frame/CwsServer.h"
#include "src/proj_comm/ProJobImpl.h"

int main(int argc, char** argv)
{
	CwsFrame::Server& server = *CServerSingleton::instance();
	server.Run();
	return 0;
}

