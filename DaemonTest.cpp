#include "Daemon.h"
#include <string.h>
#include "../common/string-util.h"
#include "../common/SysLog.h"
#include "../event/EventQueue.h"
#include "GameNetHandler.h"
#include "GameEventHandler.h"
#include "GameDataHandler.h"
#include "../logic/GameConfigMgr.h"
#include "../logic/GameServerConfig.h"
#include "GameCacheModule.h"
#include "../common/coredump.h"
#include "../logic/GuildModule.h"
#include "../logic/PlayerNameModule.h"
#include "../logic/CampModule.h"
#include "../logic/CampBattleModule.h"
#include "../logic/GoldIslandModule.h"
#include "../logic/ArenaModule.h"
#include "../logic/GuardModule.h"
#include "../logic/StageModule.h"
#include "../logic/AdvStageModule.h"
#include "../logic/RobberyModule.h"
#include "../logic/RichestModule.h"
#include "../logic/GhostShipModule.h"
#include "../logic/QuizModule.h"
#include "../logic/GlobalModule.h"
#include "../logic/PlayerBaseInfoModule.h"
#include "../logic/LimitShipModule.h"
#include "../logic/ChatModule.h"
#include "../logic/OnearmedBanditModule.h"
#include "../logic/LimitCaptainModule.h"
#include "../logic/GoldIslandModule.h"
#include "../logic/GuildBattleModule.h"
#include "../logic/ArmadaModule.h"
#include "../logic/ValentineModule.h"
#include "../logic/GoldActiveModule.h"
#include "../logic/TuanGouModule.h"
#include "../logic/RankingModule.h"
#include "../logic/GlobalGuildBattleGameModule.h"
#include "../logic/DayDayModule.h"
#include "../logic/ResourceGameModule.h"
#include "../logic/GuildBossModule.h"
#include "../logic/OceanExplorationModule.h"
#include "../logic/WorldWarModule.h"
#include "../logic/MillionaireModule.h"
#include "../logic/AlloyTowerModule.h"
#include "../logic/YouMeModule.h"
#include "../logic/GuildDinnerModule.h"
#include "../logic/GuildRollerModule.h"
#include "../logic/GuildActivityModule.h"
#include "../logic/RushHourModule.h"
#include "../logic/RedisDataServer.h"
#include "../logic/RechargeLoginModule.h"
#include "../logic/FleetRaidModule.h"
#include "GameRedisEventHandler.h"
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "../logic/SecondWorldShipModule.h"
#include "../logic/FleetsGuessModule.h"
#include "../logic/AirPlaneStageModule.h"
#include "../logic/SuperDoubleModule.h"
#include "event/RemoteCallHandler.h"
#ifdef WIN32
#include "../common/DumpHelper.h"
#endif

#define _TEST

#ifdef _TEST
#include <gtest/gtest.h>
#include "../gamed/shiptest/Environment.h"
int PullInMyLibrary();
static int dummy = PullInMyLibrary();
#endif

#ifndef _WIN32
#include <auth.h>
#endif

extern int g_serverId;

bool g_gameerrorexit = false;
Daemon Daemon::m_Daemon;

Daemon::Daemon()
: nid_(0)
{

}

Daemon::~Daemon()
{
    quit();
}

void
Daemon::Init(int nId)
{
    string logger_cfg = "logger.cfg";
    nid_ = nId;
    g_serverId = nId;
    log4cxx::PropertyConfigurator::configure(logger_cfg);
    //ÊÇ·ñÆô¶¯
    if (ServerConfig::Instance().IsGameLogStart())
    {
        CSysLog::GetInstance()->SetLogInfo(true, nId, ServerConfig::Instance().GetGameLogDir(), ServerConfig::Instance().GetGameLogName(),
                ServerConfig::Instance().GetGameLog2SrvAddr(), ServerConfig::Instance().GetGameLog2SrvPort(), ServerConfig::Instance().IsGameLogStart(),
                ServerConfig::Instance().GetGameLogStatLv(), ServerConfig::Instance().GetGameLogStatModul(), ServerConfig::Instance().GetGameLogStatModulVal());
        CSysLog::GetInstance()->ChgLogFile();
    }
    ServerConfig::Instance().SetSelfIP( ServerConfig::Instance().gamedBindIp( ServerConfig::Instance().newregionId(nId) ) );
    RemoteCallHandler::getInstance();

    eq_.reset(new EventQueue());
    nh_.reset(new GameNetHandler(eq_.get(), nId));
    dh_.reset(new GameDataHandler(nId));
    eh_.reset(new GameEventHandler(eq_.get(), dh_.get(), nh_.get(), nId));
    rd_.reset(new RedisDataServer());

    logger_ = log4cxx::Logger::getLogger("Daemon");
    LOG4CXX_INFO(logger_, "start gamed now !!!!");

    GameCacheInst::instance().SetEventHandler(Daemon::Instance().GetGameEventHandler());

    // 0 is local redis server
    rd_->ConnectRedis(0, GameServerConfig::Instance().RedisAddr().data(), GameServerConfig::Instance().RedisPort(), GameServerConfig::Instance().RedisTimeOut(), GameServerConfig::Instance().RedisPassword().data());

    for (int i = 1; i <= ServerConfig::Instance().redisNum() ; i++ )
    {
        rd_->ConnectRedis(i, ServerConfig::Instance().redisAddr(i).data(), ServerConfig::Instance().redisPort(i), 1000, ServerConfig::Instance().redisPassword(i).data());
    }
    rd_->SetEventDelegate<GameRedisEventHandler>();
}

