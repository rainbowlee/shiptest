#include <gtest/gtest.h>
#include <iostream>
#include <gtest/gtest.h>
#include <vector>

#include "../logic/User.h"
#include "../logic/Player.h"
#include "../logic/BattleMgr.h"
#include "../logic/WishGiftMgr.h"
#include "../logic/StageMgr.h"
#include "../logic/dbinterface.pb.h"
#include "../logic/StageTbl.h"
#include "../common/const_def.h"
#include "../common/type_def.h"

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


TEST(BattleTest2, NormalTest)
{
}

TEST(BattleTest, NormalTest)
{
	
	//for( int i = 0; i < 100; i++)
	{
		User* pUser = new User;
		pUser->Init();
		pUser->InitNewUser();
		Player* pPlayer = pUser->GetPlayer();
		pPlayer->SetLevel(1000);

		WishGiftMgr* mgr = pPlayer->GetWishGiftMgr();
		mgr->SetNewDay();
		//mgr->TriggerTask();
		const char* const str = "world";

		const char* p = str;
		//EXPECT_FALSE(SkipPrefix("W", &p));
		EXPECT_EQ(str, p);

		p = str;
		//EXPECT_FALSE(SkipPrefix("world!", &p));
		EXPECT_EQ(str, p);
		pPlayer->AddCash(5000000,GM_AddMoney_Add);

		pPlayer->AddToken(TOKEN_TYPE_WishGift, 500001);

		mgr->SetWishGiftId(2);
		DceWishGift dceproto;
		dceproto.add_boxindex(0);
		dceproto.add_boxindex(1);
		dceproto.add_boxindex(2);
		dceproto.add_boxindex(3);
		dceproto.add_boxindex(4);
		dceproto.set_id(2);

		DseWishGift dseproto;

		//58	0	ÐÄÔ¸±Ò	1	|1|2|3|4|5	|10|25|30|25|10
		//2135	180	ÈÙÓþ¼ÇÂ¼	2	|4|8|12|16|20	|10|25|30|25|10

		map<string,int> keyCount;
		for(int i = 0; i < 10000; i++)
		{
			dceproto.clear_boxindex();
			dceproto.add_boxindex(i%10);
			dseproto.mutable_droplist()->clear_droplist();
			int ret = mgr->OpenBox(&dceproto,&dseproto);

			if(ret != 0)
			{
				int test = 0;
				test++;
			}

			if( dseproto.mutable_droplist()->droplist_size() == 0)
			{
				int test1 = 0;
				test1++;
			}


			int id = dseproto.mutable_droplist()->droplist(0).id();
			int count = dseproto.mutable_droplist()->droplist(0).count();


			string key = toString(id) + "_" + toString(count);

			auto iter = keyCount.find(key);
			if(iter == keyCount.end())
			{
				keyCount.insert(make_pair(key,0));
				iter = keyCount.find(key);
			}

			iter->second++;
		}


		for(int r = 0; r < 100; r++)
		{
			dseproto.Clear();
			mgr->ShopRefresh(&dseproto);
			
			int buycount = 0;
			for(int buyi = 0; buyi < dseproto.shoplist_size(); buyi++)
			{
				if(buyi%2==1)
				{
					int id = dseproto.shoplist(buyi).id();
					DseWishGift dseprotoret;
					mgr->ShopBuy(id,&dseprotoret);
				}
			}

		}
		
		dseproto.Clear();


		mgr->ShopRefresh(&dseproto);
	}
}