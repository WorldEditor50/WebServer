

#include <iostream>

#include "test/test.h"
#include "frame/Server.h"

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

#ifdef CLIENT_MAIN

int main(int argc, char** argv)
{
	//CWSTest::helloWorld();
	//CWSTest::threadPool();
	//CWSTest::log();
	CWSTest::json();
	return 0;
}

#endif // CLIENT_MAIN
