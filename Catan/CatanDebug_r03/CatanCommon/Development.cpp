/*!
* @file Development.cpp
* @brief 建築資源と建設アクトの制御
* @author semitsu
* @date 12.10.02 開始
*/


#include "Development.h"
#include "Map.h"
#include "Disp.h"
#include "Act.h" /* ポイント追加 */
#define DEBUG_BUILD_HOUSE 0
#define DEBUG_BUILD_ROAD 0
/* --建築資源の宣言-------------------------------------------------------- */
static int DevelopmentResource[NUM_OF_PLAYERS][TYPE_DEV_END]=
{
	{NUM_OF_EACH_HOUSE,NUM_OF_EACH_ROAD,NUM_OF_EACH_FACTORY},
	{NUM_OF_EACH_HOUSE,NUM_OF_EACH_ROAD,NUM_OF_EACH_FACTORY},
	{NUM_OF_EACH_HOUSE,NUM_OF_EACH_ROAD,NUM_OF_EACH_FACTORY},
	{NUM_OF_EACH_HOUSE,NUM_OF_EACH_ROAD,NUM_OF_EACH_FACTORY}};

static int prev_DevelopmentResource[NUM_OF_PLAYERS][TYPE_DEV_END];
static int prev_FirstHouse[NUM_OF_PLAYERS];
static int prev_SecondHouse[NUM_OF_PLAYERS];
/* --関数宣言-------------------------------------------------------------- */
 bool Dev_IsErrorBuildHouse(int player, int cp_idx);

void Dev_SaveState(void)
{
	memcpy(prev_DevelopmentResource,DevelopmentResource,sizeof(int)*NUM_OF_PLAYERS*TYPE_DEV_END);
	//memcpy(prev_FirstHouse,FirstHouse,sizeof(int)*NUM_OF_PLAYERS);
	//memcpy(prev_SecondHouse,SecondHouse,sizeof(int)*NUM_OF_PLAYERS);
}
void Dev_LoadState(void)
{
	memcpy(DevelopmentResource,prev_DevelopmentResource,sizeof(int)*NUM_OF_PLAYERS*TYPE_DEV_END);
	//memcpy(FirstHouse,prev_FirstHouse,sizeof(int)*NUM_OF_PLAYERS);
	//memcpy(SecondHouse,prev_SecondHouse,sizeof(int)*NUM_OF_PLAYERS);
}
/*!
* @brief 家を建てる
* @param [player] 0-3 プレイヤ 
* @param [cp_idx] CPのID
* @retval 家が建てられたかどうか
*/
bool Dev_BuildHouse(int player, int cp_idx)
{
	bool doBuild=true;
#if DEBUG_BUILD_HOUSE ==1
	Map_TestCase1ForDev_BuildHouse();
	cout<<Dev_IsErrorBuildHouse(0,9)<<endl;//1
	cout<<Dev_IsErrorBuildHouse(0,8)<<endl;//1
	cout<<Dev_IsErrorBuildHouse(0,3)<<endl;//0
	cout<<Dev_IsErrorBuildHouse(1,8)<<endl;//1
	cout<<Dev_IsErrorBuildHouse(1,14)<<endl;//0
	/* ok */
#elif DEBUG_BUILD_HOUSE == 2
	Map_TestCase1ForDev_BuildHouse();
	Rsc_TestCase1ForBuildHouse();
	cout<<Dev_IsErrorBuildHouse(0,3)<<endl;//0
	Rsc_TestCase2ForBuildHouse();
	cout<<Dev_IsErrorBuildHouse(0,3)<<endl;//0
	Rsc_TestCase3ForBuildHouse();
	cout<<Dev_IsErrorBuildHouse(0,3)<<endl;//0
	Rsc_TestCase4ForBuildHouse();
	cout<<Dev_IsErrorBuildHouse(0,3)<<endl;//0
	Rsc_TestCase5ForBuildHouse();
	cout<<Dev_IsErrorBuildHouse(0,3)<<endl;//0
#endif
	/*if(Dev_IsErrorBuildHouse(player,cp_idx)){
	return false;
	}*/
	if(Map_IsErrorBuildHouse(player,cp_idx))doBuild= false;
	//if(Rsc_IsErrorBuildHouse(player))doBuild= false;
	if(DevelopmentResource[player][TYPE_DEV_HOUSE]==0)doBuild= false;

	if(doBuild)
	{
		Rsc_BuildHouse(player);
		Map_BuildHouse(player,cp_idx);
		DevelopmentResource[player][TYPE_DEV_HOUSE]--;
		Act_AddPoint(player);
		Dsp_DrawHouseFactory(cp_idx,player,TYPE_DEV_HOUSE);
	}
	return doBuild;
}
bool Dev_BuildHouseFirst(int player, int cp_idx,int numTurn)
{
	Map_BuildHouse(player,cp_idx);
	DevelopmentResource[player][TYPE_DEV_HOUSE]--;
	Act_AddPoint(player);
	/* 最初に立てた家の情報をセット */
	Map_SetFirstSecondHouse(numTurn,player,cp_idx);
	Dsp_DrawHouseFactory(cp_idx,player,TYPE_DEV_HOUSE);
	return true;
}
//bool Dev_BuildHouseSecond(int player, int cp_idx)
//{
//	Map_BuildHouse(player,cp_idx);
//	DevelopmentResource[player][TYPE_DEV_HOUSE]--;
//	SecondHouse[player]=cp_idx;
//	return true;
//}
 bool Dev_IsErrorBuildHouse(int player, int cp_idx)
 {
	bool isError=false;
	if(Map_IsErrorBuildHouse(player,cp_idx))isError= true;
	if(Rsc_IsErrorBuildHouse(player))isError= true;
	if(DevelopmentResource[player][TYPE_DEV_HOUSE]==0)isError= true;

	return isError;
 }

