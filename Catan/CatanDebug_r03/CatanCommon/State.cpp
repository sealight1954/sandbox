/*!
 * @file State.cpp
 * @brief 状態遷移を制御
 * @author semitsu
 * @date 12.10.23 開始
 * @date 12.10.31 BF,INについて状態遷移完成
 */
#include "State.h"
#include "Act.h"
#include "Disp.h"
#include "DevCard.h"
#include "Resource.h"
#include "Development.h"
#include "Map.h"
#include "Error.h"
#if DEBUG_GAME
static int mode_l=STATE_L_ST;
static int mode_m=STATE_ST_WAIT_GAME_START;
#else
static int mode_l=STATE_L_ST;
static int mode_m=STATE_ST_WAIT_GAME_START;
#endif
static int WaitButtonLst_AF_END[]={
	BUT_GET_DEV_CARD,BUT_CHANGE_2_1,BUT_CHANGE_3_1,BUT_CHANGE_4_1,                              /* 交換ボタン */
	BUT_ACT_DEVELOP,BUT_ACT_USE_CARD,BUT_ACT_DICE,BUT_ACT_TURN_END                              /* アクション */
};
static int size_WaitButtonLst_AF_END=sizeof(WaitButtonLst_AF_END)/sizeof(int);
static void St_MoveStateAction_AF_CHANGE(CatanInput input);

static bool St_EventAndAction(bool (condFunc)(int),bool ActionFunc0(int),bool ActionFunc1(int),int dst_mode_m0,int dst_mode_m1,int input=-1);
static bool St_EventAndActionBeta(bool (condFunc)(int,int),bool ActionFunc0(int,int),bool ActionFunc1(int,int),int dst_mode_m0,int dst_mode_m1,int input0,int input1);
static bool St_EventAndActionGamma(bool (condFunc)(int,int,int),bool ActionFunc0(int,int,int),bool ActionFunc1(int,int,int),int dst_mode_m0,int dst_mode_m1,int input0,int input1,int input2);
static void St_MoveStateAction_BF_SEVEN_BURST(CatanInput input);
static void St_MoveStateAction_AF_CHANGE_1_1(CatanInput input);
//static void St_GetDstState(CatanInput input,int &cur_player,bool *waitFlg,bool &waitMapFlg){
//
//}

/*!
 * @brief 受け付ける入力を更新する
 * @param [waitButtonLst,size] フラグを更新するボタンのリスト
 * @param [waitFlg,waitMapFlg] 入力待ちフラグ
 */
void St_UpdateWaitFlgFromButLst(int *waitButtonLst,int size,bool waitMapFlg,bool *dst_waitFlg,bool &dst_waitMapFlg)
{
	for(int i=0;i<BUT_END;i++){
		/* reset */
		dst_waitFlg[i]=false;
	}
	for(int i=0;i<size;i++)
	{
		dst_waitFlg[waitButtonLst[i]]=true;
	}
	dst_waitMapFlg=waitMapFlg;
}
/*!
 * @brief 受け付ける入力を更新する
 * @param [mode_l,mode_m] 現在の状態
 * @param [waitFlg,waitMapFlg] 入力待ちフラグ
 */
