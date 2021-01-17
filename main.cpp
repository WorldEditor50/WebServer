

#include <iostream>

#include "test/test.h"
#include "frame/Server.h"

#ifdef CLIENT_MAIN

int main(int argc, char** argv)
{
	//CWSTest::helloWorld();
	//CWSTest::threadPool();
	//CWSTest::log();
	CWSTest::client();
	return 0;
}

#else

int main(int argc, char** argv)
{
	//CWSTest::helloWorld();
	//CWSTest::threadPool();
	//CWSTest::log();
	//CWSTest::json();
	CWSLib::Server server;
	server.run();
	return 0;
}

#endif // CLIENT_MAIN
