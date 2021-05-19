#include <iostream>
#include "Environment.h"
#include "../logic/GameConfigMgr.h"
#include "../gamed/Daemon.h"
void TestEnvironment::SetUp()
{
	Daemon::Instance().LoadConfig();

	Daemon::Instance().Init(1);
	//ServerConfig::Instance().LoadConfig();
	//WorldConfigMgr::GetInst()->LoadWorldConfigInfo();
	//FLAGS_minloglevel = google::GLOG_ERROR;
}

void TestEnvironment::TearDown()
{

}

int TestEnvironment::testtype = 0; //0 ∆’Õ®≤‚ ‘  1 ≤‚ ‘dbsrv
int TestEnvironment::parambyte = 100000; 
int TestEnvironment::paramcount = 100000;
unsigned long long TestEnvironment::robotUid = 300001ll;
