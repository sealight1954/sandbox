#include "Act_Client.h"
#include "Disp.h"
#include "State.h"
#include "Act.h"
#include "Net_Client.h"
/* Client�̓��͏�� */
static bool prevInput[BUT_END]={0};  /*!< �O��C�x���g���̓��͏�ԁi����or�Ȃ��j */
static int prevInputInt=BUT_END;
static int me_player;
/* static�Ȋ֐���` */
static void ActC_OnMouse( int event, int _x, int _y, int flag, void* );

//void IN_InitActC(void)
//{
//	//for(int i=0;i<INPUT_END;i++)
//	//{
//	//	string str[10];/* ���I�m�ۂ͂߂�ǂ������̂ŁA�\���Ɏ���΂����B�v�f���͊e�֐����Ŏw�� */
//	//	(*Act_getStrInfoFuncPtr[i])(0,str);//player0
//	//	Dsp_DrawButtonLst(i,str,InputRegionIndices[i],-1,true);
//	//}
//	//St_UpdateWaitFlgFromMode();
//	//Dsp_UpdateMainWindow();
//}
/*!
 * @brief ��ʓ��͂̓ǂݍ���(MAP�̓��͐؂藣������)
 * @param [x,y] �N���b�N���̃}�E�X�|�C���^�̍��W
 * @retval MAP���͑҂��̂Ƃ��͍ŋߖT��CP,CS,CH��Ԃ��B
 *         ���͑҂��̃{�^����������Ă���Ƃ���MAP���͂ɗD�悵�ă{�^���̔ԍ���Ԃ��B
 *         �ǂ���ł��Ȃ�����BUT_END��Ԃ��B
 */
static
	CatanInput ActC_inputButton(int _x,int _y)
{
	
	CatanInput input={
		me_player,TYPE_OBJ_END,-1, BUT_END,false
	};
	int cur_player=Act_GetWaitPlayer();
	bool waitMapFlg=Act_GetWaitMapFlg();
	bool isWaitingForMe = Act_IsMeWaited(input.player);
	/* �}�b�v���͑҂��@���@�����҂��������͑S���҂� 2013.02.17 */
	/* �}�b�v���͑҂��@���@�����҂���������3�l�҂� 2013.03.03 */
	if(isWaitingForMe && waitMapFlg)
		//(me_player==cur_player||/* �����̂ݑ҂� */
		//( ( cur_player < 0 ) && (- cur_player != me_player) )))/* 3�l�҂� */
	{
		input.obj = Dsp_GetNNObj( _x, _y);
		input.isInput=true;
		//input.but_num=BUT_MAP;//BUT_END�̂܂�
	}
	/* �{�^���̓��͂�����Ƃ���MAP���͂ɗD�悵�ă{�^���̔ԍ���Ԃ� */
	/* ��̏����ŗD�悷��悤�ɁA�Ƃ����b 2013.02.17 */
	for(int i = 0; i < BUT_END ; i++)
	{
		//if(i==BUT_MAP)continue;
		bool waitFlg_i=Act_GetWaitFlg(i);
		if(/*waitFlg[i]*/isWaitingForMe && waitFlg_i)
		//(me_player==cur_player||/* �����̂ݑ҂� */
		//( ( cur_player < 0 ) && ((-cur_player) - 1 != me_player) )))/* 3�l�҂� */
		{
			if(isInRect( _x, _y, i))
			{
				//printf("input %d\n",i);
				input.but_num = i;
				input.isInput = true;
				break;/* �������{�^���͓����Ɉ�܂łȂ̂ł���break���� */
			}
		}
	}
	return input;
}
/*!
 * @brief �}�E�X�J�[�\������ɂ��邩�ǂ����ŁA�\����ύX����B
 * @param [input] 
 *        input�́A���͑҂��{�^���̃t�B���^�����O�ς݂̓��́B
 */
static void Act_UpdateButtonLstOnOff(CatanInput input)
{
	/* �O��Ɠ��͂��ω� */
	static int prev_input_region=INPUT_END;
	if(prevInputInt != input.but_num)
	{
		/* ���͂��ꂽ�̈�����߂� */
		int cur_input_region = Act_GetInputRegion(input.but_num);
		string str[10];/* ���I�m�ۂ͂߂�ǂ������̂ŁA�\���Ɏ���΂����B�v�f���͊e�֐����Ŏw�� */
		if(cur_input_region != INPUT_END)
		{
			/* ���ݓ��͗̈�X�V */
			int onMouseIdx = input.but_num - InputRegionIndices[cur_input_region];
			(*Act_getStrInfoFuncPtr[cur_input_region])(me_player, str);
			Dsp_DrawButtonLst(cur_input_region, str, InputRegionIndices[cur_input_region], onMouseIdx);/* ���� */
		}

		if(prev_input_region != INPUT_END && prev_input_region != cur_input_region)
		{/* �O����͂��肩�A���݂ƈႤ�̈� */

			/* �ߋ��̓��͗̈�X�V */
			(*Act_getStrInfoFuncPtr[prev_input_region])(me_player,str);
			Dsp_DrawButtonLst(prev_input_region,str,InputRegionIndices[prev_input_region],-1);/* ���� */
		}
		prev_input_region=cur_input_region;
		prevInputInt=input.but_num;
	}
	Dsp_UpdateMainWindow();
}


