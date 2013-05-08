/*!
* @file Disp.cpp
* @brief ��ʕ\���E�}�E�X�N���b�N�̗̈攻�萧��
* @author semitsu
* @date 12.10.14 �J�n
* @date 12.10.20 Mat�̎��Ԃ�Disp.cpp�Ɉړ�
*/

#include "Disp.h"
#include "Map.h"
extern void(*Act_getStrInfoFuncPtr[])(int,string *str);


/*!
@brief �Z�p�`�̓_
*/
static struct DispCatanPoint{
	int id;
	Point pt;
}mcp[NUM_OF_POINTS];
/*!
@brief �Z�p�`�̕�
(row,col)�̂���row�̏��������̂�p1,�����Ȃ�col�̏��������̂�p1
*/
static struct DispCatanSide{
	int id;
	DispCatanPoint *p1;
	DispCatanPoint *p2;
	Point center;
	/* �Q�[���� */
}mcs[NUM_OF_SIDES];
/*! @brief �Z�p�` */
static struct DispCatanHexagon{
	int id;
	/* �ʒu��� */
	Point center;
	/* �Q�[���� */
}mch[NUM_OF_HEXAGONS];
/*!
* @brief ���͗̈�̕`����B��s�ڂ̃e�L�X�g�̂ݕʌ��i����p�ӂ���^�C�~���O�����̗v�f�ƈႤ���߁j
* @param [] 
*/
static struct DrawInfo{
	string title;         /*!< �̈�̃^�C�g�� */
	int x,y,w,h ;         /*!< ��ʂɂ�����ʒu */
	int x_title,y_title;  /*!< �^�C�g���̈ʒu */
	int x_but,y_but;      /*!< �{�^���̕\���J�n�ʒu */
	int w_but,h_but;      /*!< �{�^���̑傫�� 2�s�ڂ�h_but/2����\�� */
	int num_but;          /*!< �{�^���̐� */
	const string *but_top_text;     /*!< �{�^���̕\�L�̔z�� */
	const Scalar *but_color;    /*!< �{�^����text�F�̔z�� */
};

/* --static �֐��錾------------------------------------------------------------------ */
static void Dsp_DrawLine(Mat &img,const int cs_idx);
static long Dsp_GetEucDistance(const Point &pt1, const Point &pt2);
static void Dsp_DrawButton(Mat &img,string str_top,string str_bot,int y_bot,Scalar color,Scalar bg_color=COLOR_GRAY_3);
#if !(DSP_CALL_FROM_ACT_DEBUG)
static bool Dsp_DrawButtonLst(Mat &img,string *but_bot_text,DrawInfo info,/*Rect *RectLst,*/int but_idx,bool isFirst,int onMouseIdx);
#endif
static Mat main_img = cv::Mat::zeros(cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT), CV_8UC3); 
static struct DispCatanPoint;
static struct DispCatanSide;
static struct DispCatanHexagon;

static string Ex_text_bot[]={"","Exchange","Exchange","Exchange","Exchange"};/* �v���C����Ă̌�����ǉ�2013.02.28 */
static string Act_text_bot[]={"","Use","","",""};
static string YesNo_text_bot[]={"",""};
/* �̈攻��p */
static Rect RectLst[BUT_END];

