/*!
 * @file Disp.h
 * @brief 画面表示・マウスクリックの領域判定制御
 * @author semitsu
 * @date 12.10.14 開始
 */
#ifndef __DISP_H
#define __DISP_H
#include "common.h"
//#include "Map.h"

/* 入力関係 */
/* それぞれの行の中の順番はTYPEと一致しなければならない */
enum BUT_NUM {
#if !(DSP_CALL_FROM_ACT_DEBUG)
	BUT_MAP,                                                                                               /* MAP(実際にはボタンはなし） */
#endif
	BUT_PLAYER_0,BUT_PLAYER_1,BUT_PLAYER_2,BUT_PLAYER_3,                                                             /* プレイヤ */
	BUT_MASTER_LUMBER,BUT_MASTER_BRICK,BUT_MASTER_WOOL,BUT_MASTER_GRAIN,BUT_MASTER_ORE,                              /* 山札の資源 */
	BUT_PLAYER_LUMBER,BUT_PLAYER_BRICK,BUT_PLAYER_WOOL,BUT_PLAYER_GRAIN,BUT_PLAYER_ORE,                              /* プレイヤの資源 */
	BUT_PLAYER_HOUSE,BUT_PLAYER_ROAD,BUT_PLAYER_FACTORY,                                                             /* プレイヤの建築資源 */
	BUT_CRD_MONOPOLY_OPEN,BUT_CRD_HARVEST_OPEN,BUT_CRD_ROAD_OPEN,BUT_CRD_SOLDIER_OPEN,BUT_CRD_POINT_OPEN,            /* カード資源オープン */
	BUT_CRD_MONOPOLY_CLOSED,BUT_CRD_HARVEST_CLOSED,BUT_CRD_ROAD_CLOSED,BUT_CRD_SOLDIER_CLOSED,BUT_CRD_POINT_CLOSED,  /* カード資源クローズド */
	BUT_GET_DEV_CARD,BUT_CHANGE_2_1,BUT_CHANGE_3_1,BUT_CHANGE_4_1,BUT_CHANGE_1_1,                                                   /* 交換ボタン */
	BUT_ACT_DEVELOP,BUT_ACT_USE_CARD,BUT_ACT_DICE,BUT_ACT_TURN_END,BUT_ACT_UNDO,                                                   /* アクション */
	BUT_YESNO_YES,BUT_YESNO_NO,
	BUT_END
};/* プレイヤのDEV_CARD */
enum INPUT_TYPE {
#if !(DSP_CALL_FROM_ACT_DEBUG)
	INPUT_MAP,
#endif
	INPUT_PLAYER,
	INPUT_RSC_MASTER,
	INPUT_RSC_PLAYER,
	INPUT_DEV_RSC,
	INPUT_CRD_OPEN,
	INPUT_CRD_CLOSED,
	INPUT_EXCHANGE,
	INPUT_ACTION,
	INPUT_YESNO,
	INPUT_END
};


/* ウィンドウ系 */
#define MAIN_WINDOW_NAME ("CATAN_MAIN")
/* 描画に使う情報 */
#define PIX_PER_LINE_HOR (25)
#define PIX_PER_LINE_VER ((PIX_PER_LINE_HOR)*1.7320508)
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define LINE_DEFAULT_WIDTH (2)
#define LINE_ROAD_WIDTH (4)
#define WIDTH_HOUSE (20)
#define HEIGHT_HOUSE (20)
/* テキスト描画 */
#define RSC_FONT (FONT_HERSHEY_COMPLEX)
#define DEV_FONT (FONT_HERSHEY_DUPLEX)
#define FONT_SCALE_CH_CONTENT (0.5)
#define FONT_SCALE_DEV (0.4)
#define THICKNESS_DEV (1)
#define THICKNESS_CH_CONTENT (1)
#define FONT_SCALE (0.65)
#define INFO_THICKNESS (1)
#define COLOR_WHITE (Scalar(255,255,255))
#define COLOR_BLACK (Scalar(0,0,0))
#define COLOR_GRAY_2 (Scalar(50,50,50))
#define COLOR_GRAY_3 (Scalar(80,80,80))
#define COLOR_GREEN (Scalar(0,200,0))
#define COLOR_YELLOW (Scalar(0,255,255))
#define COLOR_BROWN (Scalar(0,100,100))
#define COLOR_BLUE (Scalar(200,0,0))
#define COLOR_CYAN (Scalar(255,255,0))
#define COLOR_RED (Scalar(0,0,200))
/* 地図内 計算マクロ */
#define EUC_DIST(x1,y1,x2,y2) (POW((x1)-(x2)) + POW((y1)-(y2)))
#define DEF_CP(x,y) (Point((x)*PIX_PER_LINE_HOR,((y)-1)*PIX_PER_LINE_VER))
/* 表示位置 XXは画面全体における位置、他は領域の左上を原点とした相対位置 */
/* プレイヤー */
#define XX_PLAYER (XX)
#define YY_PLAYER (60*0)
#define  W_PLAYER (500)
#define  H_PLAYER (60)
#define X_PLAYER_BUT (120)
#define Y_PLAYER_BUT (10)
#define NUM_OF_TXT_PER_LINE (8)
/* 資源(山札) */
#define XX_RSC_IN_MASTER XX
#define YY_RSC_IN_MASTER (60*1)
//#define X_RSC_MASTER_BUT 150
//#define Y_RSC_MASTER_BUT 10
#define W_RSC_IN_MASTER 470
#define H_RSC_IN_MASTER 60
/* 資源 */
#define XX 600
#define XX_RSC_IN_HAND XX
#define YY_RSC_IN_HAND (60*2)
#define W_RSC_IN_HAND 470
#define H_RSC_IN_HAND 60
#define X_RSC_BUT 150
#define Y_RSC_BUT 10
/* 建築資源 */
#define XX_DEV_IN_HAND XX
#define YY_DEV_IN_HAND (60*3)
#define X_DEV_BUT 150
#define Y_DEV_BUT 10
#define W_DEV_IN_HAND 450
#define H_DEV_IN_HAND 60
/* DevCard（OPEN） */
#define XX_CRD_OPEN XX
#define YY_CRD_OPEN (60*4)
#define W_CRD_OPEN 470
#define H_CRD_OPEN 60
#define X_CRD_BUT 150
#define Y_CRD_BUT 10
/* DevCard（CLOSED） */
#define XX_CRD_CLOSE XX
#define YY_CRD_CLOSE (60*5)
#define W_CRD_CLOSE 470
#define H_CRD_CLOSE 60
/* 交換ボタン */
#define XX_EX_OPEN XX
#define YY_EX_OPEN (60*6)
#define W_EX_OPEN 450
#define H_EX_OPEN 60
#define X_EX_BUT 150
#define Y_EX_BUT 10
/* アクションボタン */
#define XX_ACT_OPEN XX
#define YY_ACT_OPEN (60*7)
#define W_ACT_OPEN 470
#define H_ACT_OPEN 60
#define X_ACT_BUT 150
#define Y_ACT_BUT 10
/* メッセージボックス */
//#define XX_MSG (XX)
//#define YY_MSG (60*8)
#define XX_MSG_0 (XX)
#define YY_MSG_0 (60*8)
#define XX_MSG_1 (XX)
#define YY_MSG_1 (60*9)
#define XX_MSG_2 (XX)
#define YY_MSG_2 (60*9+30)

