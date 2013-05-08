/*!
 * @file Act.cpp
 * @brief 入力制御と状態遷移、各アクションの制御
 * @author semitsu
 * @date 12.09.30 開始
 * @date 12.10.21 Act_UpdateButtonLstOnOff追記
 */
#include <time.h>
#include "Act.h"
#include "Map.h"
#include "Resource.h"
#include "Development.h"
#include "State.h"
#include "DevCard.h"
#include "Disp.h" //dice num 表示
/* --入力制御用変数------------------------------------------------------------------ */
static bool waitFlg[BUT_END]={0};    /*!< 入力待ちフラグ */
static bool waitMapFlg=false;
/*!< 現在入力待ちのプレイヤ。0-3:一人待ち -1 - -4 :(-cur_player) - 1以外待ち */
static int cur_player = 0;
//static int cur_player=0;
/* --ゲーム情報------------------------------------------------------------------ */
//static int modeL=STATE_L_IN;                    /*!< 大きな状態遷移(IN->BF->AF) */
//static int modeM=STATE_IN_CHOOSE_HOUSE;                    /*!< 小さな状態遷移() */
int InputRegionIndices[INPUT_END]={
	//BUT_MAP,
	BUT_PLAYER_0,
	BUT_MASTER_LUMBER,
	BUT_PLAYER_LUMBER,
	BUT_PLAYER_HOUSE,
	BUT_CRD_MONOPOLY_OPEN,
	BUT_CRD_MONOPOLY_CLOSED,
	BUT_GET_DEV_CARD,
	BUT_ACT_DEVELOP,
	BUT_YESNO_YES
};

static int player_point[NUM_OF_PLAYERS]={0};
static int prev_player_point[NUM_OF_PLAYERS]={0};
/* -------------------------------------------------------------------- */
//static void Act_OnMouse( int event, int x, int y, int flag, void* );
//static void Act_UpdateButtonLstOnOff(CatanInput input);
static void Act_GetInfo_CurrentPlayerStr(int player,string *val);
/*!
 * @brief ボタンに表示する情報の取得関数郡。中身は
 * @param [int ] プレイヤナンバーとか。
 * @param [string *str] 情報の格納先。ボタンの数分配列を用意しておく 
 */
void(*Act_getStrInfoFuncPtr[INPUT_END])(int,string *str)={
//volatile static void(*Act_getStrInfoFuncPtr[INPUT_END])(int,string *str)={
		//Rsc_GetInfo_RscInHand,/* debug */
		//Rsc_GetInfo_RscInHand,/* debug */
		//Rsc_GetInfo_RscInHand,/* debug */
		//Rsc_GetInfo_RscInHand/* debug */
		//Map_GetInfo_Dummy,
		Act_GetInfo_CurrentPlayerStr,
		Rsc_GetInfo_RscInMaster,
		Rsc_GetInfo_RscInHand,
		Dev_GetInfo_DevRscInHand,
		Crd_GetInfo_DevCardOpen,
		Crd_GetInfo_DevCardClose,
		Dsp_GetInfo_ExchangeBut,
		Dsp_GetInfo_ActionBut,
		Dsp_GetInfo_YesNoBut

};
void Act_SaveState(void)
{
	memcpy(prev_player_point,player_point,sizeof(int)*NUM_OF_PLAYERS);
}
void Act_LoadState(void)
{
	memcpy(player_point,prev_player_point,sizeof(int)*NUM_OF_PLAYERS);
}
void InitRand()
{
    srand((unsigned int)time(NULL));
}
//void Act_AddPoint(int player)
//{
//	player_point[player]++;
//}
int Act_GetDiceNum()
{
	//int first=rand() % 6 + 1;
	//int second=rand() % 6 + 1;
    //return first + second;
	int dice=rand() % 6 + rand() % 6 + 2;
	stringstream stream;
	stream<<"Dice Number : " << dice;
	Dsp_DrawMsg(stream.str(),0);
	return dice;
}

//0-maxまでの乱数
inline int Act_Rand(int max)
{
	return rand() % (max+1);
}


/*!
 * @brief 手札からカードの種類をランダムに決めるとかに使える。
 * @param [] 
 */
int Act_RandCategory(vector<int> vec)
{
	vector<int> sum_vec;
	int sum=0;
	for(int i=0;i<vec.size();i++)
		for(int j=0;j<vec[i];j++)
			sum_vec.push_back(i);
	int random_num=Act_Rand(sum_vec.size()-1);
	/*for(int i=0;i<vec.size();i++)
	{
		if(i!=vec.size()-1)
		{
			if(random_num<=sum_vec[i])return i;
		}
		else
		{
			return i;
		}
	}
	*/

	return sum_vec[random_num];
}

int Act_RandCategory(int *cat_vec,int size)
{
	vector<int> sum_vec;
	int sum=0;
	for(int i=0;i<size;i++)
		for(int j=0;j<cat_vec[i];j++)
			sum_vec.push_back(i);
	int random_num=Act_Rand(sum_vec.size()-1);
	return sum_vec[random_num];
}

/*!
 * @brief ボタンリストを全て更新、UpdateButtonLstAllと同じ
 * @param [disp_player] 表示するプレイヤ、NUM_OF_PLAYERS(Default)ならcur_player。
 *                                        default値はサーバしか使わない？2013.04.30
 */