/*!
* @brief �\������}�b�v�p�ɍ��W��ݒ�BCP,CS,CH�̐e�q�֌W��ݒ肵����Ăяo���B
* @param [&img] �\������Mat�摜 
*/
void IN_InitSetDispMap(){
	cv::namedWindow(MAIN_WINDOW_NAME, 1 );

	/*�|�C���^��������*/
	for(int i=0;i<NUM_OF_POINTS;i++)
	{
		mcp[i].id=i;
	}
	for(int i=0;i<NUM_OF_SIDES;i++)
	{
		mcs[i].id=i;

		mcs[i].p1=NULL;
		mcs[i].p2=NULL;
	}
	for(int i=0;i<NUM_OF_HEXAGONS;i++)
	{
		mch[i].id=i;
	}
	/* �_�̏�����(���W���Z�b�g) */
	mcp[0].pt=DEF_CP(11,3);
	mcp[1].pt=DEF_CP(13,3);
	mcp[2].pt=DEF_CP(8,4);
	mcp[3].pt=DEF_CP(10,4);
	mcp[4].pt=DEF_CP(14,4);
	mcp[5].pt=DEF_CP(16,4);
	mcp[6].pt=DEF_CP(5,5);
	mcp[7].pt=DEF_CP(7,5);
	mcp[8].pt=DEF_CP(11,5);
	mcp[9].pt=DEF_CP(13,5);
	mcp[10].pt=DEF_CP(17,5);
	mcp[11].pt=DEF_CP(19,5);
	mcp[12].pt=DEF_CP(4,6);
	mcp[13].pt=DEF_CP(8,6);
	mcp[14].pt=DEF_CP(10,6);
	mcp[15].pt=DEF_CP(14,6);
	mcp[16].pt=DEF_CP(16,6);
	mcp[17].pt=DEF_CP(20,6);
	mcp[18].pt=DEF_CP(5,7);
	mcp[19].pt=DEF_CP(7,7);
	mcp[20].pt=DEF_CP(11,7);
	mcp[21].pt=DEF_CP(13,7);
	mcp[22].pt=DEF_CP(17,7);
	mcp[23].pt=DEF_CP(19,7);
	mcp[24].pt=DEF_CP(4,8);
	mcp[25].pt=DEF_CP(8,8);
	mcp[26].pt=DEF_CP(10,8);
	mcp[27].pt=DEF_CP(14,8);
	mcp[28].pt=DEF_CP(16,8);
	mcp[29].pt=DEF_CP(20,8);
	mcp[30].pt=DEF_CP(5,9);
	mcp[31].pt=DEF_CP(7,9);
	mcp[32].pt=DEF_CP(11,9);
	mcp[33].pt=DEF_CP(13,9);
	mcp[34].pt=DEF_CP(17,9);
	mcp[35].pt=DEF_CP(19,9);
	mcp[36].pt=DEF_CP(4,10);
	mcp[37].pt=DEF_CP(8,10);
	mcp[38].pt=DEF_CP(10,10);
	mcp[39].pt=DEF_CP(14,10);
	mcp[40].pt=DEF_CP(16,10);
	mcp[41].pt=DEF_CP(20,10);
	mcp[42].pt=DEF_CP(5,11);
	mcp[43].pt=DEF_CP(7,11);
	mcp[44].pt=DEF_CP(11,11);
	mcp[45].pt=DEF_CP(13,11);
	mcp[46].pt=DEF_CP(17,11);
	mcp[47].pt=DEF_CP(19,11);
	mcp[48].pt=DEF_CP(8,12);
	mcp[49].pt=DEF_CP(10,12);
	mcp[50].pt=DEF_CP(14,12);
	mcp[51].pt=DEF_CP(16,12);
	mcp[52].pt=DEF_CP(11,13);
	mcp[53].pt=DEF_CP(13,13);

	/* CS�̏d�S�ݒ� */
	for(int i=0;i<NUM_OF_SIDES;i++)
	{
		int tmp1,tmp2;
		IN_GetIndex_ChildCPofCS(i,tmp1,tmp2);
		mcs[i].center.x=(mcp[tmp1].pt.x+mcp[tmp2].pt.x)/2;
		mcs[i].center.y=(mcp[tmp1].pt.y+mcp[tmp2].pt.y)/2;
		mcs[i].p1=&mcp[tmp1];
		mcs[i].p2=&mcp[tmp2];
	}
	/* CH�̏d�S�ݒ� */
	for(int i=0;i<NUM_OF_HEXAGONS;i++)
	{
		int tmp1,tmp2;
		IN_GetIndex_ChildCPofCH(i,tmp1,tmp2);
		mch[i].center.x=(mcp[tmp1].pt.x+mcp[tmp2].pt.x)/2;
		mch[i].center.y=(mcp[tmp1].pt.y+mcp[tmp2].pt.y)/2;
	}
}