void St_UpdateWaitFlgFromMode(void/*int mode_l,int mode_m*//*,bool *waitFlg,bool &waitMapFlg*/)
{
	Act_ClearWaitFlg();
	int player=Act_GetCurrentPlayer();
	//if(mode_l==STATE_L_AF)
	//{
	switch(mode_m)
	{
	case STATE_ST_WAIT_GAME_START:
		Dsp_DrawMsg("Waiting for new players");
		
		break;
	case STATE_IN_CHOOSE_HOUSE:
	case STATE_IN_CHOOSE_ROAD:
		Act_SetWaitMapFlg(true);
		Dsp_DrawMsg("Waiting:Map");
		break;
	case STATE_BF_IN:
		//ActUpdateRegionWaitFlg(INPUT_CRD_CLOSED);
		Act_SetWaitFlg(BUT_ACT_DICE);
		Act_SetWaitFlg(BUT_ACT_USE_CARD);
		Dsp_DrawMsg("Waiting:Dice UseCard");
		break;
	case STATE_BF_USE_CRD:
		Act_SetWaitFlg(BUT_CRD_SOLDIER_CLOSED);
		if(!Crd_IsErrorNoCard(player,TYPE_CRD_SOLDIER))Act_SetWaitFlg(BUT_CRD_SOLDIER_CLOSED);
		Act_SetWaitFlg(BUT_ACT_UNDO);
		Dsp_DrawMsg("Waiting:Soldier");
		break;
	case STATE_BF_SEVEN_BURST:
		/* プレイヤーの資源情報表示変更忘れないように */
		ActUpdateRegionWaitFlg(INPUT_RSC_PLAYER);
		Dsp_DrawMsg("Waiting:RscInHand");
		break;
	case STATE_BF_USE_CRD_SOLDIER:
	case STATE_BF_SEVEN_MOVE_THIEF:
	case STATE_AF_USE_CRD_SOLDIER:
		Act_SetWaitMapFlg(true);
		ActUpdateRegionWaitFlg(INPUT_PLAYER);
		
		Dsp_DrawMsg("Waiting:Map Player");
		break;
	case STATE_AF_IN:
		ActUpdateRegionWaitFlg(INPUT_ACTION);
		ActUpdateRegionWaitFlg(INPUT_EXCHANGE);
		Act_SetWaitFlg(BUT_ACT_DICE,false);
		Dsp_DrawMsg("Waiting:Action Exchange");
		//St_UpdateWaitFlgFromButLst(WaitButtonLst_AF_END,size_WaitButtonLst_AF_END,false,waitFlg,waitMapFlg);
		break;
	case STATE_AF_BUILD:
		//ActUpdateRegionWaitFlg(INPUT_DEV_RSC);
		Act_SetWaitFlg(BUT_ACT_UNDO);
		Act_SetWaitMapFlg(true);
		Dsp_DrawMsg("Waiting:Map");
		break;
		/* 使ってない */
	//case STATE_AF_BUILD_HOUSE:
	//case STATE_AF_BUILD_ROAD:
	//case STATE_AF_BUILD_FACTORY:
	//	Act_SetWaitMapFlg(true);
	//	Dsp_DrawMsg("Waiting:Map Development");
	//	break;
	case STATE_AF_CHANGE_2_1:
	case STATE_AF_CHANGE_3_1:
	case STATE_AF_CHANGE_4_1:
		ActUpdateRegionWaitFlg(INPUT_RSC_MASTER);
		ActUpdateRegionWaitFlg(INPUT_RSC_PLAYER);
		Act_SetWaitFlg(BUT_ACT_UNDO);
		Dsp_DrawMsg("Waiting:RscInHand Master");
		break;
	case STATE_AF_CHANGE_1_1_OUT_CRD:
		//ActUpdateRegionWaitFlg(INPUT_YESNO);
		Act_SetWaitFlg(BUT_YESNO_YES);
		ActUpdateRegionWaitFlg(INPUT_RSC_PLAYER);
		Dsp_DrawMsg("select RscInHand then Yes");
	case STATE_AF_CHANGE_1_1_IN_CRD:
		Act_SetWaitFlg(BUT_YESNO_YES);
		ActUpdateRegionWaitFlg(INPUT_RSC_MASTER);
		Dsp_DrawMsg("select RscInMaster then Yes");
	case STATE_AF_CHANGE_1_1_REQUEST:
		ActUpdateRegionWaitFlg(INPUT_YESNO);
		Dsp_DrawMsg("Exchange Request Waiting:Yes/No");

	case STATE_AF_USE_CRD:
		for(int i=0;i<TYPE_CRD_END;i++)
		{
			if(!Crd_IsErrorNoCard(player,i))Act_SetWaitFlg(BUT_CRD_MONOPOLY_CLOSED+i);
		}
		Act_SetWaitFlg(BUT_ACT_UNDO);
		Dsp_DrawMsg("Waiting:DevCard");
		break;
	case STATE_AF_USE_CRD_MONOPOLY:
	case STATE_AF_USE_CRD_HARVEST:
		ActUpdateRegionWaitFlg(INPUT_RSC_MASTER);
		Dsp_DrawMsg("Waiting:RscInMaster");
		break;
	case STATE_AF_USE_CRD_ROAD:
		Act_SetWaitMapFlg(true);
		Dsp_DrawMsg("Waiting:Map");
		break;
	}

}
/*!
 * @brief カタンプレイヤーがそろったときに呼び出し、STATE_L_INに移行
 * @param [input] なんかいれとく。使わない
 */
static void St_MoveStateAction_ST_WAIT_GAME_START(CatanInput input)
{
	mode_l=STATE_L_IN;
	mode_m=STATE_IN_CHOOSE_HOUSE;
}
/*!
 * @brief STATE_AFの初期状態からの条件判断（ほぼ無条件）・状態遷移とアクション
 * @param [] 
 */
static void St_MoveStateAction_AF_IN(CatanInput input)
{
	//int cur_player=Act_GetCurrentPlayer();
	//Map_SaveState();

	switch(input.but_num){
	case BUT_ACT_DEVELOP:
		mode_m=STATE_AF_BUILD;
		break;
	case BUT_CHANGE_2_1:
		St_MoveStateAction_AF_CHANGE(input);
		mode_m=STATE_AF_CHANGE_2_1;
		//mode_m=STATE_AF_CHANGE;
		break;
	case BUT_CHANGE_3_1:
		St_MoveStateAction_AF_CHANGE(input);
		//mode_m=STATE_AF_CHANGE;
		mode_m=STATE_AF_CHANGE_3_1;
		break;
	case BUT_CHANGE_4_1:
		St_MoveStateAction_AF_CHANGE(input);
		//mode_m=STATE_AF_CHANGE;
		mode_m=STATE_AF_CHANGE_4_1;
		break;
	case BUT_CHANGE_1_1:
		/* この関数初回呼び出し */
		St_MoveStateAction_AF_CHANGE_1_1(input);
		mode_m=STATE_AF_CHANGE_1_1_OUT_CRD;
		break;
	case BUT_ACT_USE_CARD:
		mode_m=STATE_AF_USE_CRD;
		break;
	case BUT_GET_DEV_CARD:
		St_EventAndAction(&Rsc_IsErrorExchangeDevCard,NULL,&Crd_GetDevCard,mode_m,mode_m,input.player/*,mode_l,mode_m*/);
		break;
	case BUT_ACT_TURN_END:
		mode_l=STATE_L_BF;
		mode_m=STATE_BF_IN;
		Act_PlayerChange();//cur_player++;
		Dsp_DrawMsg("next player",0);
		break;
	case BUT_ACT_UNDO:
		switch(mode_m){
		case STATE_AF_CHANGE_2_1:
		case STATE_AF_CHANGE_3_1:
		case STATE_AF_CHANGE_4_1:
		case STATE_AF_BUILD:
			mode_m=STATE_AF_IN;
			break;
		}
		break;
	}
	//表示更新
}
/*!
 * @brief STATE_AF_BUILDの初期状態からの条件判断（ほぼ無条件）・状態遷移とアクション
 * @param [] 
 */
