/*!
 * @file DevCard.h
 * @brief DevelopmentCardの使用を制御
 * @author semitsu
 * @date 12.10.09 開始
 */
#ifndef __DEV_CARD_H
#define __DEV_CARD_H
#include "common.h"
//#include "Resource.h"
//#include "Map.h"
bool Crd_GetDevCard(int player);
//bool Crd_GetDevCard(int player);
//bool Crd_GetDevCard(int player,TypeDevCard type);
int Crd_GetSumOfCard(void);
int Crd_GetLargestArmyPlayer(void);
bool Crd_Monopoly(int player, int typeRsc);
bool Crd_Soldier(int player, int cp_idx, int enemy_player);
bool Crd_RoadBuilding(int player, int cs_idx1,int cs_idx2);
bool Crd_Harvest(int player, int typeRsc1,int typeRsc2);
bool Crd_Point(int player);
void Crd_CostDevCard(int player,int type);
void Crd_GetInfo_DevCardOpen(int player,string *val);
void Crd_GetInfo_DevCardClose(int player,string *val);
bool Crd_IsErrorNoCard(int player,int crdType);
bool Crd_IsErrorDoHarvest(int player, int ch_idx, int enemy_player);/* 使ってない */
bool Crd_IsErrorDoSoldier(int player, int ch_idx, int enemy_player);


#endif //__DEV_CARD_H