void
Daemon::start()
{
    if (NULL != rd_)
    {
        rd_->Run();
    }

    eh_.get()->start();
    nh_.get()->start();
}

void
Daemon::quit()
{
    if (NULL != rd_)
    {
        rd_->Stop();
    }
	if (eh_.get() != NULL)
		eh_.get()->quit();
	if (nh_.get() != NULL)
		nh_.get()->quit();
}
#ifdef WIN32
void WindowsQuit(int arg){
    Daemon::Instance().quit();
    Daemon::Instance().ClearConfig();
    printf("Quit with arg %d \n",arg);
}
#endif 
void
Daemon::LoadConfig()
{
    GameConfigMgr::CreateInst()->LoadGameConfigInfo();
}

void
Daemon::ClearConfig()
{
    GameConfigMgr::GetInst()->ClearGameConfigInfo();
    GameConfigMgr::ReleaseInst();
}

void gameerrorexit_null()
{

}

void
gameerrorexit()
{
    if (!GameServerConfig::Instance().ErrorSaveEnable() || g_gameerrorexit)
    {
        return;
    }

    g_gameerrorexit = true;
    if (!Daemon::Instance().GetGameDataHandler()->Inited())
    {
        printf("errorexit data handler not inited\n");
        return;
    }

    printf("errorexit before save all user data\n");
    Daemon::Instance().GetGameDataHandler()->saveAllUserData(true);
    GuildModuleInst::instance().SaveAll();
    //PlayerNameModuleInst::instance().saveAll();
    CampModuleInst::instance().saveAll();
    ArenaModuleInst::instance().saveAll();
    GuardModuleInst::instance().saveAll();
    StageModuleInst::instance().saveAll();
    AdvStageModuleInst::instance().saveAll();
    RobberyModuleInst::instance().SaveAll();
    RichestModuleInst::instance().SaveAll();
    GhostShipModuleInst::instance().SaveAll();
    QuizModuleInst::instance().saveAll();
    GlobalModuleInst::instance().SaveAll();
    PlayerBaseInfoModuleInst::instance().saveAll();
    LimitShipModuleInst::instance().SaveAll();
    CampBattleModuleInst::instance().SaveAll();
    ChatModuleInst::instance().SaveAll();
    OnearmedBanditModuleInst::instance().SaveAll();
    LimitCaptainModuleInst::instance().SaveAll();
    GoldIslandModuleInst::instance().SaveAll();
    GuildBattleModuleInst::instance().SaveAll();
    ArmadaModuleInst::instance().SaveAll();
    ValentineModuleInst::instance().SaveAll();
    GoldActiveModuleInst::instance().SaveAll();
    TuanGouModuleInst::instance().SaveAll();
    RankingModuleInst::instance().SaveAll();
    GlobalGuildBattleGameModuleInst::instance().SaveAll();
    DayDayModuleInst::instance().SaveAll();
    ResourceGameModuleInst::instance().SaveAll();
    GuildBossModuleInst::instance().SaveAll();
    OceanExplorationModuleInst::instance().SaveAll();
    // 	MillionModuleInst::instance().SaveAll();
    WorldWarModuleInst::instance().SaveAll();
    AlloyTowerModuleInst::instance().saveAll();
    YouMeModuleInst::instance().SaveAll();
    GuildDinnerModuleInst::instance().SaveAll();
    GuildRollerModuleInst::instance().SaveAll();
    GuildActivityModuleInst::instance().SaveAll();
    RushHourModuleInst::instance().SaveAll();
    RechargeLoginModuleInst::instance().SaveAll();
    FleetRaidModuleInst::instance().SaveAll();
    SecondWorldShipModuleInst::instance().SaveAll();
    FleetsGuessModuleInst::instance().SaveAll();
    AirPlaneStageModuleInst::instance().saveAll();
	SuperDoubleModuleInst::instance().SaveAll();


    std::map<int, ModuleBase*>& modules = Daemon::Instance().GetGameDataHandler()->GetModules();
    for (std::map<int, ModuleBase*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        if (it->second)
            it->second->SaveAll();
    }

    //Daemon::Instance().GetGameDataHandler()->termThreads();
    printf("errorexit after save all user data\n");
    fflush(stdout);
}

int
main(int argn, char *argv[]) // here is the entrance of daemon
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	pErrorExit = gameerrorexit_null;

	testing::Environment* env = testing::AddGlobalTestEnvironment(new TestEnvironment);
	testing::InitGoogleTest(&argn, argv);
	return RUN_ALL_TESTS();

	  return 0; 
}
