

#include <iostream>

#include "frame/CwsServer.h"
#include "src/proj_comm/ProJobImpl.h"
#include "src/init/ProServerInit.h"

int main(int argc, char** argv)
{
	if (0 != ServerInit()) return -1;
	CwsFrame::Server& server = *CServerSingleton::instance();
	server.Run();
	return 0;
}

