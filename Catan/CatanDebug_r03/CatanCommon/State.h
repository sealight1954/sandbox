/*!
 * @file State.h
 * @brief ��ԑJ�ڂ𐧌�
 * @author semitsu
 * @date 12.10.23 �J�n
 */
#ifndef __STATE_H
#define __STATE_H
#include "common.h"
enum StateL {
	/* �͂��܂��Ă����Ȃ� */
	STATE_L_ST,
	/* �����z�u����t�F�[�Y */
	STATE_L_IN,
	STATE_L_BF,
	STATE_L_AF
};
enum StateM_AF {
	STATE_ST_WAIT_GAME_START,
	STATE_IN_CHOOSE_HOUSE,
	STATE_IN_CHOOSE_ROAD,
	STATE_AF_IN,
	STATE_AF_BUILD,
#if AF_BUILD_FUNC
	STATE_AF_BUILD_HOUSE,
	STATE_AF_BUILD_ROAD,
	STATE_AF_BUILD_FACTORY,
#endif
	//STATE_AF_GET_DEV_CARD,/* ��Ԃ��Ȃ� */
	STATE_AF_CHANGE_2_1,
	STATE_AF_CHANGE_3_1,
	STATE_AF_CHANGE_4_1,
	STATE_AF_USE_CRD,
	STATE_AF_USE_CRD_MONOPOLY,
	//STATE_AF_USE_CRD_HARVEST1,
	//STATE_AF_USE_CRD_HARVEST2,
	STATE_AF_USE_CRD_HARVEST,
	//STATE_AF_USE_CRD_ROAD1,
	//STATE_AF_USE_CRD_ROAD2,
	STATE_AF_USE_CRD_ROAD,
	STATE_AF_USE_CRD_SOLDIER,
	/* �J�[�h��I�� */
	STATE_AF_CHANGE_1_1_OUT_CRD,
	STATE_AF_CHANGE_1_1_IN_CRD,
	/* �v���C�����m�̌����v�� */
	STATE_AF_CHANGE_1_1_REQUEST,
	//STATE_AF_USE_CRD_POINT,/* ��Ԃ��Ȃ� */
	//STATE_AF_END
//};
//enum StateM_BF {
	STATE_BF_IN,
	STATE_BF_USE_CRD,
	STATE_BF_USE_CRD_SOLDIER,
	STATE_BF_SEVEN_BURST,
	STATE_BF_SEVEN_MOVE_THIEF,
	//STATE_BF_SEVEN_CHOOSE_PLAYER,
	STATE_BF_END
};
//void St_MoveState(CatanInput input,int &cur_player,bool *waitFlg,bool &waitMapFlg);
//void St_Event(CatanInput input,int &cur_player/*,bool *waitFlg,bool &waitMapFlg*/);
void St_Event(CatanInput input);
void St_DrawMode(void);
void setMode_L(int m);
void setMode_M(int m);
void St_UpdateWaitFlgFromMode(void);
int St_GetModeL(void);
int St_GetModeM(void);

#endif //__STATE_H