static void St_MoveStateAction_AF_BUILD(CatanInput input){
	//int cur_player=Act_GetCurrentPlayer();
		if(input.but_num==BUT_ACT_UNDO)
	{
		mode_m=STATE_AF_IN;
	}
	else
#if AF_BUILD_FUNC
	switch(input.but_num){
	case BUT_DEV_:
		St_EventAndAction(&Rsc_IsErrorBuildHouse,NULL,NULL,STATE_AF_BUILD,STATE_AF_BUILD_HOUSE,input.player);
		break;
	case TYPE_DEV_ROAD:
		St_EventAndAction(&Err_IsErrorBuildRoad1,NULL,NULL,STATE_AF_BUILD,STATE_AF_BUILD_ROAD,input.player);
		break;
	case TYPE_DEV_FACTORY:
		St_EventAndAction(&Err_IsErrorBuildFactory1,NULL,NULL,STATE_AF_BUILD,STATE_AF_BUILD_FACTORY,input.player);
		break;
	}
#else
	{
		switch(input.obj.typeObj){
		case TYPE_OBJ_POINT:
			if(Map_IsHouseThere(input.obj.id))
			{
				St_EventAndActionBeta(&Dev_IsErrorBuildFactory,NULL,&Dev_BuildFactory,STATE_AF_BUILD,STATE_AF_IN,input.player,input.obj.id);
			}else{
				St_EventAndActionBeta(&Dev_IsErrorBuildHouse,NULL,&Dev_BuildHouse,STATE_AF_BUILD,STATE_AF_IN,input.player,input.obj.id);
			}
			break;
		case TYPE_OBJ_SIDE:
			St_EventAndActionBeta(&Dev_IsErrorBuildRoad, NULL, &Dev_CostAndBuildRoad,STATE_AF_BUILD,STATE_AF_IN, input.player , input.obj.id);
			break;
			//case TYPE_DEV_FACTORY:
			//break;
		}
	}
#endif
}
/*!
 * @brief state     : STATE_AF_BUILD_HOUSE
 *        condition : 家を建てれるか・家を持っているか
 *        action    : 家を建てる
 *        dst_state0 : STATE_AF_BUILD_HOUSE
 *        dst_state1 : STATE_AF_IN
 * @param [input]  
 */
#if AF_BUILD_FUNC
static void St_MoveStateAction_AF_BUILD_HOUSE(CatanInput input)
{
	int cur_player=Act_GetCurrentPlayer();
	if(input.obj.typeObj==TYPE_OBJ_POINT)
	St_EventAndActionBeta(&Map_IsErrorBuildHouse,NULL,&Dev_BuildHouse,STATE_AF_BUILD_HOUSE,STATE_AF_IN,cur_player,input.obj.id);
}
#endif
/*!
 * @brief state     : STATE_AF_BUILD_ROAD
 *        condition : 道を建てれるか・道を持っているか
 *        action    : 道を建てる
 *        dst_state0 : STATE_AF_BUILD_ROAD
 *        dst_state1 : STATE_AF_IN
 * @param [input]  
 */
#if AF_BUILD_FUNC
static void St_MoveStateAction_AF_BUILD_ROAD(CatanInput input)
{
	int cur_player=Act_GetCurrentPlayer();
	if(input.obj.typeObj==TYPE_OBJ_SIDE)
	St_EventAndActionBeta(&Err_IsErrorBuildRoad2,NULL,&Dev_BuildRoad,STATE_AF_BUILD_ROAD,STATE_AF_IN,cur_player,input.obj.id);
}
#endif
/*!
 * @brief state     : STATE_AF_BUILD_ROAD
 *        condition : 道を建てれるか・道を持っているか
 *        action    : 道を建てる
 *        dst_state0 : STATE_AF_BUILD_ROAD
 *        dst_state1 : STATE_AF_IN
 * @param [input]  
 */
#if AF_BUILD_FUNC
static void St_MoveStateAction_AF_BUILD_FACTORY(CatanInput input)
{
	int input.player=Act_GetCurrentPlayer();
	St_EventAndActionBeta(&Err_IsErrorBuildFactory2,NULL,&Dev_BuildRoad,STATE_AF_BUILD_ROAD,STATE_AF_IN,input.player,input.obj.id);
}
#endif
//static void St_MoveStateAction_AF_GET_DEV_CARD(CatanInput input)
//{
//	int input.player=Act_GetCurrentPlayer();
//	bool isError=false;
//	/* 手札の資源が足りない */
//	if(Rsc_IsErrorExchangeDevCard(cur_player))isError=true;
//	/* 山札にカードがない */
//	int sum=Crd_GetSumOfCard();
//	if(sum==0)isError=true;
//	if(!isError){
//		Crd_GetDevCard(cur_player);
//	}
//}
/*!
 * @brief 交換の入力待ちからの状態遷移
 *        特例で状態に入ったときのアクションがある（MoveStateAction_AF_INから呼ばれる）
 * @param [input] 交換ボタン、または資源の種類（山札・手札）
 */
