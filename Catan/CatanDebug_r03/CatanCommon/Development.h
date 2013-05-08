/*!
 * @file Development.h
 * @brief 建築資源と建設アクトの制御
 * @author semitsu
 * @date 12.10.02 開始
 */
#ifndef __DEVELOPMENT_H
#define __DEVELOPMENT_H
#include "common.h"
#include <iostream>
//#include "Map.h"
#include "Resource.h"

#define NUM_OF_EACH_HOUSE (5)
#define NUM_OF_EACH_FACTORY (4)
#define NUM_OF_EACH_ROAD (15)
void Dev_SaveState(void);
void Dev_LoadState(void);
bool Dev_BuildHouse(int player, int cp_idx);
//bool Dev_BuildHouseFirst(int player, int cp_idx);
bool Dev_BuildHouseFirst(int player, int cp_idx,int numTurn);
bool Dev_CostAndBuildRoad(int player, int cs_idx);
bool Dev_BuildRoad(int player, int cs_idx);
bool Dev_BuildRoadFirst(int player, int cs_idx);
//bool Dev_BuildHouseSecond(int player, int cp_idx);
bool Dev_BuildFactory(int player, int cp_idx);
void Dev_GetInfo_DevRscInHand(int player,string *val);
 bool Dev_IsErrorBuildHouse(int player, int cp_idx);
//bool Dev_IsErrorBuildRoad(int player);
bool Dev_IsErrorBuildRoad_(int player);
bool Dev_IsErrorBuildRoad(int player,int cs_idx);
bool Dev_IsErrorBuildRoadFirst(int player, int cs_idx,int numHouse);
//bool Dev_IsErrorBuildRoadFirst(int player, int cs_idx);
//bool Dev_IsErrorBuildFactory(int player);
bool Dev_IsErrorBuildFactory(int player,int cp_idx);

//void Dev_BuildHouse(int player, int cp_idx);
//void Bld_IsErrorBuildHouse(int player, TypeResource rscType);//公開しない

#endif //__BUILDING_H