#if !(DSP_CALL_FROM_ACT_DEBUG)
/* �_�~�[�֐� */
void Dsp_DrawMap(string *val,int onMouseIdx){

}
void Dsp_DrawPlayer(string *val,int onMouseIdx){
	static bool isFirst=true;
	static DrawInfo info={"RscInMaster",
		XX_PLAYER,
		YY_PLAYER,
		W_PLAYER,
		H_PLAYER-1,
		X_TITLE,Y_TITLE,
		X_PLAYER_BUT,Y_PLAYER_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		NUM_OF_PLAYERS,
		Player_text, /*val,*/
		Player_Color};
	/*if(!isFirst)
	{
	info.but_bot_text=val;
	}*/
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	isFirst=Dsp_DrawButtonLst(tmp_img,val,info,BUT_PLAYER_0,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
	//isFirst=Dsp_DrawButtonLst(tmp_img,info,BUT_PLAYER_0,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
}
void Dsp_DrawRscInMaster(/*Mat &img,*/string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"RscInMaster",
		XX_RSC_IN_MASTER,YY_RSC_IN_MASTER,W_RSC_IN_MASTER,H_RSC_IN_MASTER-1,
		X_TITLE,Y_TITLE,
		X_RSC_BUT,Y_RSC_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_RSC_END,
		Rsc_text, /*val,*/
		Rsc_Color};

	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	//if(!isFirst)
	//{
	//	info.but_bot_text=val;
	//}
	isFirst=Dsp_DrawButtonLst(tmp_img,val,info,BUT_MASTER_LUMBER,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
	//isFirst=Dsp_DrawButtonLst(tmp_img,info,BUT_MASTER_LUMBER,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
}
void Dsp_DrawRscInHand(/*Mat &img,*/string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"ResourceInHand",
		XX_RSC_IN_HAND,YY_RSC_IN_HAND,W_RSC_IN_HAND,H_RSC_IN_HAND-1,
		X_TITLE,Y_TITLE,
		X_RSC_BUT,Y_RSC_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_RSC_END,
		Rsc_text,/* val,*/
		Rsc_Color};
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	//if(!isFirst)
	//{
	//	info.but_bot_text=val;
	//}
	isFirst=Dsp_DrawButtonLst(tmp_img,val,info,BUT_PLAYER_LUMBER,/*RscInHand_RectLst,*/isFirst,onMouseIdx);
	//isFirst=Dsp_DrawButtonLst(imgRscInHand,info,BUT_PLAYER_LUMBER,/*RscInHand_RectLst,*/isFirst,onMouseIdx);
	//imshow(MAIN_WINDOW_NAME,imgRscInHand);
}
void Dsp_DrawDevRscInHand(string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"DevInHand",
		XX_DEV_IN_HAND,YY_DEV_IN_HAND,W_DEV_IN_HAND,H_DEV_IN_HAND-1,
		X_TITLE,Y_TITLE,
		X_DEV_BUT,Y_DEV_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_DEV_END,
		Dev_text,/* val,*/
		Dev_Color};
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	//if(!isFirst)
	//{
	//	info.but_bot_text=val;
	//}
	isFirst=Dsp_DrawButtonLst(tmp_img,val,info,BUT_PLAYER_HOUSE,/*DevRscInHand_RectLst,*/isFirst,onMouseIdx);
	//isFirst=Dsp_DrawButtonLst(tmp_img,info,BUT_PLAYER_HOUSE,/*DevRscInHand_RectLst,*/isFirst,onMouseIdx);
}
void Dsp_DrawDevCardOpen(string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"DevCard Open",
		XX_CRD_OPEN,YY_CRD_OPEN,W_CRD_OPEN,H_CRD_OPEN-1,
		X_TITLE,Y_TITLE,
		X_CRD_BUT,Y_CRD_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_CRD_END,
		Crd_text, /*val,*/
		Rsc_Color};/* �Ȃ񂩍��H */
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	//if(!isFirst)
	//{
	//	info.but_bot_text=val;
	//}
	isFirst=Dsp_DrawButtonLst(tmp_img,val,info,BUT_CRD_MONOPOLY_OPEN,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
	//isFirst=Dsp_DrawButtonLst(tmp_img,info,BUT_CRD_MONOPOLY_OPEN,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
}
void Dsp_DrawDevCardClose(string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"DevCard Close",
		XX_CRD_CLOSE,YY_CRD_CLOSE,W_CRD_CLOSE,H_CRD_CLOSE-1,
		X_TITLE,Y_TITLE,
		X_CRD_BUT,Y_CRD_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_CRD_END,
		Crd_text, /*val,*/
		Rsc_Color};/* �Ȃ񂩍��H */
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	//if(!isFirst)
	//{
	//	info.but_bot_text=val;
	//}
	isFirst=Dsp_DrawButtonLst(tmp_img,val,info,BUT_CRD_MONOPOLY_CLOSED,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
	//isFirst=Dsp_DrawButtonLst(tmp_img,info,BUT_CRD_MONOPOLY_CLOSED,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
}
void Dsp_DrawExchangeBut(string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"Exchange",
		XX_EX_OPEN,YY_EX_OPEN,W_EX_OPEN,H_EX_OPEN-1,
		X_TITLE,Y_TITLE,
		X_EX_BUT,Y_EX_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		NUM_OF_BUT_EXCHANGE,
		Ex_text_top, /*Ex_text_bot,*/
		Ex_Color};/* �Ȃ񂩍��H */
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	Dsp_DrawButtonLst(tmp_img,val,info,BUT_GET_DEV_CARD,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
	//Dsp_DrawButtonLst(tmp_img,info,BUT_GET_DEV_CARD,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
}
void Dsp_DrawActionBut(string *val,int onMouseIdx)
{
	static bool isFirst=true;
	static DrawInfo info={"Action",
		XX_ACT_OPEN,
		YY_ACT_OPEN,
		W_ACT_OPEN,
		H_ACT_OPEN-1,
		X_TITLE,Y_TITLE,
		X_ACT_BUT,Y_ACT_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		NUM_OF_BUT_ACTION,
		Act_text_top,
		/*Act_text_bot,*/
		Act_Color};/* �Ȃ񂩍��H */
	Mat tmp_img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));
	Dsp_DrawButtonLst(tmp_img,val,info,BUT_ACT_DEVELOP,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
	//Dsp_DrawButtonLst(tmp_img,info,BUT_ACT_DEVELOP,/*DevRscInMaster_RectLst,*/isFirst,onMouseIdx);
}
#endif
//static Rect ActBut_RectLst[4];/*!< �A�N�V�����F���݁E�J�[�h�g�p�E�_�C�X�E�G���h�EUNDO */
#if DSP_CALL_FROM_ACT_DEBUG
/*!
* @brief ���̃{�^����`��
* @param [info] �`��p�̏����i�[�����\����
* @param [str_bot] ��s�ڂ̃e�L�X�g //�Q�[���i�s����ŕω�����l�B
* @param [RectLst] �̈攻��p�̃{�^���̈��ۑ������z��i����̂ݏ������j
* @param [onMouseIdx] �}�E�X�|�C���^�̏���Ă���{�^���ԍ�(0..info.but_num-1)
*/
bool Dsp_DrawButtonLst(int regionIdx,string *str_bot,int but_idx,int onMouseIdx,bool isFirst)
{
	/* --�J�[�h�̎��--(�Ɛ�E���n�E�X���E���m�E���_) */
	static const string Player_text[]={"player0","player1","player2","player3"};
	static const string Rsc_text[] = {"lumber","brick","wool","grain","ore"};
	static const string Dev_text[]={"house","road","factory"};
	static const string Crd_text[]={"monopoly","harvest","road","soldier","point"};
	static const string Ex_text_top[]={"getDevCrd","2-1","3-1","4-1","propose"};
	static const string Act_text_top[]={"Develop","DevCard","Dice","End","Undo"};
	static const string YesNo_text_top[]={"Yes","No"};

	const DrawInfo draw_info[INPUT_END]={
		{"Player",
		XX_PLAYER,
		YY_PLAYER,
		W_PLAYER,
		H_PLAYER-1,
		X_TITLE,Y_TITLE,
		X_PLAYER_BUT,Y_PLAYER_BUT,
		W_BUT_MEDIUM,H_BUT_MEDIUM,
		NUM_OF_PLAYERS,
		Player_text, 
		Player_Color},
		{"RscInMaster",
		XX_RSC_IN_MASTER,YY_RSC_IN_MASTER,W_RSC_IN_MASTER,H_RSC_IN_MASTER-1,
		X_TITLE,Y_TITLE,
		X_RSC_BUT,Y_RSC_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_RSC_END,
		Rsc_text,
		Rsc_Color},
		{"ResourceInHand",
		XX_RSC_IN_HAND,YY_RSC_IN_HAND,W_RSC_IN_HAND,H_RSC_IN_HAND-1,
		X_TITLE,Y_TITLE,
		X_RSC_BUT,Y_RSC_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_RSC_END,
		Rsc_text, 
		Rsc_Color},
		{"DevInHand",
		XX_DEV_IN_HAND,YY_DEV_IN_HAND,W_DEV_IN_HAND,H_DEV_IN_HAND-1,
		X_TITLE,Y_TITLE,
		X_DEV_BUT,Y_DEV_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_DEV_END,
		Dev_text, 
		Dev_Color},
		{"DevCard Open",
		XX_CRD_OPEN,YY_CRD_OPEN,W_CRD_OPEN,H_CRD_OPEN-1,
		X_TITLE,Y_TITLE,
		X_CRD_BUT,Y_CRD_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_CRD_END,
		Crd_text, 
		Rsc_Color},
		{"DevCard Close",
		XX_CRD_CLOSE,YY_CRD_CLOSE,W_CRD_CLOSE,H_CRD_CLOSE-1,
		X_TITLE,Y_TITLE,
		X_CRD_BUT,Y_CRD_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		TYPE_CRD_END,
		Crd_text, 
		Rsc_Color},
		{"Exchange",
		XX_EX_OPEN,YY_EX_OPEN,W_EX_OPEN,H_EX_OPEN-1,
		X_TITLE,Y_TITLE,
		X_EX_BUT,Y_EX_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		NUM_OF_BUT_EXCHANGE,
		Ex_text_top, 
		Ex_Color},
		{"Action",
		XX_ACT_OPEN,
		YY_ACT_OPEN,
		W_ACT_OPEN,
		H_ACT_OPEN-1,
		X_TITLE,Y_TITLE,
		X_ACT_BUT,Y_ACT_BUT,
		W_BUT_SMALL,H_BUT_SMALL,
		NUM_OF_BUT_ACTION,
		Act_text_top,
		Act_Color},
		{"Answer",
		XX_YESNO,
		YY_YESNO,
		W_YESNO,
		H_YESNO-1,
		X_TITLE,Y_TITLE,
		X_BUT_YESNO,Y_BUT_YESNO,
		W_BUT_SMALL,H_BUT_SMALL,
		NUM_OF_BUT_YESNO,
		YesNo_text_top,
		YesNo_Color}
		};

		/* ��L�̂����Y������{�^���̈�̏�� */
		DrawInfo info=draw_info[regionIdx];
		Mat img     = Mat(main_img, Rect(info.x, info.y, info.w, info.h));
		Mat tmp_img = Mat(info.h, info.w, CV_8UC3, COLOR_GRAY_2);
		putText(tmp_img,info.title,Point(info.x_title,info.y_title),RSC_FONT,0.4,COLOR_WHITE,INFO_THICKNESS,CV_AA);
		for(int i=0;i<info.num_but;i++)
		{
			/* ����̂ݗ̈攻��p�̔z��ɑ�� */
			if(isFirst)
			{
				RectLst[but_idx+i].x=info.x+info.x_but + info.w_but * i;
				RectLst[but_idx+i].y=info.y+info.y_but;
				RectLst[but_idx+i].width=info.w_but-1;/* -1�ׂ͗̃{�^���Ƃ̋��E����邽�� */
				RectLst[but_idx+i].height=info.h_but;
			}
			Mat tmp_img2 = Mat(tmp_img,Rect(info.x_but + info.w_but * i,info.y_but,info.w_but-1,info.h_but));
			if(onMouseIdx == i){
				/* �}�E�X�|�C���^�̏���Ă���{�^���͐F�Ⴂ�ŕ\�� */
				Dsp_DrawButton(tmp_img2, info.but_top_text[i], str_bot[i], info.h_but, info.but_color[i] , COLOR_BLACK);
			}
			else
			{
				Dsp_DrawButton(tmp_img2, info.but_top_text[i], str_bot[i], info.h_but, info.but_color[i]);
			}
		}
		tmp_img.copyTo(img);
		return false;//(����Ăяo�����ǂ������A��
}
#else
/*!
* @brief ���̃{�^����`��
* @param [img] �g���~���O�ς݂̃{�^���`��̈�
* @param [info] �`��p�̏����i�[�����\����
* @param [RectLst] �̈攻��p�̃{�^���̈��ۑ������z��i����̂ݏ������j
* @param [isFirst] ����Ăяo�����ǂ��� 
* @param [onMouseIdx] �}�E�X�|�C���^�̏���Ă���{�^���ԍ�(0..info.but_num-1)
*/
static bool Dsp_DrawButtonLst(Mat &img,string *but_bot_text,DrawInfo info,/*Rect *RectLst,*/int but_idx,bool isFirst,int onMouseIdx)
{
	//Mat img=Mat(main_img,Rect(info.x,info.y,info.w,info.h));

	Mat tmp_img=Mat(info.h,info.w,CV_8UC3,Scalar(50,50,50));
	putText(tmp_img,info.title,Point(info.x_title,info.y_title),RSC_FONT,0.4,COLOR_WHITE,INFO_THICKNESS,CV_AA);
	for(int i=0;i<info.num_but;i++)
	{
		//Rect tmp_rect=Rect(info.x_but + info.w_but * i,info.y_but);
		/* ����̂ݗ̈攻��p�̔z��ɑ�� */
		if(isFirst)
		{
			RectLst[but_idx+i].x=info.x+info.x_but + info.w_but * i;
			RectLst[but_idx+i].y=info.y+info.y_but;
			RectLst[but_idx+i].width=info.w_but-1;/* -1�ׂ͗̃{�^���Ƃ̋��E����邽�� */
			RectLst[but_idx+i].height=info.h_but;
		}
		//Mat tmp_img2=Mat(tmp_img,RectLst[but_idx+i]);
		Mat tmp_img2=Mat(tmp_img,Rect(info.x_but + info.w_but * i,info.y_but,info.w_but-1,info.h_but));
		if(onMouseIdx==i){
			/* �}�E�X�|�C���^�̏���Ă���{�^���͐F�Ⴂ�ŕ\�� */
			Dsp_DrawButton(tmp_img2,info.but_top_text[i],but_bot_text[i],info.h_but,info.but_color[i],COLOR_BLACK);
		}
		else
		{
			Dsp_DrawButton(tmp_img2,info.but_top_text[i],but_bot_text[i],info.h_but,info.but_color[i]);
		}
	}
	tmp_img.copyTo(img);
	imshow(MAIN_WINDOW_NAME,main_img);
	waitKey();
	return false;//(����Ăяo�����ǂ������A��
}
#endif
/*!
* @brief 1�̃{�^����`��B
* @param [img] �g���~���O�ς݃{�^���`��̈� 
* @param [str_top] ��s�ڂɕ\�����镶����i�{�^���̎�ށj
* @param [str_bot] ��s�ڂɕ\�����镶����i��ނɑ΂��鐔�l�j
* @param [y_bot] ��s�ڂ̕\���ʒu
* @param [color] �e�L�X�g�̐F 
*/
static void Dsp_DrawButton(Mat &img,string str_top,string str_bot,int y_bot,Scalar color,Scalar bg_color)
{
	Mat tmp_img=Mat(img.rows,img.cols,CV_8UC3,bg_color);
	putText(tmp_img, str_top, Point(5,(y_bot/2)-5), RSC_FONT, 0.4, color, INFO_THICKNESS, CV_AA);
	putText(tmp_img, str_bot, Point(5,y_bot-5),     RSC_FONT, 0.4, color, INFO_THICKNESS, CV_AA);
	tmp_img.copyTo(img);
}
void Dsp_DrawMsg(string message,int window_num)
{
	int xx_msg = window_num==0 ? XX_MSG_0 :
		window_num==1 ? XX_MSG_1 : XX_MSG_2;
	int yy_msg = window_num==0 ? YY_MSG_0 :
		window_num==1 ? YY_MSG_1: YY_MSG_2;
	int h_msg = window_num==0 ? H_MSG_0 :H_MSG_1;
	int y_msg_txt = window_num==0 ? Y_MSG_TXT_0 :Y_MSG_TXT_1;
	Mat tmp_img=Mat(h_msg,W_MSG,CV_8UC3,COLOR_BLACK);
	double font_scale = window_num==0 ? FONT_SCALE_MSG : FONT_SCALE_MSG_1;
	putText(tmp_img,message,Point(X_MSG_TXT,y_msg_txt),RSC_FONT,font_scale,COLOR_GREEN,INFO_THICKNESS,CV_AA);
	Mat dst_img=Mat(main_img,Rect(xx_msg,yy_msg,W_MSG,h_msg));
	tmp_img.copyTo(dst_img);
	Dsp_UpdateMainWindow();
	/*if(message.size()>NUM_OF_TXT_PER_LINE)
	{
	stringstream stream;
	for(int i=0;i<NUM_OF_TXT_PER_LINE;i++)
	{
	stream<<message[i];
	}
	putText(tmp_img,stream.str(),Point(X_MSG_TXT,Y_MSG_TXT),RSC_FONT,0.4,COLOR_GREEN,INFO_THICKNESS,CV_AA);
	}else
	{
	putText(tmp_img,message,Point(X_MSG_TXT,Y_MSG_TXT),RSC_FONT,0.4,COLOR_GREEN,INFO_THICKNESS,CV_AA);

	}*/
}
/*!
* @brief ��D�̎�������`��
* @param [img] ��D�̎����p�̗̈��؂�o����Mat 
*/
//void Dsp_DrawRscInHand_(Mat &img)
//{
//	stringstream stream;
//
//	Mat tmp_img=Mat(img.rows,img.cols,CV_8UC3,Scalar(200,200,200));
//	stream<<"ResourceInHand";
//	//putText(tmp_img,stream.str(), cv::Point(X_RSC_TITLE,Y_RSC_TITLE), RSC_FONT, 0.4/*FONT_SCALE*/, COLOR_WHITE, INFO_THICKNESS, CV_AA);
//	for(int i=0;i<TYPE_RSC_END;i++)
//	{
//		/* �����̕`�� */
//		putText(tmp_img,Rsc_text[i],Point( X_RSC_BUT + W_BUT_SMALL * i ,Y_RSC_BUT),RSC_FONT,0.4,Rsc_Color[i],INFO_THICKNESS,CV_AA);
//	}
//	imshow("tmp",tmp_img);
//	waitKey();
//	//tmp_img.copyTo(img);
//	tmp_img.clone().copyTo(img);
//}
static void Dsp_DrawLine(Mat &img,const int cs_idx)
{
	line(img,mcs[cs_idx].p1->pt,mcs[cs_idx].p2->pt,COLOR_GRAY_3, LINE_DEFAULT_WIDTH, 8);
}
void Dsp_ClearMap(void)
{
	Mat tmp_img=Mat(WINDOW_HEIGHT,XX,CV_8UC3,COLOR_BLACK);
	Mat dst_img=Mat(main_img,Rect(0,0,XX,WINDOW_HEIGHT));
	tmp_img.copyTo(dst_img);
}
void Dsp_DrawRoad(const int cs_idx,bool isRoadThere=false,int player=NO_PLAYER,int type_hrb=TYPE_HRB_END)
{
	/* �`�`��p�e�L�X�g */
	static const string Hrb_text[TYPE_HRB_END]={"L2-1","B2-1","W2-1","G2-1","O2-1","3-1"};
	int line_width=isRoadThere?LINE_ROAD_WIDTH:LINE_DEFAULT_WIDTH;
	Scalar color=isRoadThere?Player_Color[player]:COLOR_GRAY_3;
	line(main_img,mcs[cs_idx].p1->pt,mcs[cs_idx].p2->pt,color,line_width, 8);
	if(type_hrb!=TYPE_HRB_END)
	{
		if(type_hrb!=TYPE_HRB_31)
			putText(main_img,Hrb_text[type_hrb], cv::Point(mcs[cs_idx].center.x-15,mcs[cs_idx].center.y), RSC_FONT, 0.4/*FONT_SCALE*/,Rsc_Color[type_hrb], THICKNESS_CH_CONTENT, CV_AA);
		else 
			putText(main_img,Hrb_text[type_hrb], cv::Point(mcs[cs_idx].center.x-15,mcs[cs_idx].center.y), RSC_FONT, 0.4/*FONT_SCALE*/,COLOR_WHITE, THICKNESS_CH_CONTENT, CV_AA);
	}
}
void Dsp_DrawHouseFactory(int cp_idx,int player,int type_dev)
{
	/* �l�p����`���R�[�h */
	//Mat src_img= Mat(HEIGHT_HOUSE,WIDTH_HOUSE,CV_8UC3,Player_Color[player]);
	////Mat tmp_img=Mat(img.rows,img.cols,CV_8UC3,bg_color);
	//Point p=mcp[cp_idx].pt;
	//Mat dst_img=Mat(main_img,Rect(p.x-WIDTH_HOUSE/2,p.y-HEIGHT_HOUSE/2,WIDTH_HOUSE,HEIGHT_HOUSE));
	//src_img.copyTo(dst_img);
	/* ���ɐ�������R�[�h */
	if(type_dev!=TYPE_DEV_END){
		Mat src_img= Mat(HEIGHT_HOUSE,WIDTH_HOUSE,CV_8UC3,COLOR_BLACK);
		string strNumDev=type_dev==TYPE_DEV_HOUSE ? "1" : "2" ;
		rectangle(src_img,Point(0,0),Point(WIDTH_HOUSE,HEIGHT_HOUSE),Player_Color[player],2);
		putText(src_img,strNumDev, cv::Point(WIDTH_HOUSE/3.0,HEIGHT_HOUSE*0.7), DEV_FONT, 0.4/*FONT_SCALE*/, Player_Color[player], THICKNESS_DEV, CV_AA);

		Point p=mcp[cp_idx].pt;
		Mat dst_img=Mat(main_img,Rect(p.x-WIDTH_HOUSE/2,p.y-HEIGHT_HOUSE/2,WIDTH_HOUSE,HEIGHT_HOUSE));
		src_img.copyTo(dst_img);
	}
}