bool Dev_IsErrorBuildRoadFirst(int player, int cs_idx,int numHouse)
{
	bool isError=false;
	//if(Map_IsErrorBuildRoad(player,cs_idx))isError=true;
	//int firstHouse= numHouse==0 ? FirstHouse[player] : SecondHouse[player];
	if(Map_IsErrorBuildRoadFirst(cs_idx,player,numHouse))isError=true;
	return isError;
}
//bool Dev_IsErrorBuildRoadSecond(int player, int cs_idx)
//{
//	bool isError=false;
//	if(Map_IsErrorBuildRoad(player,cs_idx))isError=true;
//	int secondHouse=SecondHouse[player];
//	if(Map_IsErrorBuildRoadFirst(cs_idx,secondHouse))isError=true;
//	return isError;
//}
bool Dev_BuildRoadFirst(int player, int cs_idx)
{
	Map_BuildRoad(player,cs_idx);
	DevelopmentResource[player][TYPE_DEV_ROAD]--;
	Map_DrawRoad(cs_idx);
	return true;
}
bool Dev_BuildFactory(int player, int cp_idx)
{
	bool doBuild=true;
#if DEBUG_BUILD_FACTORY
#else
	if(Map_IsErrorBuildFactory(player,cp_idx))doBuild = false;
	if(Rsc_IsErrorBuildFactory(player))doBuild=false;
	if(DevelopmentResource[player][TYPE_DEV_FACTORY]==0)doBuild=false;

	if(doBuild){
		Rsc_BuildFactory(player);
		Map_BuildFactory(cp_idx);
		DevelopmentResource[player][TYPE_DEV_FACTORY]--;
		DevelopmentResource[player][TYPE_DEV_HOUSE]++;
		Act_AddPoint(player);
		Dsp_DrawHouseFactory(cp_idx,player,TYPE_DEV_FACTORY);
	}
#endif
	return doBuild;
}
bool Dev_CostAndBuildRoad(int player, int cs_idx)
{
		Rsc_BuildRoad(player);
		Dev_BuildRoad(player,cs_idx);
		return true;
}
bool Dev_BuildRoad(int player, int cs_idx){
	bool doBuild=true;
#if DEBUG_BUILD_ROAD == 1
	Map_TestCase1ForDev_BuildHouse();
	bool f_debug_isError=false;
	f_debug_isError=Map_IsErrorBuildRoad(0,15);
	f_debug_isError=Map_IsErrorBuildRoad(0,10);
	f_debug_isError=Map_IsErrorBuildRoad(0,14);
	f_debug_isError=Map_IsErrorBuildRoad(0,0);

#elif DEBUG_BUILD_ROAD == 2
	bool f_debug_doBuild=false;
	Rsc_TestCase1ForBuildRoad();
	f_debug_doBuild=Rsc_IsErrorBuildRoad(player);
	Rsc_TestCase2ForBuildRoad();
	f_debug_doBuild=Rsc_IsErrorBuildRoad(player);
	Rsc_TestCase3ForBuildRoad();
	f_debug_doBuild=Rsc_IsErrorBuildRoad(player);
#else
	//if(Map_IsErrorBuildRoad(player,cs_idx))doBuild=false;
	//if(Rsc_IsErrorBuildRoad(player))doBuild=false;
	//if(DevelopmentResource[player][TYPE_DEV_ROAD]==0)doBuild=false;

	if(doBuild){
		Map_BuildRoad(player,cs_idx);
		DevelopmentResource[player][TYPE_DEV_ROAD]--;
		Map_DrawRoad(cs_idx);
	}
#endif
	return doBuild;
}
bool Dev_IsErrorBuildRoad_(int player)
{
	bool isError=false;
	if(DevelopmentResource[player][TYPE_DEV_ROAD]==0)isError=true;
	return isError ;

}
bool Dev_IsErrorBuildRoad(int player,int cs_idx)
{
	bool isError=false;
	if(Map_IsErrorBuildRoad(player,cs_idx))isError=true;
	if(Rsc_IsErrorBuildRoad(player))isError=true;
	if(DevelopmentResource[player][TYPE_DEV_ROAD]==0)isError=true;
	return isError ;
}
bool Dev_IsErrorBuildFactory(int player,int cp_idx)
{
	bool isError=false;
		if(Map_IsErrorBuildFactory(player,cp_idx))isError=true;
	if(Rsc_IsErrorBuildFactory(player))isError=true;
	if(DevelopmentResource[player][TYPE_DEV_FACTORY]==0)isError=true;
	return isError;
}
/*!
 * @brief プレイヤの建築資源の情報を返す。
 * @param [player] 現在のプレイヤ番号。
 * @param [val] 文字情報の格納先。返り値
 */
void Dev_GetInfo_DevRscInHand(int player,string *val)
{
	stringstream stream;
	for(int i=0;i<TYPE_DEV_END;i++)
	{
		stream.str("");
		stream<<DevelopmentResource[player][i];
		val[i]=stream.str();
	}
}

