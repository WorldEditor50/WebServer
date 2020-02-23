

#include <iostream>
#include "test/test.h"

int main(int argc, char** argv)
{
	//CWSTest::helloWorld();
	//CWSTest::threadPool();
	//CWSTest::log();
	CWSTest::json();
	return 0;
}

#ifdef __VERSION_FOR_RELEASE__

int main(int argc, char** argv)
{
	//CWSTest::helloWorld();
	//CWSTest::threadPool();
	//CWSTest::log();
	CWSTest::json();
	return 0;
}

#endif // __VERSION_FOR_RELEASE__