void Dsp_DrawCHContent(const int ch_idx,int type,int diceNum,bool isThiefThere)
{
	stringstream stream;
	//RSC_FONT
	/* �\������e�L�X�g */
	if(type==TYPE_RSC_END)return;
	string str = type==TYPE_RSC_LUMBER ? "lumber" :
		type == TYPE_RSC_BRICK ? "brick" :
		type == TYPE_RSC_WOOL  ? " wool" :
		type == TYPE_RSC_GRAIN ? "grain"  :
		type == TYPE_RSC_ORE   ? " ore " : "unknown";
	Scalar color=isThiefThere ? COLOR_GRAY_2 : Rsc_Color[type];
	stream << str;// << " " << diceNum;
	putText(main_img,stream.str(), cv::Point(mch[ch_idx].center.x-30,mch[ch_idx].center.y-10), RSC_FONT, FONT_SCALE_CH_CONTENT/*FONT_SCALE*/, color,THICKNESS_CH_CONTENT , CV_AA);
	stream.str("");
	stream<<diceNum<<" ";
	for(int i=0;i<6 - ABS(7-diceNum);i++)stream<<".";

	putText(main_img,stream.str(), cv::Point(mch[ch_idx].center.x-15,mch[ch_idx].center.y+20), RSC_FONT, FONT_SCALE_CH_CONTENT, color, THICKNESS_CH_CONTENT, CV_AA);
#if DEBUG_COUT_MAP_CONTENT
	//imshow(MAIN_WINDOW_NAME,main_img);
	//waitKey();
#endif
}
/* --�̈攻��------------------------------------------------------------------ */