static void St_MoveStateAction_AF_CHANGE(CatanInput input)
{
	//int player=Act_GetCurrentPlayer();
	static int change_num=-1;/* 交換に必要な資源の数 */
	static int input_rsc_hand=-1,input_rsc_master=-1;
	if(input.but_num==BUT_ACT_UNDO)
	{
		mode_m=STATE_AF_IN;
	}
	else if(input.but_num==BUT_CHANGE_2_1||input.but_num==BUT_CHANGE_3_1||input.but_num==BUT_CHANGE_4_1)
	{
		/* 入力待ち状態に入るときの呼び出し 遷移関数からまた呼び出しされておりタイミングが違う。 */
		change_num=input.but_num - BUT_CHANGE_2_1 + 2;
		input_rsc_hand=-1;
		input_rsc_master=-1;
	}
	else
	{
		/* 通常の呼び出し */
		int input_region=Act_GetInputRegion(input.but_num);
		if(input_region==INPUT_RSC_MASTER)
		{
			input_rsc_master=input.but_num-BUT_MASTER_LUMBER;
		}else{
			input_rsc_hand=input.but_num-BUT_PLAYER_LUMBER;
		}
		/* 条件判断 */
		bool doExchange=true;
			/* 入力エラーなし */
		if(input_rsc_hand == -1 || input_rsc_master == -1)doExchange=false;
				/* 資源エラーなし */
		if(!doExchange||Rsc_IsErrorExchange(input.player,change_num,input_rsc_hand,input_rsc_master))doExchange=false;
		if(!doExchange||Map_IsErrorExchange(input.player,change_num,input_rsc_hand))doExchange=false;
		//if(input_rsc_hand != -1 && input_rsc_master != -1)
		//{
		//	if(!Rsc_IsErrorExchange(input.player,change_num,input_rsc_hand,input_rsc_master))
		//	{
		if(doExchange){
				Rsc_Exchange(input.player,change_num,input_rsc_hand,input_rsc_master);
				mode_m=STATE_AF_IN;
				/* 初期化 */
				change_num=-1;
				input_rsc_hand=-1;
				input_rsc_master=-1;
		}
		//	}
		//}
	}
}

static void St_MoveStateAction_AF_CHANGE_1_1(CatanInput input)
{
	static bool isFirst=true;
	static int rscOut[5];
	static int rscIn[5];
	static bool requestReject[NUM_OF_PLAYERS]={0};
	static int proposePlayer;
	if(isFirst)
	{
		for(int i=0;i<TYPE_RSC_END;i++)
		{
			rscOut[i]=0;
			rscIn[i]=0;
		}
		proposePlayer=input.player;
		isFirst=false;
	}
	/* Undoなし。交換打診時のUNDOがめんどい */
	//if(input.but_num==BUT_ACT_UNDO)
	//{
	//	if(mode_m==STATE_AF_CHANGE_1_1_OUT_CRD)
	//	{
	//		isFirst=true;
	//		mode_m=STATE_AF_IN;
	//	}
	//	else if(mode_m==STATE_AF_CHANGE_1_1_IN_CRD)
	//	{
	//		mode_m=STATE_AF_CHANGE_1_1_OUT_CRD;
	//	}
	//}
	/* まず、出すカード選ぶ */
	if(mode_m==STATE_AF_CHANGE_1_1_OUT_CRD)
	{
		if(input.but_num==BUT_YESNO_YES)
		{
			/* 次へ */
			mode_m=STATE_AF_CHANGE_1_1_IN_CRD;
		}else
		{
		(rscOut[input.but_num-BUT_PLAYER_LUMBER])++;
		}
	}
	else if(mode_m==STATE_AF_CHANGE_1_1_IN_CRD)
	{

		if(input.but_num==BUT_YESNO_YES)
		{
			/* 次へ */
			mode_m=STATE_AF_CHANGE_1_1_REQUEST;
			Act_PlayerChange(-(proposePlayer + 1));

		}
		else
		{
			(rscIn[input.but_num-BUT_PLAYER_LUMBER])++;
		}
	}
	else if(mode_m=STATE_AF_CHANGE_1_1_REQUEST)
	{
		/* 入力できるかどうかの判断はActC_inputButtonのなかでしている */
		//int cur_player=Act_GetCurrentPlayer();
		
		if(input.but_num==BUT_YESNO_YES && input.player!=proposePlayer)
		{
			if(Rsc_Exchange_1_1(input.player,proposePlayer,rscOut,rscIn))
			{
				/* 交換成立 input.player -> proposePlayer : rscIn */
				/* 交換成立 proposePlayer -> input.player : rscOut */
				mode_m=STATE_AF_IN;
				Act_PlayerChange(proposePlayer);
				/* 初期化 */
				for(int i=0;i<NUM_OF_PLAYERS;i++)requestReject[i]=false;
				isFirst=true;
				for(int i=0;i<TYPE_RSC_END;i++)
				{
					rscOut[i]=0;
					rscIn[i]=0;
				}
				//proposePlayer=0;
			}
		}
		else if(input.but_num==BUT_YESNO_NO && input.player!=proposePlayer)
		{
			requestReject[input.player]=true;
			bool isAllRejected=true;
			for(int i=0;i<NUM_OF_PLAYERS;i++)
			{
				if(i==proposePlayer)continue;
				if(requestReject[i]==false)isAllRejected=false;
			}
			if(isAllRejected)
			{
				/* 全員拒否。交換不成立 */
				mode_m=STATE_AF_IN;
				Act_PlayerChange(proposePlayer);
				/* 初期化 */
				for(int i=0;i<NUM_OF_PLAYERS;i++)requestReject[i]=false;
				isFirst=true;
				for(int i=0;i<TYPE_RSC_END;i++)
				{
					rscOut[i]=0;
					rscIn[i]=0;
				}
			}
		}

	}
}
/*!
 * @brief state     : STATE_AF_USE_CRD,STATE_BF_USE_CRD
 *        condition : カードの種類
 *        action    : 状態遷移のみ
 *        dst_state : STATE_AF_USE_CRD_MONOPOLY,HARVEST,ROAD,SOLDIER,POINT
 * @param [input]  
 */
