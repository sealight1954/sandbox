/*! 
	@file Map.h
	@brief マップ描画、マップ上の建物の配置を管理
	@author semitsu
	@date 12.10.01 開始
*/
#ifndef __MAP_H
#define __MAP_H
/* インクルードファイル */
#include "common.h"
#include <iostream>

/* --------変数・マクロ宣言 --------------------------------*/
#define IN_VAL_NUM_HEXA (-1)
/*  ---------------------------------------------------------*/
enum Type_ScanLongestRoad {TYPE_LR_ANCHOR=0,TYPE_LR_1ROUTE=1,TYPE_LR_2ROUTE=2,TYPE_LR_NO_ROAD,TYPE_LR_NOT_MINE};

/*関数宣言*/
void IN_InitMap();
void IN_InitMapContents(void);
void IN_InitDrawMap();
/* Disp.hからよびだす */
void IN_GetIndex_ChildCPofCS(int cs_idx,int &cp_idx1,int &cp_idx2);
void IN_GetIndex_ChildCPofCH(int ch_idx,int &cp_idx1,int &cp_idx2);

void Map_BuildHouse(int player,int cp_idx);
void Map_BuildRoad(int player, int cs_idx);
void Map_BuildFactory(int cp_idx);
bool Map_MoveThief(int player,int ch_idx,int enemy_player);
void Map_ProduceResource(int numDice);
void Map_ProduceResourceFirst(void);

bool Map_IsErrorBuildHouse(int player, int cp_idx);
bool Map_IsErrorBuildFactory(int player, int cp_idx);
bool Map_IsErrorBuildRoad(int player, int cs_idx);
//bool Map_IsErrorBuildRoadFirst(int cs_idx,int cp_idx);
void Map_SetFirstSecondHouse(int numTurn,int player,int cp_idx);
bool Map_IsErrorBuildRoadFirst(int cs_idx,int player,int numHouse);
bool Map_IsErrorMoveThief(int player, int ch_idx, int enemy_player);
bool Map_IsErrorBuildHouseFirst(int player, int cp_idx);
bool Map_IsErrorExchange(int player,int change_num,int input_rsc_hand);
bool Map_IsHouseThere(int cp_idx);

void Map_GetInfo_Dummy(int player,string *val);
void Map_DrawRoad(int cs_idx);
void Map_DrawHouseFactory(int cp_idx);
void Map_DrawMap(void);
int Map_GetLongestRoadPlayer(void);
int Map_CalcLongestRoad(int player);

void Map_SaveState(void);
/* debug */
void Map_TestCase1ForDev_BuildHouse(void);
void Map_TestCase1ForCrd_Soldier(void);
void Map_TestCase1ForLongestRoad();
void Map_TestCase2ForLongestRoad();


#endif //__MAP_H