/*!
 * @brief �}�E�X�ɂ����͐���
 * @param [event] �}�E�X�C�x���g( EVENT_MOUSEMOVE,EVENT_LBUTTONDOWN)
 * @param [x,y] �}�E�X�|�C���^�̍��W 
 * @param [flag] �{�^���̏C������p
 */
static void ActC_OnMouse( int event, int _x, int _y, int flag, void* )
{
	std::string desc;
	CatanInput input;
	Dsp_MapObj objId; /* debug */
	//BUT_NUM input_but=BUT_END;
	//for(int i=0;i<BUT_END;i++)waitFlg[i]=true; /* debug */
	//setMode_M(STATE_AF_USE_CRD_MONOPOLY);
	//setMode_L(STATE_L_AF);
	//waitFlg[BUT_MAP]=false;
	// �}�E�X�C�x���g���擾
	switch(event) {
	case EVENT_MOUSEMOVE:
		desc += "MOUSE_MOVE";
		objId=Dsp_GetNNObj( _x, _y);/* ActC_inputButton�̂Ȃ��łł��āE�E���� */
		input = ActC_inputButton( _x, _y);
		Act_UpdateButtonLstOnOff(input);
	#if DEBUG_COUT_ACT_ON_MOUSE

		printf("%d,%d  \n",objId.typeObj,objId.id);
#endif
		break;
	case cv::EVENT_LBUTTONDOWN:
		desc += "LBUTTON_DOWN";
		//objId=Dsp_GetNNObj(x,y);

		input=ActC_inputButton( _x, _y);

		/* �N���C�A���g�́A������send����B���̌�recieve�����p�P�b�g����������St_Event�����s */
		//St_Event(input/*,waitFlg,waitMapFlg*/);
		if(input.isInput)
		{
			/* ���M���̃��b�Z�[�W */
			NetC_SendInputPacket(input);
		}
		/* ������ւ�A��M������ */
		//Act_UpdateButtonLst();
		//St_DrawMode();
		//Dsp_UpdateMainWindow();
		//printf("%d,%d  \n",objId.typeObj,objId.id);
#if DEBUG_COUT_ACT_ON_MOUSE
		if(input.obj.typeObj!=TYPE_OBJ_END){
			printf("input %d,%d MAP\n",input.obj.typeObj,input.obj.id);
		}else{
			printf("input %d\n",input.but_num);
		}
#endif
		break;
	case cv::EVENT_RBUTTONDOWN:
		desc += "RBUTTON_DOWN";
		break;
	case cv::EVENT_MBUTTONDOWN:
		desc += "MBUTTON_DOWN";
		break;
	case cv::EVENT_LBUTTONUP:
		desc += "LBUTTON_UP";
		break;
	case cv::EVENT_RBUTTONUP:
		desc += "RBUTTON_UP";
		break;
	case cv::EVENT_MBUTTONUP:
		desc += "MBUTTON_UP";
		break;
	case cv::EVENT_LBUTTONDBLCLK:
		desc += "LBUTTON_DBLCLK";
		break;
	case cv::EVENT_RBUTTONDBLCLK:
		desc += "RBUTTON_DBLCLK";
		break;
	case cv::EVENT_MBUTTONDBLCLK:
		desc += "MBUTTON_DBLCLK";
		break;
	}

	// �}�E�X�{�^���C�y�яC���L�[���擾
	if(flag & cv::EVENT_FLAG_LBUTTON)
		desc += " + LBUTTON";
	if(flag & cv::EVENT_FLAG_RBUTTON)
		desc += " + RBUTTON";
	if(flag & cv::EVENT_FLAG_MBUTTON)
		desc += " + MBUTTON";
	if(flag & cv::EVENT_FLAG_CTRLKEY)
		desc += " + CTRL";
	if(flag & cv::EVENT_FLAG_SHIFTKEY)
		desc += " + SHIFT";
	if(flag & cv::EVENT_FLAG_ALTKEY)
		desc += " + ALT";
#if DEBUG_COUT_ACT_ON_MOUSE
	std::cout << desc << " (" << _x << ", " << _y << ")" << std::endl;
#endif
}

/*!
 * @brief 
 * @param [] 
 */
int ActC_GetMePlayer()
{
	return me_player;
}

void IN_InitAct_Client(void)
{
		//�g�p����|�[�g��I��
	//int ch=EOF;
	bool isFinishInputPlayer=false;
	while(!isFinishInputPlayer)
	{
		cout<<"enter player number(0-" << NUM_OF_PLAYERS - 1 << ")" << endl;
		me_player=getchar();
		if(me_player >= '0' && me_player < '0' + NUM_OF_PLAYERS)
		{
			me_player = me_player - '0';
			isFinishInputPlayer = true;
		}
	}
	//int tmp_player=ActC_G
	/* �e���͗̈�̕\���X�V�BAct_GetInfo_CurrentPlayerStr�̂݁A�S���ŋ��ʂ̏���\������̂����� */
	for(int i=0;i<INPUT_END;i++)
	{
		string str[10];/* ���I�m�ۂ͂߂�ǂ������̂ŁA�\���Ɏ���΂����B�v�f���͊e�֐����Ŏw�� */
		
		(*Act_getStrInfoFuncPtr[i])(me_player, str);
		Dsp_DrawButtonLst(i, str, InputRegionIndices[i], -1 , true);
	}
	St_UpdateWaitFlgFromMode();
	Dsp_UpdateMainWindow();
	cv::setMouseCallback(MAIN_WINDOW_NAME, ActC_OnMouse, 0);
}