static void St_MoveStateAction_AF_USE_CRD(CatanInput input)
{
	//int cur_player=Act_GetCurrentPlayer();
	 if(input.but_num==BUT_ACT_UNDO)
	 {
		 mode_m=mode_l==STATE_L_AF ? STATE_AF_IN : STATE_BF_IN;
	 }
	 else
	 {
		 int crdType=input.but_num-BUT_CRD_MONOPOLY_CLOSED;
		 //if(!Crd_IsErrorNoCard(input.player,crdType))
		 //mode_m= mode_l==STATE_L_AF ? STATE_AF_USE_CRD_MONOPOLY+crdType : STATE_BF_USE_CRD_SOLDIER;
		 if(!Crd_IsErrorNoCard(input.player,crdType))
		 {
			 Crd_CostDevCard(input.player,crdType);
			 if(input.but_num==BUT_CRD_POINT_CLOSED)
			 {
				 /* 得点のカードは入力を待たずにアクション・状態遷移 */
				 Crd_Point(input.player);
				 mode_m=STATE_AF_IN;
			 }
			 else
			 {
				 mode_m= mode_l==STATE_L_AF ? STATE_AF_USE_CRD_MONOPOLY + crdType : STATE_BF_USE_CRD_SOLDIER;
			 }
		 }
	 }
}

/*!
 * @brief 独占のカード処理
 * @param [input] 山札の資源指定
 */
static void St_MoveStateAction_AF_CRD_MONOPOLY(CatanInput input)
{
	//int cur_player=Act_GetCurrentPlayer();
	St_EventAndActionBeta(NULL,NULL,&Crd_Monopoly,
		STATE_BF_END,STATE_AF_IN,
		input.player,input.but_num-BUT_MASTER_LUMBER);
}
/*!
 * @brief 収穫のカード処理 入力を一つ前まで保存
 * @param [input] 山札の資源指定 
 */
static void St_MoveStateAction_AF_CRD_HARVEST(CatanInput input)
{
	static int prev_input=-1;
	int cur_input=input.but_num-BUT_MASTER_LUMBER;
	//int cur_player=Act_GetCurrentPlayer();
	St_EventAndActionGamma(
		&Rsc_IsErrorDoHarvest,NULL,&Crd_Harvest,
		STATE_AF_USE_CRD_HARVEST,STATE_AF_IN,
		input.player,prev_input,cur_input);
	prev_input=cur_input;
	if(mode_m==STATE_AF_IN)prev_input=-1;/* 収穫動作を行ったら初期化 */
}
/* 2状態作るバージョン。 カード選択の関数がきれいにかけなくなるので保留 */
//static void St_MoveStateAction_AF_CRD_HARVEST1(CatanInput input)
//{
//	int cur_input=input.but_num-BUT_MASTER_LUMBER;
//	int input.player=Act_GetCurrentPlayer();
//	St_EventAndActionBeta(&Rsc_IsErrorDoHarvest1,NULL,&Rsc_DoHarvest1,
//		STATE_AF_USE_CRD_HARVEST1,STATE_AF_USE_CRD_HARVEST2,
//		input.player,cur_input);
//}
//static void St_MoveStateAction_AF_CRD_HARVEST1(CatanInput input)
//{
//	int cur_input=input.but_num-BUT_MASTER_LUMBER;
//	int input.player=Act_GetCurrentPlayer();
//	St_EventAndActionBeta(&Rsc_IsErrorDoHarvest1,NULL,&Rsc_DoHarvest1,
//		STATE_AF_USE_CRD_HARVEST2,STATE_AF_IN,
//		input.player,cur_input);
//}
/*!
 * @brief 街道のカード処理(way 1 of 2) 同じ操作を2回繰り返したら終了
 * @param [input] CSを指定
 */
static void St_MoveStateAction_AF_CRD_ROAD(CatanInput input)
{
	static int num_road_built=0;
	//int input.player=Act_GetCurrentPlayer();
	if(input.obj.typeObj==TYPE_OBJ_SIDE)
	{
		bool doBuildRoad=!Err_IsErrorBuildRoadCrd(input.player,input.obj.id);
		if(doBuildRoad)
		{
			Dev_BuildRoad(input.player,input.obj.id);
			num_road_built++;
		}
		if(num_road_built>=2)
		{
			/* 道二つできた */
			mode_m=STATE_AF_IN;
			num_road_built=0;
		}
	}
}
/*!
 * @brief 街道のカード処理(way 2 of 2) 同じ操作を2回繰り返したら終了
 * @param [input] CSを指定
 */
