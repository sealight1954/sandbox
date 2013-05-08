/*!
 * @file common.h
 * @brief �J�^���S�̂ŋ��L����萔�A�}�N����`�A�ϐ���錾
 * @author semitsu
 * @date 12.09.30 �J�n
		 12.10.01 OPENCV�n�̒�`��ǋL
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
/* �R�[�f�B���O�p�}�N����`�Ƃ� */
typedef unsigned char Byte;
#define MAX_BUF_SIZE (256)

/* �Q�[���p�ϐ� */
#define NUM_OF_PLAYERS (4)
#define NO_PLAYER (4)
#define NO_OBJ_ID (-1)
#define NO_SIDE (-1)
/* --�����̎�� LUMBER:�� BRICK:�y WOOL:�r GRAIN:�� ORE:�S -- */
enum TypeResource {TYPE_RSC_LUMBER,TYPE_RSC_BRICK,TYPE_RSC_WOOL,TYPE_RSC_GRAIN,TYPE_RSC_ORE,TYPE_RSC_END};
/* --�I�u�W�F�N�g�̎��(�_�E�ӁE�Z�p�`) --*/
enum TypeObject {TYPE_OBJ_POINT,TYPE_OBJ_SIDE,TYPE_OBJ_HEXAGON,TYPE_OBJ_END};
/* --���z�����̎��(�ƁE�H��E��)-- */
enum TypeDevelopment {/*TYPE_DEV_NONE,*/TYPE_DEV_HOUSE,TYPE_DEV_ROAD,TYPE_DEV_FACTORY,TYPE_DEV_END};
/* --�J�[�h�̎��--(�Ɛ�E���n�E�X���E���m�E���_) */
enum TypeDevCard {TYPE_CRD_MONOPOLY,TYPE_CRD_HARVEST,TYPE_CRD_BUILDING_ROAD,TYPE_CRD_SOLDIER,TYPE_CRD_POINT,TYPE_CRD_END};
/* --�`�̎��--((2-1)LUMBER:�� BRICK:�y WOOL:�r GRAIN:�� ORE:�S 3-1) */
enum TypeHarbor {TYPE_HRB_LUMBER21,TYPE_HRB_BRICK21,TYPE_HRB_WOOL21,TYPE_HRB_GRAIN21,TYPE_HRB_ORE21,TYPE_HRB_31,TYPE_HRB_END};
/* DevelopmentCard��� */
#define NUM_OF_CRD_MONOPOLY (2)
#define NUM_OF_CRD_HARVEST (2)
#define NUM_OF_CRD_BUILDING_ROAD (2)
#define NUM_OF_CRD_SOLDIER (14)
#define NUM_OF_CRD_POINT (5)

#define DSP_CALL_FROM_ACT_DEBUG (1)
/* �C���^�t�F�[�X��� */
/* �v���C����Ă̌����{�^���ǉ�2013.02.28 */
#define NUM_OF_BUT_EXCHANGE (5)
#define NUM_OF_BUT_ACTION (5)
#define NUM_OF_BUT_YESNO (2)

/* �}�b�v��� */
/* �I�u�W�F�N�g�̎�ނ�ID */
struct Dsp_MapObj{
	int typeObj;  /* �_���A�ӂ��A�Z�p�`���̏�� TypeObject�ɋL��*/
	int id;              /* �I�u�W�F�N�g��ID*/
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
/* �v�Z�p�}�N�� */
#define POW(x) ((x)*(x))
#define ABS(x) ((x)<0 ? -(x) : (x))

#define DEBUG_COUT_MAP_CONTENT (1)
#define DEBUG_GAME (1)
#define DEBUG_DRAW_CH_CONTENT (0)
#endif 