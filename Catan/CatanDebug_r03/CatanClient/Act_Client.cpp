#include "Act_Client.h"
#include "Disp.h"
#include "State.h"
#include "Act.h"
#include "Net_Client.h"
/* Clientの入力情報 */
static bool prevInput[BUT_END]={0};  /*!< 前回イベント時の入力状態（押下orなし） */
static int prevInputInt=BUT_END;
static int me_player;
/* staticな関数定義 */
static void ActC_OnMouse( int event, int _x, int _y, int flag, void* );

//void IN_InitActC(void)
//{
//	//for(int i=0;i<INPUT_END;i++)
//	//{
//	//	string str[10];/* 動的確保はめんどくさいので、十分に取れればいい。要素数は各関数内で指定 */
//	//	(*Act_getStrInfoFuncPtr[i])(0,str);//player0
//	//	Dsp_DrawButtonLst(i,str,InputRegionIndices[i],-1,true);
//	//}
//	//St_UpdateWaitFlgFromMode();
//	//Dsp_UpdateMainWindow();
//}
/*!
 * @brief 画面入力の読み込み(MAPの入力切り離した版)
 * @param [x,y] クリック時のマウスポインタの座標
 * @retval MAP入力待ちのときは最近傍のCP,CS,CHを返す。
 *         入力待ちのボタンが押されているときはMAP入力に優先してボタンの番号を返す。
 *         どちらでもない時はBUT_ENDを返す。
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
	/* マップ入力待ち　かつ　自分待ちもしくは全員待ち 2013.02.17 */
	/* マップ入力待ち　かつ　自分待ちもしくは3人待ち 2013.03.03 */
	if(isWaitingForMe && waitMapFlg)
		//(me_player==cur_player||/* 自分のみ待ち */
		//( ( cur_player < 0 ) && (- cur_player != me_player) )))/* 3人待ち */
	{
		input.obj = Dsp_GetNNObj( _x, _y);
		input.isInput=true;
		//input.but_num=BUT_MAP;//BUT_ENDのまま
	}
	/* ボタンの入力があるときはMAP入力に優先してボタンの番号を返す */
	/* 後の処理で優先するように、という話 2013.02.17 */
	for(int i = 0; i < BUT_END ; i++)
	{
		//if(i==BUT_MAP)continue;
		bool waitFlg_i=Act_GetWaitFlg(i);
		if(/*waitFlg[i]*/isWaitingForMe && waitFlg_i)
		//(me_player==cur_player||/* 自分のみ待ち */
		//( ( cur_player < 0 ) && ((-cur_player) - 1 != me_player) )))/* 3人待ち */
		{
			if(isInRect( _x, _y, i))
			{
				//printf("input %d\n",i);
				input.but_num = i;
				input.isInput = true;
				break;/* 押されるボタンは同時に一つまでなのですぐbreakする */
			}
		}
	}
	return input;
}
/*!
 * @brief マウスカーソルが上にあるかどうかで、表示を変更する。
 * @param [input] 
 *        inputは、入力待ちボタンのフィルタリング済みの入力。
 */
static void Act_UpdateButtonLstOnOff(CatanInput input)
{
	/* 前回と入力が変化 */
	static int prev_input_region=INPUT_END;
	if(prevInputInt != input.but_num)
	{
		/* 入力された領域を求める */
		int cur_input_region = Act_GetInputRegion(input.but_num);
		string str[10];/* 動的確保はめんどくさいので、十分に取れればいい。要素数は各関数内で指定 */
		if(cur_input_region != INPUT_END)
		{
			/* 現在入力領域更新 */
			int onMouseIdx = input.but_num - InputRegionIndices[cur_input_region];
			(*Act_getStrInfoFuncPtr[cur_input_region])(me_player, str);
			Dsp_DrawButtonLst(cur_input_region, str, InputRegionIndices[cur_input_region], onMouseIdx);/* 未完 */
		}

		if(prev_input_region != INPUT_END && prev_input_region != cur_input_region)
		{/* 前回入力ありかつ、現在と違う領域 */

			/* 過去の入力領域更新 */
			(*Act_getStrInfoFuncPtr[prev_input_region])(me_player,str);
			Dsp_DrawButtonLst(prev_input_region,str,InputRegionIndices[prev_input_region],-1);/* 未完 */
		}
		prev_input_region=cur_input_region;
		prevInputInt=input.but_num;
	}
	Dsp_UpdateMainWindow();
}


/*!
 * @brief マウスによる入力制御
 * @param [event] マウスイベント( EVENT_MOUSEMOVE,EVENT_LBUTTONDOWN)
 * @param [x,y] マウスポインタの座標 
 * @param [flag] ボタンの修飾制御用
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
	// マウスイベントを取得
	switch(event) {
	case EVENT_MOUSEMOVE:
		desc += "MOUSE_MOVE";
		objId=Dsp_GetNNObj( _x, _y);/* ActC_inputButtonのなかでできて・・いる */
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

		/* クライアントは、ここでsendする。その後recieveしたパケットを処理してSt_Eventを実行 */
		//St_Event(input/*,waitFlg,waitMapFlg*/);
		if(input.isInput)
		{
			/* 送信中のメッセージ */
			NetC_SendInputPacket(input);
		}
		/* ここらへん、受信処理に */
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

	// マウスボタン，及び修飾キーを取得
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
		//使用するポートを選択
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
	/* 各入力領域の表示更新。Act_GetInfo_CurrentPlayerStrのみ、全員で共通の情報を表示するのが特殊 */
	for(int i=0;i<INPUT_END;i++)
	{
		string str[10];/* 動的確保はめんどくさいので、十分に取れればいい。要素数は各関数内で指定 */
		
		(*Act_getStrInfoFuncPtr[i])(me_player, str);
		Dsp_DrawButtonLst(i, str, InputRegionIndices[i], -1 , true);
	}
	St_UpdateWaitFlgFromMode();
	Dsp_UpdateMainWindow();
	cv::setMouseCallback(MAIN_WINDOW_NAME, ActC_OnMouse, 0);
}