//static void St_MoveStateAction_AF_CRD_ROAD1(CatanInput input)
//{
//	int input.player=Act_GetCurrentPlayer();
//	St_EventAndActionBeta(&Err_IsErrorBuildRoadCrd,NULL,&Dev_BuildRoad,
//		STATE_AF_USE_CRD_ROAD1,STATE_AF_USE_CRD_ROAD2,
//		input.player,input.obj.id);
//}
//static void St_MoveStateAction_AF_CRD_ROAD2(CatanInput input)
//{
//	int input.player=Act_GetCurrentPlayer();
//	St_EventAndActionBeta(&Err_IsErrorBuildRoadCrd,NULL,&Dev_BuildRoad,
//		STATE_AF_USE_CRD_ROAD2,STATE_AF_IN,
//		input.player,input.obj.id);
//}
/*!
 * @brief ソルジャーのカード処理 STATE_BF_USE_CRD_SOLDIER STATE_AF_USE_CRD_SOLDIER
 * @param [] 
 */
static void St_MoveStateAction_AF_CRD_SOLDIER(CatanInput input)
{
	//int cur_player=Act_GetCurrentPlayer();
	static int input_ch=NO_OBJ_ID;
	static int input_player=NO_PLAYER;
	/* 入力処理 */
	if(input.obj.typeObj==TYPE_OBJ_HEXAGON)
	{
		input_ch=input.obj.id;
	}
	else if(Act_GetInputRegion(input.but_num)==INPUT_PLAYER)
	{
		input_player=input.but_num-BUT_PLAYER_0;
	}
	/* 条件判断 */
	int dst_mode0,dst_mode1;
	bool (*ActionFunc)(int,int,int);
	if(mode_l==STATE_L_AF&&mode_m==STATE_AF_USE_CRD_SOLDIER)
	{
		dst_mode0=STATE_AF_USE_CRD_SOLDIER;
		dst_mode1=STATE_AF_IN; 
		ActionFunc=&Crd_Soldier;
	}
	else if(mode_l==STATE_L_BF&&mode_m==STATE_BF_USE_CRD_SOLDIER)
	{
		dst_mode0=STATE_BF_USE_CRD_SOLDIER;
		dst_mode1=STATE_BF_IN;
		ActionFunc=&Crd_Soldier;
	}else if(mode_l==STATE_L_BF&&mode_m==STATE_BF_SEVEN_MOVE_THIEF)
	{
		dst_mode0=STATE_BF_SEVEN_MOVE_THIEF;
		dst_mode1=STATE_AF_IN;
		ActionFunc=&Map_MoveThief;
	}
	// = mode_l==STATE_L_AF ? STATE_AF_USE_CRD_SOLDIER :
	//	mode_m==STATE_BF_USE_CRD_SOLDIER ? STATE_BF_USE_CRD_SOLDIER : 
	//	STATE_BF_SEVEN_MOVE_THIEF;
	//int dst_mode1 = mode_l==STATE_L_AF ? STATE_AF_IN :
	//	mode_m==STATE_BF_USE_CRD_SOLDIER ? STATE_BF_IN : 
	//	STATE_AF_IN;
	/* burst後のmove_thiefのとき、mode_lをどうやって判断する？ */
	St_EventAndActionGamma(&Map_IsErrorMoveThief,NULL,ActionFunc,
		dst_mode0,dst_mode1,
		//STATE_AF_USE_CRD_SOLDIER,STATE_AF_IN,
		input.player,input_ch,input_player);
	/* BFからAFへ移行 */
	if(mode_m==STATE_AF_IN/*||mode_m==STATE_BF_IN*/)
	{
		input_ch=NO_OBJ_ID;
		input_player=NO_PLAYER;
		mode_l=STATE_L_AF;
		//if(mode_l==STATE_L_BF)mode_m=STATE_BF_IN;
	}
	else if (mode_m==STATE_BF_IN)
	{
		input_ch=NO_OBJ_ID;
		input_player=NO_PLAYER;
	}
}
static void St_MoveStateAction_AF_CRD_POINT(CatanInput input)
{
	//int player=Act_GetCurrentPlayer();
	/* 呼ばれない（得点カードには入力待ち状態がない） */
}
static void St_MoveStateAction_BF_IN(CatanInput input)
{
	int diceNum;
	switch(input.but_num)
	{
	case BUT_ACT_DICE:
		diceNum = Act_GetDiceNum();

		if(diceNum==7)
		{
			bool sevenBurst=false;
			for(int i=0;i<NUM_OF_PLAYERS;i++)
			{
				if(Rsc_GetSumOfRscInHand(i)>7)
				{
					sevenBurst=true;
				}
			}
			if(sevenBurst)
			{
				/* バーストあり */
				mode_m=STATE_BF_SEVEN_BURST;
				/* 初回呼び出し・アクションは起こさない */
				St_MoveStateAction_BF_SEVEN_BURST(input);
				/* 代案 */
				//上の関数はよばない。
				//	ダイスプレイヤ退避、
				//	最初のバーストプレイヤとその喪失資源数を記憶（ファイル全体で共有）
				//Dsp_DrawMsg("7 Burst",0);
			}
			else
			{
				mode_m=STATE_BF_SEVEN_MOVE_THIEF;
				Dsp_DrawMsg("7 Move Thief",0);
			}
		}
		else
		{
			Map_ProduceResource(diceNum);
			mode_m=STATE_AF_IN;
			mode_l=STATE_L_AF;
		}
		break;
	case BUT_ACT_USE_CARD:
		mode_m=STATE_BF_USE_CRD;
		break;
	}

}

/*!
 * @brief 状態に入るときにはじめの呼び出しをする
 * @param [] 
 */

