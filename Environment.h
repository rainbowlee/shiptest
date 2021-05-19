#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_
#include <gtest/gtest.h>
#include <cstring>
using namespace std;
class TestEnvironment : public testing::Environment
{
public:
	virtual void SetUp();
	virtual void TearDown();
	static int testtype;
	static int parambyte;
	static int paramcount;
	static unsigned long long robotUid;

};

#endif
