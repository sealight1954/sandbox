/*!
 * @file DevCard.cpp
 * @brief DevelopmentCardの制御
 * @author semitsu
 * @date 12.10.09 開始
 */

#include "DevCard.h"
#include "Act.h"
#include "Map.h"
/* DrawMsg */
#include "Disp.h"
//#include 
#include "Resource.h"
#define DEBUG_MOVE_THIEF 0
static int largest_army_num=LARGEST_ARMY_MIN;
static int largest_army_player=NO_PLAYER;
static int DevCardOpen[NUM_OF_PLAYERS][TYPE_CRD_END]={};/* 初期値は0 */
#if DEBUG_GAME
static int DevCardClose[NUM_OF_PLAYERS][TYPE_CRD_END]={{3,3,3,3,3},{3,3,3,3,3},{3,3,3,3,3},{3,3,3,3,3}};/* 初期値は0 */
#else
static int DevCardClose[NUM_OF_PLAYERS][TYPE_CRD_END]={};/* 初期値は0 */
#endif
static int DevCardMaster[TYPE_CRD_END]={NUM_OF_CRD_MONOPOLY, NUM_OF_CRD_HARVEST, NUM_OF_CRD_BUILDING_ROAD, NUM_OF_CRD_SOLDIER, NUM_OF_CRD_POINT};
static int prev_DevCardOpen[NUM_OF_PLAYERS][TYPE_CRD_END]={};/* 初期値は0 */
static int prev_DevCardClose[NUM_OF_PLAYERS][TYPE_CRD_END]={};/* 初期値は0 */
static int prev_DevCardMaster[TYPE_CRD_END]={NUM_OF_CRD_MONOPOLY, NUM_OF_CRD_HARVEST, NUM_OF_CRD_BUILDING_ROAD, NUM_OF_CRD_SOLDIER, NUM_OF_CRD_POINT};
//static void Crd_CostDevCard(int player,int type);
static bool Crd_GetDevCard_(int player,int type);
//static bool Crd_GetDevCard_(int player,TypeDevCard type);
void Crd_SaveState(void)
{
	memcpy(prev_DevCardClose,DevCardClose,sizeof(int)*NUM_OF_PLAYERS*TYPE_CRD_END);
	memcpy(prev_DevCardOpen,DevCardOpen,sizeof(int)*NUM_OF_PLAYERS*TYPE_CRD_END);
	memcpy(prev_DevCardMaster,DevCardMaster,sizeof(int)*TYPE_CRD_END);
}
void Crd_LoadState(void)
{
	memcpy(DevCardClose, prev_DevCardClose, sizeof(int)*NUM_OF_PLAYERS*TYPE_CRD_END);
	memcpy(DevCardOpen,  prev_DevCardOpen,  sizeof(int)*NUM_OF_PLAYERS*TYPE_CRD_END);
	memcpy(DevCardMaster,prev_DevCardMaster,sizeof(int)*TYPE_CRD_END);
}
int Crd_GetSumOfCard(void)
{
	int sum=0;
	for(int i=0;i<TYPE_CRD_END;i++)
	{
		sum+=DevCardMaster[i];
	}
	return sum;
}
int Crd_GetLargestArmyPlayer()
{
	return largest_army_player;
}
bool Crd_GetDevCard(int player)
{
	//int i=3;/* randomに */
	int i=Act_RandCategory(DevCardMaster,TYPE_CRD_END);
	bool doGetCard=false;
	if(Crd_GetDevCard_(player,i))
	{
		doGetCard=true;
	}
	return doGetCard;
}
static bool Crd_GetDevCard_(int player,int type)
{
	bool doGetCard=true;
	if(Rsc_IsErrorExchangeDevCard(player))
	{
		doGetCard=false;
	}

	if(doGetCard)Rsc_ExchangeDevCard(player);
	/* DevCardCloseを交換する処理 */
	DevCardClose[player][type]++;
	DevCardMaster[type]--;
	return doGetCard;
}
bool Crd_Monopoly(int player, int rscType)
{
	bool doMonopoly=true;
	/* エラー条件なし */
	if(doMonopoly)
	{
		//Crd_CostDevCard(player,TYPE_CRD_MONOPOLY);
		//DevCardClose[player][TYPE_CRD_MONOPOLY]--;
		//DevCardClose[player][TYPE_CRD_MONOPOLY]=DevCardOpen[player][TYPE_CRD_MONOPOLY]-1;
		//DevCardOpen[player][TYPE_CRD_MONOPOLY]++;
		Rsc_Monopoly(player,rscType);
	}
	return doMonopoly;
}
bool Crd_IsErrorNoCard(int player,int crdType)
{
	return DevCardClose[player][crdType]==0;
}
bool Crd_IsErrorDoSoldier(int player, int ch_idx, int enemy_player)
{
	bool isError=false;
	if(Map_IsErrorMoveThief(player,ch_idx,enemy_player))isError=true;
	//if(DevCardClose[player][TYPE_CRD_SOLDIER]==0)isError=true;
	return isError;
}
bool Crd_Soldier(int player, int ch_idx, int enemy_player)
{
	bool doSoldier=true;
//bool Map_IsErrorMoveThief(int ch_idx, int enemy_player)
	bool f_debug_move_thief;
#if DEBUG_MOVE_THIEF == 1
	f_debug_move_thief = Map_IsErrorMoveThief(1,2);
	f_debug_move_thief = Map_IsErrorMoveThief(DESERT_CH,2);
	Map_TestCase1ForCrd_Soldier();
	f_debug_move_thief=Map_IsErrorMoveThief(1,0);
#endif
	//if(Crd_IsErrorDoSoldier(player,ch_idx,enemy_player))
	//{
	//	doSoldier=false;
	//}
	if(doSoldier)
	{
		//Crd_CostDevCard(player,TYPE_CRD_SOLDIER);
		/* Largest_Army 更新 */
		if(DevCardOpen[player][TYPE_CRD_SOLDIER]>largest_army_num)
		{
			//if(largest_army_player!=NO_PLAYER)
			Act_SubPoint(largest_army_player,2);
			largest_army_player=player;
			Act_AddPoint(largest_army_player,2);
			Dsp_DrawMsg("LargestArmy Update",0);
		}
		Map_MoveThief(player,ch_idx,enemy_player);
		//Rsc_MoveRsc/* 資源を1つ奪う */
	}
	return doSoldier;
}
//bool Crd_DevRoad(int player, int cs_idx)
//{
//	bool doBuild=true;
//	if(doBuild){
//		Rsc_BuildRoad(player);
//		Map_BuildRoad(player,cs_idx);
//		DevelopmentResource[player][TYPE_DEV_ROAD]--;
//		Map_DrawRoad(cs_idx);
//	}
//}
/* 使ってない */
bool Crd_IsErrorDoHarvest(int player, int ch_idx, int enemy_player)
{
	bool isError=false;
	if(Map_IsErrorMoveThief(player,ch_idx,enemy_player))isError=true;
	//if(DevCardClose[player][TYPE_CRD_HARVEST]==0)isError=true;
	return isError;
}
bool Crd_Harvest(int player, int typeRsc1,int typeRsc2)
{
	bool doHarvest=true;
	if(Crd_IsErrorDoHarvest(player,typeRsc1,typeRsc2))doHarvest=false;
	
	if(doHarvest)
	{
	Rsc_DoHarvest(player,typeRsc1,typeRsc2);
	//Crd_CostDevCard(player,TYPE_CRD_HARVEST);
	}
	return doHarvest;
}
bool Crd_Point(int player)
{
	//Crd_CostDevCard(player,TYPE_CRD_POINT);
	Act_AddPoint(player);
	return true;
}
 void Crd_CostDevCard(int player,int type)
{
		DevCardClose[player][type]--;
		DevCardOpen[player][type]++;
}

void Crd_GetInfo_DevCardOpen(int player,string *val)
{
	stringstream stream;
	for(int i=0;i<TYPE_CRD_END;i++)
	{
		stream.str("");
		stream<<DevCardOpen[player][i];
		val[i]=player!=NO_PLAYER ? stream.str() : "-1";
	}
}
void Crd_GetInfo_DevCardClose(int player,string *val)
{
	stringstream stream;
	for(int i=0;i<TYPE_CRD_END;i++)
	{
		stream.str("");
		stream<<DevCardClose[player][i];
		val[i]=player!=NO_PLAYER ? stream.str() : "-1";
	}
}