/*!
@brief �}�E�X�|�C���^�̈ʒu���ǂ̃I�u�W�F�N�g�ɋ߂�����n��
@param [] 
*/
Dsp_MapObj Dsp_GetNNObj(int x,int y){
	Dsp_MapObj tmp_id;
	tmp_id.id=-1;
	tmp_id.typeObj=TYPE_OBJ_END;
	long min_dist=1000000,tmp_dist=min_dist;
	Point pt=Point(x,y);
	/*�_�ɂ��đ���*/
	for(int i=0;i<NUM_OF_POINTS;i++)
	{
		tmp_dist=Dsp_GetEucDistance(pt,mcp[/*dcp[i]*/i].pt);
		if(tmp_dist<min_dist)
		{
			min_dist=tmp_dist;
			tmp_id.typeObj=TYPE_OBJ_POINT;
			tmp_id.id=/*dcp[i]*/i;
		}
	}
	/*�ӂɂ��đ���*/
	for(int i=0;i</*sizeDebugSide*/NUM_OF_SIDES;i++)
	{
		tmp_dist=Dsp_GetEucDistance(pt,mcs[/*dcs[i]*/i].center);
		if(tmp_dist<min_dist)
		{
			min_dist=tmp_dist;
			tmp_id.typeObj=TYPE_OBJ_SIDE;
			tmp_id.id=/*dcs[i]*/i;
		}
	}
	/*�Z�p�`�ɂ��đ���*/
	for(int i=0;i</*sizeDebugHexa*/NUM_OF_HEXAGONS;i++)
	{
		tmp_dist=Dsp_GetEucDistance(pt,mch[/*dch[i]*/i].center);
		if(tmp_dist<min_dist)
		{
			min_dist=tmp_dist;
			tmp_id.typeObj=TYPE_OBJ_HEXAGON;
			tmp_id.id=/*dch[i]*/i;
		}
	}
	return tmp_id;
}

