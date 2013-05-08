/*!
	@file Act.h
	@brief 入力制御と状態遷移、各アクションの制御
	@author semitsu
	@date 12.09.30
*/
#ifndef __ACT_H
#define __ACT_H
/* インクルードファイル */
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "common.h"
//#include "Resource.h"
//#include "Map.h"
//#include "Disp.h"
//#include "Development.h"
//#include "DevCard.h"

using namespace cv;

void IN_InitAct(void);
void InitRand();
inline int Act_Rand(int max);
int Act_RandCategory(vector<int> vec);
int Act_RandCategory(int *vec,int size);
int Act_GetDiceNum(void);
//enum CatanMode {MODE_INITIAL_TURN,MODE_BEFORE_DICE,MODE_AFTER_DICE};
void Act_SetWaitFlg(int input_but,bool flg=true);
//void Act_SetWaitFlg(int input_but);
void ActUpdateRegionWaitFlg(int input_region);
void Act_SetWaitMapFlg(bool flg);
//void Act_AddPoint(int player);
bool Act_IsMeWaited(int me_player);
void Act_ClearWaitFlg(void);
void Act_UpdateButtonLst(int disp_player = NUM_OF_PLAYERS);
//void Act_UpdateButtonLstAll(void);
void Act_PlayerChange(int player=NO_PLAYER);
void Act_PlayerChangeInv(void);
void Act_AddPoint(int player,int point=1);
void Act_SubPoint(int player,int point=1);
int Act_GetInputRegion(int but_num);
int Act_GetCurrentPlayer(void);
//int Act_GetDiceNum(int min,int max);
/* Act_Clientからの問い合わせ用 */
bool Act_GetWaitMapFlg(void);
bool Act_GetWaitFlg(int i);
extern void(*Act_getStrInfoFuncPtr[])(int,string *str);
extern int InputRegionIndices[];
int Act_GetWaitPlayer();
#endif