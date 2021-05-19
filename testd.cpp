//#include <gtest/gtest.h>
//#include "Environment.h"
//int PullInMyLibrary();
//static int dummy = PullInMyLibrary();
//int main(int argc, char *argv[])
//{
//	if (argc < 4)
//	{
//		printf("usage: gamed type byte count\n");
//	}
//
//	//int nid = -1;
//	//int byte = 1;
//	//int count = 1;
//	//if (argc > 1)
//	//	safe_atoi(argv[1], nid);
//
//	//if (argc > 2)
//	//	safe_atoi(argv[2], byte);
//
//	//if (argc > 3)
//	//	safe_atoi(argv[3], count);
//
//	//srand((unsigned int)time(NULL));
//
//	//TestEnvironment::testtype = nid;
//	//TestEnvironment::parambyte = byte;
//	//TestEnvironment::paramcount = count;
//	//if (argc > 1)
//	//{
//	//	TestEnvironment::robotUid = 1;//safe_atoll2(argv[1], 0);
//	//}
//	
//
//	testing::AddGlobalTestEnvironment(new TestEnvironment);
//	testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//	return 0;
//}