#include <gtest/gtest.h>
//#include "gamed/UserMgr/UItemBagMgr.h"
//#include "gamed/logic/User.h"
//#include "gamed/logic/Player.h"
//#include "gamed/GameDataHandler.h"
//#include "event/DceItemBag.pb.h"
//#include "public/Config/tblCode/ItemTbl.h"
//#include "public/Config/tblCode/GiftDropTbl.h"
//#include "public/common/Clock.h"
class BagTest : public testing::Test
{
protected:
	void SetUp()
	{
		//GameEventHandler(DataHandler * pDH, NetHandler * pNH, int nSrvID, emServerType emType);
	
		//auto dh = new GameDataHandler(1);
		//auto nh = new GameNetHandler(1, Srv_Game);
		//GameEventHandler* eventHander = new GameEventHandler(dh, nh, 1, Srv_Game);
		//dh->Init(eventHander);

		//m_pUser = new User(eventHander);
		//m_pUser->InitDB();

		//auto player = m_pUser->GetPlayer();
		//for (int i = 0; i < Res_Type::BagResMax; ++i)
		//{
		//	player->CostMoney((Res_Type)i, player->GetMoney((Res_Type)i), IAT_Admin);
		//	EXPECT_EQ(0, player->GetMoney((Res_Type)i));
		//}

		//string value1 = m_pUser->GetDbUser().SerializeAsString();

		//auto itembagmgr = player->GetItemMgr();
		//itembagmgr->GetDBItemBag()->Clear();

		const char* const str = "world";

		const char* p = str;
		//EXPECT_FALSE(SkipPrefix("W", &p));
		EXPECT_EQ(str, p);

		p = str;
		//EXPECT_FALSE(SkipPrefix("world!", &p));
		EXPECT_EQ(str, p);
	}

	void TearDown()
	{
		//delete m_pUser;
		//m_pUser = NULL;
	}
	//User* m_pUser;
};


TEST_F(BagTest, ItemTest)
{
	bool result = false;
	EXPECT_EQ(result, false);
}


TEST(BattleTest, NormalTest)
{
	const char* const str = "world";

	const char* p = str;
	//EXPECT_FALSE(SkipPrefix("W", &p));
	EXPECT_EQ(str, p);

	p = str;
	//EXPECT_FALSE(SkipPrefix("world!", &p));
	EXPECT_EQ(str, p);
}