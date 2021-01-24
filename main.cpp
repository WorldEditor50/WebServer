

#include <iostream>

#include "frame/CwsServer.h"

int main(int argc, char** argv)
{
	CwsFrame::Server& server = *CServerSingleton::instance();
	server.Run();
	return 0;
}