static long Dsp_GetEucDistance(const Point &pt1, const Point &pt2)
{
	long x1=pt1.x,x2=pt2.x;
	long y1=pt1.y,y2=pt2.y;
	long retval=EUC_DIST(x1,y1,x2,y2);
	return retval;
}

bool isInRect(int x,int y,Rect rect)
{
	bool isIn=false;
	if(rect.x <= x && x <= rect.x + rect.width &&
		rect.y <= y && y <= rect.y + rect.height)
	{
		isIn=true;
	}
	return isIn;
}
bool isInRect(int x,int y,int but_idx)
{
	bool isIn=false;
	Rect rect=RectLst[but_idx];
	if(rect.x <= x && x <= rect.x + rect.width &&
		rect.y <= y && y <= rect.y + rect.height)
	{
		isIn=true;
	}
	return isIn;
}

void Dsp_UpdateMainWindow(void)
{
	imshow(MAIN_WINDOW_NAME,main_img);
	//waitKey(1);
}
void Dsp_UpdateMainWindow_Debug(void)
{
	imshow(MAIN_WINDOW_NAME,main_img);
	waitKey(1);
}

/*!
* @brief �����{�^���̈�̃e�L�X�g���i��s�ځj��Ԃ��B��ɓ����e�L�X�g��\���B
* @param [player] ���݂̃v���C�����B�g��Ȃ��B
* @param [val] �������̊i�[��B�Ԃ�l
*/
void Dsp_GetInfo_ExchangeBut(int player,string *val)
{
	for(int i=0;i<NUM_OF_BUT_EXCHANGE;i++)
	{
		val[i]=Ex_text_bot[i];
	}
}

/*!
* @brief �A�N�V�����{�^���̈�̃e�L�X�g���i��s�ځj��Ԃ��B��ɓ����e�L�X�g��\���B
* @param [player] ���݂̃v���C�����B�g��Ȃ��B
* @param [val] �������̊i�[��B�Ԃ�l
*/
void Dsp_GetInfo_ActionBut(int player,string *val)
{
	for(int i=0;i<NUM_OF_BUT_ACTION;i++)
	{
		val[i]=Act_text_bot[i];
	}
}
/*!
* @brief �A�N�V�����{�^���̈�̃e�L�X�g���i��s�ځj��Ԃ��B��ɓ����e�L�X�g��\���B
* @param [player] ���݂̃v���C�����B�g��Ȃ��B
* @param [val] �������̊i�[��B�Ԃ�l
*/
void Dsp_GetInfo_YesNoBut(int player,string *val)
{
	for(int i=0;i<NUM_OF_BUT_YESNO;i++)
	{
		val[i]=YesNo_text_bot[i];
	}
}