#define  W_MSG (450)
#define  H_MSG_0 (60)
#define  H_MSG_1 (30)
#define  X_MSG_TXT (0)
#define  Y_MSG_TXT_0 (30)
#define  Y_MSG_TXT_1 (15)
#define FONT_SCALE_MSG (1.2)
#define FONT_SCALE_MSG_1 (0.6)
/* Yes No Button */
#define XX_YESNO 400
#define YY_YESNO (60*9)
#define W_YESNO (200)
#define H_YESNO (60)
#define X_YESNO_TXT (10)
#define Y_YESNO_TXT (20)
#define X_BUT_YESNO (70)
#define Y_BUT_YESNO (10)

/* 共通 */
#define X_TITLE 10
#define Y_TITLE 20
#define W_BUT_SMALL 60
#define H_BUT_SMALL 40
#define W_BUT_MEDIUM 90
#define H_BUT_MEDIUM 40

/* 描画色情報 */
/* --資源の種類 LUMBER:木 BRICK:土 WOOL:羊 GRAIN:麦 ORE:鉄 -- */
static const Scalar Rsc_Color[]={COLOR_GREEN,COLOR_BROWN,COLOR_YELLOW,Scalar(100,200,255),COLOR_BLUE};
static const Scalar Player_Color[]={COLOR_RED,Scalar(0,200,0),Scalar(100,0,0),COLOR_CYAN};
static const Scalar Dev_Color[]={COLOR_WHITE,COLOR_WHITE,COLOR_WHITE};/* 白 */
static const Scalar Ex_Color[]={Scalar(0,0,200),Scalar(0,200,0),Scalar(100,0,0),Scalar(100,200,255)};
static const Scalar Act_Color[]={Scalar(0,0,200),Scalar(0,200,0),Scalar(100,0,0),Scalar(100,200,255),Scalar(100,200,255)};
static const Scalar YesNo_Color[]={COLOR_WHITE,COLOR_WHITE};

Dsp_MapObj Dsp_GetNNObj(int x,int y);
void IN_InitSetDispMap();
void Dsp_DrawCHContent(const int ch_idx,int type,int diceNum,bool isThiefThere);
//void Dsp_DrawCHContent(const int ch_idx,int type,int diceNum);
void Dsp_DrawRoad(const int cs_idx,bool isRoadThere,int player,int type_hrb);
void Dsp_DrawHouseFactory(int cp_idx,int player,int type_dev);
void Dsp_DrawMsg(string message,int window_num=1);
void Dsp_ClearMap(void);

#if DSP_CALL_FROM_ACT_DEBUG
bool Dsp_DrawButtonLst(int regionIdx,string *str_bot,int but_idx,int onMouseIdx=-1,bool isFirst=false);
#else
void Dsp_DrawMap(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawPlayer(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawRscInHand(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawDevRscInHand(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawRscInMaster(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawDevCardOpen(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawDevCardClose(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawExchangeBut(/*Mat &img,*/string *val,int onMouseIdx=-1);
void Dsp_DrawActionBut(/*Mat &img,*/string *val,int onMouseIdx=-1);
#endif
bool isInRect(int x,int y,Rect rect);
bool isInRect(int x,int y,int but_idx);
void Dsp_UpdateMainWindow_Debug(void);
void Dsp_UpdateMainWindow(void);
void Dsp_GetInfo_ExchangeBut(int player,string *val);
void Dsp_GetInfo_ActionBut(int player,string *val);
void Dsp_GetInfo_YesNoBut(int player,string *val);
#endif //__DISP_H