/*!
	@file Resource.h
	@brief ê∂éYéëåπÇÃä«óù
	@author semitsu
	@date 12.10.01 äJén
*/
#ifndef __RESOURCE_H
#define __RESOURCE_H
#include "common.h"

#define NUM_OF_EACH_RESOURCE (19)


//void Rsc_MasterToHand(int player, TypeResource rscType, int num);
void Rsc_MasterToHand(int player, int rscType, int num=1);
void Rsc_CostResouce(int player, int rscType, int num);
void Rsc_ExChangeResource(int player1,int rscType1,int num1,
	int player2, int rscType2, int num2);
bool Rsc_DoCostResource(int player, int rscType, int num);
bool Rsc_DoGetResource(int rscType,int num);/*éRéDÇ…éëåπÇ™Ç†ÇÈÇ©*/
//void Rsc_Monopoly(int player, int rscType);
bool Rsc_Monopoly(int player, int rscType);
bool Rsc_DoHarvest(int player,int rscType0,int rscType1);
bool Rsc_DoHarvest1(int player,int rscType);
bool Rsc_IsErrorDoHarvest(int player,int rscType0,int rscType1);
bool Rsc_IsErrorDoHarvest1(int player,int rscType);
bool Rsc_IsErrorResourceOnHandZero(int player,int rscType);
//bool Rsc_IsErrorResourceOnHand(int player, int *type_tbl,int *cost,const int size);
bool Rsc_Exchange_1_1(int player1,int player2 , int* rscOut1,int* rscOut2);

void Rsc_BuildHouse(int player);
void Rsc_BuildFactory(int player);
void Rsc_BuildRoad(int player);
void Rsc_ExchangeDevCard(int player);
void Rsc_Exchange(int player,int numRscHand,int type_hand_rsc,int type_master_rsc);

bool Rsc_IsErrorBuildHouse(int player);
//bool Rsc_IsErrorBuildDevCard(int player);
bool Rsc_IsErrorBuildRoad(int player);
bool Rsc_IsErrorBuildFactory(int player);
bool Rsc_IsErrorExchangeDevCard(int player);
bool Rsc_IsErrorExchange(int player,int numRscHand,int type_rsc_hand,int type_rsc_master);

int Rsc_GetPlayerResourceInfo(int player,int type);
void Rsc_GetInfo_RscInMaster(int player,string *val);
void Rsc_GetInfo_RscInHand(int player,string *val);
int Rsc_GetSumOfRscInHand(int player);
void Rsc_MoveResource(int dst_player,int src_player);
void Rsc_HandToMaster(int player,int rscType);


void Rsc_TestCase1ForBuildHouse(void);
void Rsc_TestCase2ForBuildHouse(void);
void Rsc_TestCase3ForBuildHouse(void);
void Rsc_TestCase4ForBuildHouse(void);
void Rsc_TestCase5ForBuildHouse(void);
void Rsc_TestCase1ForBuildRoad(void);
void Rsc_TestCase2ForBuildRoad(void);
void Rsc_TestCase3ForBuildRoad(void);


#endif //__RESOURCE_H