static void St_MoveStateAction_BF_SEVEN_BURST(CatanInput input)
{
	static bool isFirst=true;
	//static bool isBurst[NUM_OF_PLAYERS];
	//static int burstPlayer,burstSize;
	static int dicePlayer=NO_PLAYER;
	static int burstPlayer=0,burstSize[NUM_OF_PLAYERS]={0};
	static int loseRscNum=0;
	if(isFirst)
	{
		dicePlayer=Act_GetCurrentPlayer();
		for(int i=0;i<NUM_OF_PLAYERS;i++)burstSize[i]=0;
		bool isInitBurstPlayer=false;
		for(int i=0;i<NUM_OF_PLAYERS;i++)
		{
			int sum=Rsc_GetSumOfRscInHand(i);
			if(sum>7)
			{
				burstSize[i]=sum/2;
				if(!isInitBurstPlayer)
				{
					/* 最初のバーストプレイヤを見つける */
					Act_PlayerChange(i);
					burstPlayer=i;
					isInitBurstPlayer=true;
				}
			}
		}//for
		stringstream str;
		str<<"7 Burst lose ";
		str<<burstSize[burstPlayer];
		str<<" rsc";
		Dsp_DrawMsg(str.str(),0);
		loseRscNum=0;

		isFirst=false;
	}
	else
	{
		int rscType=input.but_num - BUT_PLAYER_LUMBER;
		if(!Rsc_IsErrorResourceOnHandZero(burstPlayer,rscType))
		{
			Rsc_HandToMaster(burstPlayer,rscType);
			loseRscNum++;
		}
		if(loseRscNum==burstSize[burstPlayer])/* 一人分終わり */
		{
			bool isFinishedBurst=true;
			for(int i=burstPlayer+1;i<NUM_OF_PLAYERS;i++)
			{
				if(burstSize[i])
				{
					isFinishedBurst=false;
					burstPlayer=i;
					Act_PlayerChange(i);
					loseRscNum=0;
//#ifdef _CATAN_SERVER
//					Act_UpdateButtonLstAll(NUM_OF_PLAYERS);
//#else
//					Act_UpdateButtonLstAll(i);
//#endif
					stringstream str;
					str<<"7 Burst lose ";
					str<<burstSize[burstPlayer];
					str<<" rsc";
					Dsp_DrawMsg(str.str(),0);
					break;
				}
			}//for
			if(isFinishedBurst)
			{
				Act_PlayerChange(dicePlayer);
					Dsp_DrawMsg("Move Thief to Somewhere",0);
				mode_m=STATE_BF_SEVEN_MOVE_THIEF;
				isFirst=true;
				/* 出口 */
			}
		}//if
	}
}

static void St_MoveStateAction_IN_CHOOSE_HOUSE(CatanInput input)
{
	static int numHouse=0;
	Dev_SaveState();
	int cp_idx;
	int player=Act_GetCurrentPlayer();
	/* ここでプレイヤの判断をしなくてもいいはず 2013.02.23 */
	if(input.obj.typeObj == TYPE_OBJ_POINT && input.player == player)
	{
		cp_idx=input.obj.id;
		int numTurn = numHouse / NUM_OF_PLAYERS;
		bool doBuildHouse=true;
		if(Map_IsErrorBuildHouseFirst(input.player,cp_idx))doBuildHouse=false;
		/* 書きかけ */
		if(doBuildHouse)
		{
			Dev_BuildHouseFirst(input.player,cp_idx,numTurn);
			mode_m=STATE_IN_CHOOSE_ROAD;
			if(numHouse++ == (NUM_OF_PLAYERS * 2) - 1)
			{
				numHouse=0;
			}
		}
	}
}
static void St_MoveStateAction_IN_CHOOSE_ROAD(CatanInput input)
{
	static int numRoad=0;
	//int input.player=Act_GetCurrentPlayer();
	if(input.obj.typeObj==TYPE_OBJ_SIDE){
		int cs_idx=input.obj.id;
		bool doBuildRoad=true;
		int numHouse=numRoad/NUM_OF_PLAYERS;
		if(Dev_IsErrorBuildRoadFirst(input.player,cs_idx,numHouse))doBuildRoad=false;
		if(doBuildRoad)
		{
			Dev_BuildRoadFirst(input.player,cs_idx);
			int numTurn=numRoad/NUM_OF_PLAYERS;
			/* 1ターン目、最後以外 */
			if(numTurn==0&&numRoad!=NUM_OF_PLAYERS-1)
			{
				Act_PlayerChange();
			}
			/* 2ターン目、最後以外 */
			else if(numTurn==1&&numRoad!=NUM_OF_PLAYERS*2-1)
			{
				Act_PlayerChangeInv();
			}
			/* 最後 */
			if(numRoad++==(NUM_OF_PLAYERS*2)-1)
			{
				mode_l=STATE_L_BF;
				mode_m=STATE_BF_IN;
				Map_ProduceResourceFirst();
				Dsp_DrawMsg("Let's Catan",0);
				numRoad=0;
			}
			else
			{
				mode_m=STATE_IN_CHOOSE_HOUSE;
			}
			//numRoad;

		}
	}
	/*else if(input.but_num==BUT_ACT_UNDO)
	{
		Dev_LoadState();
		mode_m=STATE_IN_CHOOSE_HOUSE;
		//家建てた数の初期化ができない。。
	}*/
}
/*!
 * @brief 2分岐・1引数（player）の状態遷移
 * @param [] 
 */
