/*!
 * @file common.h
 * @brief カタン全体で共有する定数、マクロ定義、変数を宣言
 * @author semitsu
 * @date 12.09.30 開始
		 12.10.01 OPENCV系の定義を追記
 */
#ifndef __COMMON_H
#define __COMMON_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)
using namespace cv;
using namespace std;
#if OPENCV_VERSION_CODE < OPENCV_VERSION(2,3,1)
namespace cv
{
	enum {
		EVENT_MOUSEMOVE      =CV_EVENT_MOUSEMOVE,
		EVENT_LBUTTONDOWN    =CV_EVENT_LBUTTONDOWN,
		EVENT_RBUTTONDOWN    =CV_EVENT_RBUTTONDOWN,
		EVENT_MBUTTONDOWN    =CV_EVENT_MBUTTONDOWN,
		EVENT_LBUTTONUP      =CV_EVENT_LBUTTONUP,
		EVENT_RBUTTONUP      =CV_EVENT_RBUTTONUP,
		EVENT_MBUTTONUP      =CV_EVENT_MBUTTONUP,
		EVENT_LBUTTONDBLCLK  =CV_EVENT_LBUTTONDBLCLK,
		EVENT_RBUTTONDBLCLK  =CV_EVENT_RBUTTONDBLCLK,
		EVENT_MBUTTONDBLCLK  =CV_EVENT_MBUTTONDBLCLK
	};
	enum {
		EVENT_FLAG_LBUTTON   =CV_EVENT_FLAG_LBUTTON,
		EVENT_FLAG_RBUTTON   =CV_EVENT_FLAG_RBUTTON,
		EVENT_FLAG_MBUTTON   =CV_EVENT_FLAG_MBUTTON,
		EVENT_FLAG_CTRLKEY   =CV_EVENT_FLAG_CTRLKEY,
		EVENT_FLAG_SHIFTKEY  =CV_EVENT_FLAG_SHIFTKEY,
		EVENT_FLAG_ALTKEY    =CV_EVENT_FLAG_ALTKEY
	};
}
#endif
/* コーディング用マクロ定義とか */
typedef unsigned char Byte;
#define MAX_BUF_SIZE (256)

/* ゲーム用変数 */
#define NUM_OF_PLAYERS (4)
#define NO_PLAYER (4)
#define NO_OBJ_ID (-1)
#define NO_SIDE (-1)
/* --資源の種類 LUMBER:木 BRICK:土 WOOL:羊 GRAIN:麦 ORE:鉄 -- */
enum TypeResource {TYPE_RSC_LUMBER,TYPE_RSC_BRICK,TYPE_RSC_WOOL,TYPE_RSC_GRAIN,TYPE_RSC_ORE,TYPE_RSC_END};
/* --オブジェクトの種類(点・辺・六角形) --*/
enum TypeObject {TYPE_OBJ_POINT,TYPE_OBJ_SIDE,TYPE_OBJ_HEXAGON,TYPE_OBJ_END};
/* --建築資源の種類(家・工場・道)-- */
enum TypeDevelopment {/*TYPE_DEV_NONE,*/TYPE_DEV_HOUSE,TYPE_DEV_ROAD,TYPE_DEV_FACTORY,TYPE_DEV_END};
/* --カードの種類--(独占・収穫・街道・兵士・得点) */
enum TypeDevCard {TYPE_CRD_MONOPOLY,TYPE_CRD_HARVEST,TYPE_CRD_BUILDING_ROAD,TYPE_CRD_SOLDIER,TYPE_CRD_POINT,TYPE_CRD_END};
/* --港の種類--((2-1)LUMBER:木 BRICK:土 WOOL:羊 GRAIN:麦 ORE:鉄 3-1) */
enum TypeHarbor {TYPE_HRB_LUMBER21,TYPE_HRB_BRICK21,TYPE_HRB_WOOL21,TYPE_HRB_GRAIN21,TYPE_HRB_ORE21,TYPE_HRB_31,TYPE_HRB_END};
/* DevelopmentCard情報 */
#define NUM_OF_CRD_MONOPOLY (2)
#define NUM_OF_CRD_HARVEST (2)
#define NUM_OF_CRD_BUILDING_ROAD (2)
#define NUM_OF_CRD_SOLDIER (14)
#define NUM_OF_CRD_POINT (5)

#define DSP_CALL_FROM_ACT_DEBUG (1)
/* インタフェース情報 */
/* プレイヤ提案の交換ボタン追加2013.02.28 */
#define NUM_OF_BUT_EXCHANGE (5)
#define NUM_OF_BUT_ACTION (5)
#define NUM_OF_BUT_YESNO (2)

/* マップ情報 */
/* オブジェクトの種類とID */
struct Dsp_MapObj{
	int typeObj;  /* 点か、辺か、六角形かの情報 TypeObjectに記載*/
	int id;              /* オブジェクトのID*/
};
struct CatanInput{
	int player;
	Dsp_MapObj obj;
	int but_num;
	bool isInput; 
};
#define NUM_OF_POINTS (54)
#define NUM_OF_SIDES (72)
#define NUM_OF_HEXAGONS (19)
#define NUM_OF_HARBOR (9)
#define DESERT_CH (18)
#define LARGEST_ARMY_MIN (2)
#define LONGEST_ROAD_MIN (4)
/* 計算用マクロ */
#define POW(x) ((x)*(x))
#define ABS(x) ((x)<0 ? -(x) : (x))

#define DEBUG_COUT_MAP_CONTENT (1)
#define DEBUG_GAME (1)
#define DEBUG_DRAW_CH_CONTENT (0)
#endif 