void Act_UpdateButtonLst(int _disp_player)
{
	int disp_player = _disp_player == NUM_OF_PLAYERS ?
		//Act_GetCurrentPlayer():
		cur_player : _disp_player;
	for(int i = 0 ; i < INPUT_END ; i++)
	{
		string str[10];/* 動的確保はめんどくさいので、十分に取れればいい。要素数は各関数内で指定 */
		//if(i == INPUT_PLAYER)
		//{
		//	/* 入力待ちプレイヤ表示領域は常にcur_playerを引数にとる */
		//	(*Act_getStrInfoFuncPtr[i])(cur_player, str);//player0

		//}else
		{
			/* 他のボタンはme_playerをとったりする。 */
			(*Act_getStrInfoFuncPtr[i])(disp_player, str);
		}
		Dsp_DrawButtonLst(i, str, InputRegionIndices[i], -1, false);
	}
}

/*!
 * @brief ボタンリストの表記をすべて更新する
 * @param [disp_player] NUM_OF_PLAYERSのときはcur_player 
 */
//void Act_UpdateButtonLstAll(int disp_player)
//{
//	int tmp_player = disp_player == NUM_OF_PLAYERS ? cur_player : disp_player;
//	for(int cur_input_region = 0 ; cur_input_region < INPUT_END ; cur_input_region++ )
//	{
//	string str[10];
//		(*Act_getStrInfoFuncPtr[cur_input_region])(disp_player,str);
//		Dsp_DrawButtonLst(cur_input_region, str,InputRegionIndices[cur_input_region]);
//	}
//}
/*!
 * @brief 各プレイヤーの情報を取得
 * @param [_me_player] 入力を行ったクライアント:使わない
 * @param [*val] 入力ボタンごとの表示する文字列情報格納先
 */
static void Act_GetInfo_CurrentPlayerStr(int _me_player, string *_val)
{
	stringstream ss;
	int whohasLargestArmy = Crd_GetLargestArmyPlayer();
	int whohasLongestRoad = Map_GetLongestRoadPlayer();
	
	for(int i = 0 ; i < NUM_OF_PLAYERS ; i++)
	{
		ss.str("");
		ss << "pt:" << player_point[i];
		if(i == whohasLargestArmy)ss << " LA";
		if(i == whohasLongestRoad)ss << " LR";
		if(i == cur_player)ss << " Act";
		_val[i] = ss.str();
	}
}
/*!
 * @brief 入力ボタンがどのボタン描画領域か返す
 * @param [but_num] 入力ボタン番号
 */
int Act_GetInputRegion(int but_num)
//static int Act_GetInputRegion(int but_num)
{
	int input_region = INPUT_END;
	for(int i=0;i<INPUT_END;i++)
	{
		if(i < INPUT_END - 1){
			if(but_num < InputRegionIndices[i + 1])
			{
				//if(i==BUT_MAP)continue;
				input_region = i;
				break;
			}		
		}
		else
		{
			if(but_num >= InputRegionIndices[i] && but_num<BUT_END)
			{
				input_region=i;
			}	
		}// if INPUT_END
	}// for i
	return input_region;
}

/*!
 * @brief cur_playerの定義に従ってme_playerが入力待ちかどうか返す。
 * @date 2013.03.03 
 */
bool Act_IsMeWaited(int me_player)
{
	if(me_player == cur_player ||/* 自分のみ待ち */
		( ( cur_player < 0 ) && ((- cur_player) - 1 != me_player) ))/* 3人待ち */
		return true;
	return false;
}
/*!
 * @brief 現在入力待ちのプレイヤ。0-3:一人待ち -1 - -4 :(-cur_player) - 1以外待ち
 * @param [] 
 */
int Act_GetCurrentPlayer(void)
{
	return cur_player;
}
/*!
 * @brief NO_PLAYER:turn to next player, -4 - 3:set
 * @param [] 
 */
void Act_PlayerChange(int player)
{
	if(player==NO_PLAYER)
	{
		cur_player=(cur_player+1)%NUM_OF_PLAYERS;
	}
	else
	{
		cur_player=player;
	}
}
void Act_PlayerChangeInv(void)
{
	cur_player--;
}
void Act_AddPoint(int player,int point)
{
	if(player!=NO_PLAYER)
	player_point[player]+=point;
	/* 勝利 */
	if(player_point[player]>=10)
	{
		stringstream str;
		str<<"Player ";
		str<<player;
		str<< " Win !!";
		Dsp_DrawMsg(str.str(),0);
	}
}
void Act_SubPoint(int player,int point)
{
	if(player!=NO_PLAYER)
	player_point[player]-=point;
}
/*!
 * @brief 受け付ける入力を更新する
 * @param [input_region] フラグを更新するボタンの
 * @param [waitFlg,waitMapFlg] 入力待ちフラグ
 */
void ActUpdateRegionWaitFlg(int input_region)
{
	int st = InputRegionIndices[input_region];
	int en = input_region < INPUT_END - 1 ? InputRegionIndices[input_region + 1] : BUT_END;
	for(int i=st;i<en;i++)
	{
		waitFlg[i]=true;
	}
}
void Act_SetWaitFlg(int input_but, bool flg)
{
	waitFlg[input_but]=flg;

}
void Act_ClearWaitFlg(void)
{
	for(int i=0;i<BUT_END;i++)
	{
		waitFlg[i]=false;
	}
	waitMapFlg=false;
}
void Act_SetWaitMapFlg(bool flg)
{
	waitMapFlg=flg;
}
bool Act_GetWaitMapFlg(void)
{
	return waitMapFlg;
}
bool Act_GetWaitFlg(int i)
{
	return waitFlg[i];
}
int Act_GetDiceNum(int min, int max)
{
	return 2;/* debug */
}
int Act_GetWaitPlayer()
{
	return cur_player;
}