static bool St_EventAndAction(bool (condFunc)(int),bool ActionFunc0(int),bool ActionFunc1(int),int dst_mode_m0,int dst_mode_m1,int input)
{
	bool doMoveState1=true;/*condFuncがNULLのときは無条件にActionFunc1*/
	if(condFunc!=NULL && condFunc(input))
	{
		doMoveState1=false;
	}
	if(doMoveState1)
	{
		if(ActionFunc1!=NULL)ActionFunc1(input);
		mode_m=dst_mode_m1;
	}
	else 
	{
		if(ActionFunc0!=NULL)ActionFunc0(input);
		mode_m=dst_mode_m0;
	}
	//else
	//{
	//	printf("error cannot move state\n");
	//}
	return doMoveState1;
	//mode_m=STATE_M_BUILD_HOUSE;
}
/*!
 * @brief 2分岐・2引数（player,input2）の状態遷移
 * @param [] 
 */
static bool St_EventAndActionBeta(bool (condFunc)(int,int),bool ActionFunc0(int,int),bool ActionFunc1(int,int),int dst_mode_m0,int dst_mode_m1,int input0,int input1)
{
	bool doMoveState1 = true;/* condFuncがNULLのときは無条件にActionFunc1 */
	if(condFunc != NULL && condFunc(input0,input1))
	{
		doMoveState1 = false;
	}
	if(doMoveState1)
	{
		if(ActionFunc1 != NULL)ActionFunc1(input0,input1);
		mode_m = dst_mode_m1;
	}
	else 
	{
		if(ActionFunc0 != NULL)ActionFunc0(input0,input1);
		mode_m = dst_mode_m0;
	}
	return doMoveState1;
	//mode_m=STATE_M_BUILD_HOUSE;
}
/*!
 * @brief 2分岐・3引数（player）の状態遷移
 * @param [] 
 */
static bool St_EventAndActionGamma(bool (condFunc)(int,int,int),bool ActionFunc0(int,int,int),bool ActionFunc1(int,int,int),int dst_mode_m0,int dst_mode_m1,int input0,int input1,int input2)
{
	bool doMoveState1=true;/*condFuncがNULLのときは無条件にActionFunc1*/
	if(condFunc!=NULL && condFunc(input0,input1,input2))
	{
		doMoveState1=false;
	}
	if(doMoveState1)
	{
		if(ActionFunc1!=NULL)ActionFunc1(input0,input1,input2);
		mode_m=dst_mode_m1;
	}
	else 
	{
		if(ActionFunc0!=NULL)ActionFunc0(input0,input1,input2);
		mode_m=dst_mode_m0;
	}
	return doMoveState1;
	//mode_m=STATE_M_BUILD_HOUSE;
}
/*!
 * @brief マウスクリックが起こったときに呼び出す。
 *        inputが初期値のまま渡されたときどこへ行く？ 2013.02.17
 * @param [input] マウスクリック時の画面入力情報 
 */
void St_Event(CatanInput input/*,bool *waitFlg,bool &waitMapFlg*/){
	//int cur_player=Act_GetCurrentPlayer();
	
	static void(*MoveActionFunc_AF[STATE_BF_END])(CatanInput)={
		St_MoveStateAction_ST_WAIT_GAME_START,
		St_MoveStateAction_IN_CHOOSE_HOUSE,
		St_MoveStateAction_IN_CHOOSE_ROAD,
		St_MoveStateAction_AF_IN,
		St_MoveStateAction_AF_BUILD,
#if AF_BUILD_FUNC
		St_MoveStateAction_AF_BUILD_HOUSE,
		St_MoveStateAction_AF_BUILD_ROAD,
		St_MoveStateAction_AF_BUILD_FACTORY,
#endif
		St_MoveStateAction_AF_CHANGE,
		St_MoveStateAction_AF_CHANGE,
		St_MoveStateAction_AF_CHANGE,
		St_MoveStateAction_AF_USE_CRD,
		St_MoveStateAction_AF_CRD_MONOPOLY,
		St_MoveStateAction_AF_CRD_HARVEST,
		St_MoveStateAction_AF_CRD_ROAD,
		St_MoveStateAction_AF_CRD_SOLDIER,
		St_MoveStateAction_AF_CHANGE_1_1,
		St_MoveStateAction_AF_CHANGE_1_1,
		St_MoveStateAction_AF_CHANGE_1_1,
		St_MoveStateAction_BF_IN,
		St_MoveStateAction_AF_USE_CRD,
		St_MoveStateAction_AF_CRD_SOLDIER,
		St_MoveStateAction_BF_SEVEN_BURST,
		St_MoveStateAction_AF_CRD_SOLDIER
	};
	MoveActionFunc_AF[mode_m](input);
	St_UpdateWaitFlgFromMode();
	Dsp_UpdateMainWindow();
}
void St_DrawMode(void)
{
	switch(mode_l)
	{
	case STATE_L_IN:
		Dsp_DrawMsg("state:InitialTurn",2);
		break;
	case STATE_L_AF:
		Dsp_DrawMsg("state:AfterDice",2);
		break;
	case STATE_L_BF:
		Dsp_DrawMsg("state:BeforDice",2);
		break;
	}
	Dsp_UpdateMainWindow();
}

/* debug */
void setMode_M(int m)
{
	mode_m=m;
}
void setMode_L(int m)
{
	mode_l=m;
}
int St_GetModeL(void)
{
	return mode_l;
}
int St_GetModeM(void)
{
	return mode_m;
}