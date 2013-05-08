/*!
* @file Map.cpp
* @brief マップ描画、マップ上の建物の配置を管理
* @author semitsu
* @date 12.10.01 開始
*/

#include "common.h"
#include "Map.h"
#include "Act.h"//ダイス
#include "Disp.h"//道・家の描画
#include "Resource.h"
/*マクロ定義  ----------------------------------------------------------- */

/* 変数型宣言 ----------------------------------------------------------  */
static int FirstHouse[NUM_OF_PLAYERS];
static int SecondHouse[NUM_OF_PLAYERS];
static int longest_road_num = LONGEST_ROAD_MIN;
static int longest_road_player = NO_PLAYER;
//debug ---------------------------
int dcp[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53};
int sizeDebugPoint=sizeof(dcp)/sizeof(int);
int dcs[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71};
int sizeDebugSide=sizeof(dcs)/sizeof(int);
int dch[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
int sizeDebugHexa=sizeof(dch)/sizeof(int);
const static int CH_DICE[]={2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12};
const static int CH_RSC[]={
	TYPE_RSC_LUMBER,
	TYPE_RSC_LUMBER,
	TYPE_RSC_LUMBER,
	TYPE_RSC_LUMBER,
	TYPE_RSC_BRICK,
	TYPE_RSC_BRICK,
	TYPE_RSC_BRICK,
	TYPE_RSC_WOOL,
	TYPE_RSC_WOOL,
	TYPE_RSC_WOOL,
	TYPE_RSC_WOOL,
	TYPE_RSC_GRAIN,
	TYPE_RSC_GRAIN,
	TYPE_RSC_GRAIN,
	TYPE_RSC_GRAIN,
	TYPE_RSC_ORE,
	TYPE_RSC_ORE,
	TYPE_RSC_ORE
};
const static int TypeLst_Of_Harbor[NUM_OF_HARBOR]={
	TYPE_HRB_LUMBER21,
	TYPE_HRB_BRICK21,
	TYPE_HRB_WOOL21,
	TYPE_HRB_GRAIN21,
	TYPE_HRB_ORE21,
	TYPE_HRB_31,
	TYPE_HRB_31,
	TYPE_HRB_31,
	TYPE_HRB_31
};
static int CsIdxLst_Of_Harbor[NUM_OF_HARBOR]={NO_SIDE,NO_SIDE,NO_SIDE,NO_SIDE,NO_SIDE,NO_SIDE,NO_SIDE,NO_SIDE,NO_SIDE};
/* 港のあるCSのインデックス */
const static int CS_IDX_HARBOR[NUM_OF_HARBOR]={
	0,8,25,59,68,69,54,29,5
};
//---------------------------------
static struct CatanPoint;
static struct CatanSide;
static struct CatanHexagon;
static int cur_CH_OF_THIEF=DESERT_CH;/* 現在の盗賊のいる位置 */
/*!
@brief 六角形の点
*/
static struct CatanPoint{
	int id;
	//Point pt;
	CatanSide *parent_side[3];
	CatanHexagon *parent_hexa[3];
	CatanPoint *adjacent_point[3];
	/* ゲーム部 */
	//bool isHouseThere;
	TypeDevelopment development;
	int owner;
}cp[NUM_OF_POINTS];
CatanPoint prev_cp[NUM_OF_POINTS];
/*!
@brief 六角形の辺

(row,col)のうちrowの小さいものがp1,同じならcolの小さいものがp1
*/
static struct CatanSide{
	int id;
	CatanPoint *p1;
	CatanPoint *p2;
	//Point center;
	CatanSide *adjacent_side[4];
	/* ゲーム部 */
	int type_hrb;
	bool isRoadThere;/*道が建てられているか*/
	int owner;/* 0-3 owner */
}cs[NUM_OF_SIDES];
CatanSide prev_cs[NUM_OF_SIDES];
/*! @brief 六角形 */
static struct CatanHexagon{
	int id;
	/* 位置情報 */
	CatanPoint *child_pt[6];
	CatanSide *child_side[6];
	CatanHexagon *adjacent_hexa[6];
	//Point center;
	/* ゲーム部 */
	int rscType;
	int numHexa;
	bool isThiefThere;
}ch[NUM_OF_HEXAGONS];
CatanHexagon prev_ch[NUM_OF_HEXAGONS];
//static CatanHexagon ;
static void IN_MakeCatanSide(int cs_idx,int cp_idx1,int cp_idx2);
static void IN_MakeCatanHexagon(int ch_idx,int cp_idx0,int cp_idx1,int cp_idx2,int cp_idx3,int cp_idx4,int cp_idx5
	/*,int cs_idx0,int cs_idx1,int cs_idx2,int cs_idx3,int cs_idx4,int cs_idx5*/);
static void IN_setCP_ParentCH(int cp_idx);
static void IN_setCP_ParentCS(int cp_idx);
static void IN_setCH_ChildCS(int ch_idx);
static void IN_setCH_AdjacentCH(int ch_idx);
static void IN_setCS_AdjacentCS(int cs_idx);
static void IN_setCP_AdjacentCP(int cp_idx);
static void Map_ProduceResourceCPCH(CatanHexagon *parent_ch,CatanPoint *child_cp);
static void IN_setCP_ParentCS_AdjacentCP(int cp_idx);

static bool Map_IsMyRoadAroundCP(int player, int cp_idx);
static bool Map_IsHouseAroundCP(int cp_idx);
static void Map_DrawCHContent(int ch_idx);
static int Map_RecLength(int cs_idx,int cp_idx,int player,int *check,int length);

static void Map_ProduceResourceChildCP(int ch_idx);

/* ----関数記述----------------------------------------------------------- */
void Map_DrawMap(void)
{
	Dsp_ClearMap();
	for(int i=0;i<NUM_OF_SIDES;i++)
	{
		Map_DrawRoad(i);
		//Dsp_DrawRoad(i,cs[i].isRoadThere,cs[i].owner,cs[i].type_hrb);
	}
	for(int i=0;i<NUM_OF_HEXAGONS;i++)
	{
		if(i==DESERT_CH)continue;
		Map_DrawCHContent(i);
		//Dsp_DrawCHContent(i,ch[i].rscType,ch[i].numHexa,ch[i].isThiefThere);
	}
	for(int i=0;i<NUM_OF_POINTS;i++)
	{
		Map_DrawHouseFactory(i);
	}
	/* 各入力インターフェース描画関数 初回呼び出し */
#if !(DSP_CALL_FROM_ACT_DEBUG)
	string val[]={"1","2","3","4","5"};
	Dsp_DrawRscInHand(/*main_img,*/val);
	string val2[]={"1","2","3"};
	string val4[]={"a","b","c","d"};
	Dsp_DrawPlayer(val4);
	Dsp_DrawExchangeBut(Ex_text_bot);

	Dsp_DrawDevRscInHand(val2);
	Dsp_DrawRscInMaster(val);
	Dsp_DrawDevCardOpen(val);
	Dsp_DrawDevCardClose(val);
	Dsp_DrawActionBut(Act_text_bot);
#endif
	//for(int i=
	Dsp_UpdateMainWindow();
	//imshow(MAIN_WINDOW_NAME,main_img);
}
void Map_SaveState(void)
{
	memcpy(cp,prev_cp,sizeof(CatanPoint)*NUM_OF_POINTS);
	memcpy(cs,prev_cs,sizeof(CatanSide)*NUM_OF_SIDES);
	memcpy(ch,prev_ch,sizeof(CatanHexagon)*NUM_OF_HEXAGONS);
}
void Map_LoadState()
{
	memcpy(prev_cp,cp,sizeof(CatanPoint)*NUM_OF_POINTS);
	memcpy(prev_cs,cs,sizeof(CatanSide)*NUM_OF_SIDES);
	memcpy(prev_ch,ch,sizeof(CatanHexagon)*NUM_OF_HEXAGONS);
}

/*!
 * @brief dstの値がdefaultのままならvalを代入。他の値が入っているなら何もせずfalseを返す
 * @param [dst] 値の格納先
 * @param [default_val] 初期値
 * @param [val] 初期値の時、新たに代入する値
 */
static bool IN_setVal(int &dst,int default_val,int val)
{
	bool isSet=false;
	if(dst==default_val)
	{
		dst=val;
		isSet=true;
	}
	return isSet;
}
static bool IN_setCH_NumHexa(int num,int ch_idx)
{
	bool isSet=false;
	if(ch[ch_idx].numHexa==IN_VAL_NUM_HEXA)
	{
		ch[ch_idx].numHexa=num;
		isSet=true;
	}
	return isSet;
}
static bool IN_setCH_Resource(int type,int ch_idx)
{
	bool isSet=false;
	if(ch[ch_idx].rscType==TYPE_RSC_END)
	{
		ch[ch_idx].rscType=type;
		isSet=true;
	}
	return isSet;
}
static bool IN_setCS_Harbor(int type_hrb,int cs_idx)
{
	bool isSet=false;
	if(cs[cs_idx].type_hrb==TYPE_HRB_END)
	{
		cs[cs_idx].type_hrb=type_hrb;

		isSet=true;
	}
	return isSet;
}
/*!
 * @brief CHの資源・ダイスの目を初期化(砂漠の位置固定)
 * @param [] 
 */
void IN_InitMapContents(void)
{
	/* 資源をセット */
	for(int i=0;i<NUM_OF_HEXAGONS-1;i++)
	{
		bool isSet=false;
		while(!isSet)
		{
			int ch_idx=Act_Rand(NUM_OF_HEXAGONS-1);
			if(ch_idx==DESERT_CH)continue;
			//isSet=IN_setCH_Resource(CH_RSC[i],ch_idx);
			isSet=IN_setVal(ch[ch_idx].rscType,TYPE_RSC_END,CH_RSC[i]);

		}
#if DEBUG_COUT_MAP_CONTENT
			cout<<i<<" rsc set finished"<<endl;
#endif
	}
	for(int i=0;i<NUM_OF_HEXAGONS-1;i++)
	{
		bool isSet=false;
		/* セットできるまで繰り返す */
		while(!isSet)
		{
			int ch_idx=Act_Rand(NUM_OF_HEXAGONS-1);
			if(ch_idx==DESERT_CH)continue;
			//isSet=IN_setCH_NumHexa(CH_DICE[i],ch_idx);
			isSet=IN_setVal(ch[ch_idx].numHexa,IN_VAL_NUM_HEXA,CH_DICE[i]);
		}
		#if DEBUG_COUT_MAP_CONTENT
			cout<<i<<" dice set finished"<<endl;
#endif
	}
	ch[DESERT_CH].isThiefThere=true;/* 盗賊の初期位置 */
	for(int i=0;i<NUM_OF_HARBOR;i++)
	{
		bool isSet=false;
		/* セットできるまで繰り返す */
		while(!isSet)
		{
			int cs_idx=CS_IDX_HARBOR[Act_Rand(NUM_OF_HARBOR-1)];
			//isSet=IN_setCS_Harbor(CS_HA[i],cs_idx);
			isSet=IN_setVal(cs[cs_idx].type_hrb,TYPE_HRB_END,TypeLst_Of_Harbor[i]);
			CsIdxLst_Of_Harbor[i]=cs_idx;
		}
#if DEBUG_COUT_MAP_CONTENT
			cout<<i<<" hrb set finished"<<endl;
#endif
	}
}

/*!
@brief 初期のマップを生成・描画する
*/
void IN_InitMap(){

	/*CPを初期化*/
	for(int i=0;i<NUM_OF_POINTS;i++)
	{
		cp[i].id=i;
		cp[i].development=TYPE_DEV_END;
		cp[i].owner=NO_PLAYER;
		for(int j=0;j<3;j++)
		{
			cp[i].parent_hexa[j]=NULL;
			cp[i].parent_side[j]=NULL;
			cp[i].adjacent_point[j]=NULL;
		}
	}
	/* ポインタ初期化 */
	for(int i = 0 ; i < NUM_OF_SIDES ; i++)
	{
		cs[i].p1 = NULL;
		cs[i].p2 = NULL;
		//cs[i].id=i;
		//cs[i].owner=NO_PLAYER;
		//cs[i].type_hrb=TYPE_HRB_END;
		//cs[i].isRoadThere=false;
		for(int j=0;j<4;j++)
		{
			cs[i].adjacent_side[j]=NULL;
		}
	}
	for(int i=0;i<NUM_OF_HEXAGONS;i++)
	{
		ch[i].id=i;
		ch[i].isThiefThere=false;
		ch[i].numHexa=IN_VAL_NUM_HEXA;
		ch[i].rscType=TYPE_RSC_END;
		for(int j=0;j<6;j++)
		{
			ch[i].child_pt[j]=NULL;
			ch[i].child_side[j]=NULL;
			ch[i].adjacent_hexa[j]=NULL;
		}
	}
	/* 辺の初期化 */
	IN_MakeCatanSide(0,0,1);
	IN_MakeCatanSide(1,0,3);
	IN_MakeCatanSide(2,1,4);
	IN_MakeCatanSide(3,2,3);
	IN_MakeCatanSide(4,4,5);
	IN_MakeCatanSide(5,2,7);
	IN_MakeCatanSide(6,3,8);
	IN_MakeCatanSide(7,4,9);
	IN_MakeCatanSide(8,5,10);
	IN_MakeCatanSide(9,6,7);
	IN_MakeCatanSide(10,8,9);
	IN_MakeCatanSide(11,10,11);
	IN_MakeCatanSide(12,6,12);
	IN_MakeCatanSide(13,7,13);
	IN_MakeCatanSide(14,8,14);
	IN_MakeCatanSide(15,9,15);
	IN_MakeCatanSide(16,10,16);
	IN_MakeCatanSide(17,11,17);
	IN_MakeCatanSide(18,13,14);
	IN_MakeCatanSide(19,15,16);
	IN_MakeCatanSide(20,12,18);
	IN_MakeCatanSide(21,13,19);
	IN_MakeCatanSide(22,14,20);
	IN_MakeCatanSide(23,15,21);
	IN_MakeCatanSide(24,16,22);
	IN_MakeCatanSide(25,17,23);
	IN_MakeCatanSide(26,18,19);
	IN_MakeCatanSide(27,20,21);
	IN_MakeCatanSide(28,22,23);
	IN_MakeCatanSide(29,18,24);
	IN_MakeCatanSide(30,19,25);
	IN_MakeCatanSide(31,20,26);
	IN_MakeCatanSide(32,21,27);
	IN_MakeCatanSide(33,22,28);
	IN_MakeCatanSide(34,23,29);
	IN_MakeCatanSide(35,25,26);
	IN_MakeCatanSide(36,27,28);
	IN_MakeCatanSide(37,24,30);
	IN_MakeCatanSide(38,25,31);
	IN_MakeCatanSide(39,26,32);
	IN_MakeCatanSide(40,27,33);
	IN_MakeCatanSide(41,28,34);
	IN_MakeCatanSide(42,29,35);
	IN_MakeCatanSide(43,30,31);
	IN_MakeCatanSide(44,32,33);
	IN_MakeCatanSide(45,34,35);
	IN_MakeCatanSide(46,30,36);
	IN_MakeCatanSide(47,31,37);
	IN_MakeCatanSide(48,32,38);
	IN_MakeCatanSide(49,33,39);
	IN_MakeCatanSide(50,34,40);
	IN_MakeCatanSide(51,35,41);
	IN_MakeCatanSide(52,37,38);
	IN_MakeCatanSide(53,39,40);
	IN_MakeCatanSide(54,36,42);
	IN_MakeCatanSide(55,37,43);
	IN_MakeCatanSide(56,38,44);
	IN_MakeCatanSide(57,39,45);
	IN_MakeCatanSide(58,40,46);
	IN_MakeCatanSide(59,41,47);
	IN_MakeCatanSide(60,42,43);
	IN_MakeCatanSide(61,44,45);
	IN_MakeCatanSide(62,46,47);
	IN_MakeCatanSide(63,43,48);
	IN_MakeCatanSide(64,44,49);
	IN_MakeCatanSide(65,45,50);
	IN_MakeCatanSide(66,46,51);
	IN_MakeCatanSide(67,48,49);
	IN_MakeCatanSide(68,50,51);
	IN_MakeCatanSide(69,49,52);
	IN_MakeCatanSide(70,50,53);
	IN_MakeCatanSide(71,52,53);
	/* 六角形の初期化 */
	IN_MakeCatanHexagon(0,0,1,4,9,8,3/*,0,2,7,10,6,1*/);
	IN_MakeCatanHexagon(1,4,5,10,16,15,9/*,4,8,16,19,15,7*/);
	IN_MakeCatanHexagon(2,10,11,17,23,22,16);
	IN_MakeCatanHexagon(3,22,23,29,35,34,28);
	IN_MakeCatanHexagon(4,34,35,41,47,46,40);
	IN_MakeCatanHexagon(5,39,40,46,51,50,45);
	IN_MakeCatanHexagon(6,44,45,50,53,52,49);
	IN_MakeCatanHexagon(7,37,38,44,49,48,43);
	IN_MakeCatanHexagon(8,30,31,37,43,42,36);
	IN_MakeCatanHexagon(9,18,19,25,31,30,24);
	IN_MakeCatanHexagon(10,6,7,13,19,18,12);
	IN_MakeCatanHexagon(11,2,3,8,14,13,7/*,3,6,14,18,13,5*/);
	IN_MakeCatanHexagon(12,8,9,15,21,20,14);
	IN_MakeCatanHexagon(13,15,16,22,28,27,21);
	IN_MakeCatanHexagon(14,27,28,34,40,39,33);
	IN_MakeCatanHexagon(15,32,33,39,45,44,38);
	IN_MakeCatanHexagon(16,25,26,32,38,37,31);
	IN_MakeCatanHexagon(17,13,14,20,26,25,19);
	IN_MakeCatanHexagon(18,20,21,27,33,32,26);

	//親CH,CSを検索、セット
	for(int i=0;i<sizeDebugPoint;i++)
	{
		IN_setCP_ParentCH(dcp[i]);
		//IN_setCP_ParentCS(dcp[i]);
		IN_setCP_ParentCS_AdjacentCP(dcp[i]);
	}
	//CHの子CS、隣接CHを検索、セット
	for(int i=0;i<sizeDebugHexa;i++)
	{
		IN_setCH_ChildCS(dch[i]);
		IN_setCH_AdjacentCH(dch[i]);
	}	
	/* CSの隣接CSを検索、セット */
	for(int i=0;i<sizeDebugSide;i++)
	{
		IN_setCS_AdjacentCS(dcs[i]);
	}
	/* CPの隣接CPを検索、セット */
	//for(int i=0;i<sizeDebugSide;i++)
	//{
	//	IN_setCP_AdjacentCP(dcp[i]);
	//}
}

/*!
@brief 六角形の辺を初期化
@param [cs_idx] 辺のid
[cp_idx1,2] 点のid 上にある方が、同じなら左にある方が1
*/
static void IN_MakeCatanSide(int cs_idx,int cp_idx1,int cp_idx2)
{
	CatanSide *cs_tmp=&cs[cs_idx];
	cs_tmp->p1=&cp[cp_idx1];
	cs_tmp->p2=&cp[cp_idx2];
	
	cs_tmp->id=cs_idx;
	cs_tmp->type_hrb=TYPE_HRB_END;
	cs_tmp->isRoadThere=false;
	cs_tmp->owner=NO_PLAYER;//とらない数字
	
}
static void IN_MakeCatanHexagon(int ch_idx,int cp_idx0,int cp_idx1,int cp_idx2,int cp_idx3,int cp_idx4,int cp_idx5
	/*,int cs_idx0,int cs_idx1,int cs_idx2,int cs_idx3,int cs_idx4,int cs_idx5*/)
{
	CatanHexagon ch_tmp=ch[ch_idx];
	ch[ch_idx].child_pt[0]=&cp[cp_idx0];
	ch[ch_idx].child_pt[1]=&cp[cp_idx1];
	ch[ch_idx].child_pt[2]=&cp[cp_idx2];
	ch[ch_idx].child_pt[3]=&cp[cp_idx3];
	ch[ch_idx].child_pt[4]=&cp[cp_idx4];
	ch[ch_idx].child_pt[5]=&cp[cp_idx5];
	ch[ch_idx].id=ch_idx;
	ch[ch_idx].isThiefThere=false;
	ch[ch_idx].numHexa=IN_VAL_NUM_HEXA;/* 2個のダイスがとらない数字 */
	ch[ch_idx].rscType=TYPE_RSC_END;
}

/*!
@brief CatanPointの属する六角形を検索
@param [cp_idx] CatanPointのID 
*/
static void IN_setCP_ParentCH(int cp_idx)
{
	int p_idx=0;  /* CPの親CHを指すポインタ配列のインデックス*/
	for(int i=0;i<sizeDebugHexa;i++)
	{
		bool boolFoundParent=false;
		for(int j=0;j<6;j++)
		{
			if(&cp[cp_idx]==ch[dch[i]].child_pt[j])
			{
				cp[cp_idx].parent_hexa[p_idx++]=&ch[dch[i]];
				boolFoundParent=true;
			}
			if(boolFoundParent)break;
		}//for 各頂点
		if(p_idx==3)break;
	}//for 六角形
}

/*!
@brief  CatanPointの属する辺を検索
@param [cp_idx] CatanPointのID 
*/
static void IN_setCP_ParentCS(int cp_idx)
{
	int p_idx=0;/* CPの親CSを指すポインタ配列のインデックス*/
	for(int i=0;i<sizeDebugSide;i++){
		if(&cp[cp_idx]==cs[dcs[i]].p1 || &cp[cp_idx]==cs[dcs[i]].p2){
			cp[cp_idx].parent_side[p_idx++]=&cs[dcs[i]];
		}
		if(p_idx==3)break;
	}
}
/*!
@brief  CatanPointの属する辺を検索
@param [cp_idx] CatanPointのID 
*/
static void IN_setCP_ParentCS_AdjacentCP(int cp_idx)
{
	int p_idx=0;/* CPの親CSを指すポインタ配列のインデックス*/
	for(int i=0;i<sizeDebugSide;i++){
		if(&cp[cp_idx]==cs[dcs[i]].p1){
			cp[cp_idx].parent_side[p_idx]=&cs[dcs[i]];
			cp[cp_idx].adjacent_point[p_idx]=cs[dcs[i]].p2;
			p_idx++;
		}
		else if( &cp[cp_idx]==cs[dcs[i]].p2)
		{
			cp[cp_idx].parent_side[p_idx]=&cs[dcs[i]];
			cp[cp_idx].adjacent_point[p_idx]=cs[dcs[i]].p1;
			p_idx++;
		}
		if(p_idx==3)break;
	}
}
/*!
@brief  CatanHexagonの有する辺を検索
@param [ch_idx] CatanHexagonのID 
*/
static void IN_setCH_ChildCS(int ch_idx)
{
	//int c_idx=0;/* CHの子CSを指すポインタ配列のインデックス*/
	int cp_pair[6][2]={{0,1},{1,2},{2,3},{4,3},{5,4},{0,5}};/* CHの子CSのCPインデックスのペア*/
	/* CHの子CSについて走査*/
	for(int j=0;j<6;j++)
	{
		for(int i=0;i<sizeDebugSide;i++)
		{
			int *pair=cp_pair[j];
			if(ch[ch_idx].child_pt[pair[0]]==cs[dcs[i]].p1 && ch[ch_idx].child_pt[pair[1]]==cs[dcs[i]].p2)
			{
				ch[ch_idx].child_side[j]=&cs[dcs[i]];
				break;
			}// if 辺が六角形の辺と一致
		}// for 辺について走査
	}// for 子CSについて走査
}
/*!
@brief CatanHexagonの隣接CHを検索
@param [ch_idx] CatanHexagonのインデックス
*/
static void IN_setCH_AdjacentCH(int ch_idx)
{
	/* 隣接するときの互いの子CSのインデックスの組 */
	int cs_pair[6][2]={{0,3},{1,4},{2,5},{3,0},{4,1},{5,2}};
	for(int j=0;j<6;j++)
	{
		int *pair=cs_pair[j];
		for(int i=0;i<sizeDebugHexa;i++)
		{
			if(i==ch_idx)continue;		
			if(ch[ch_idx].child_side[pair[0]]==ch[i].child_side[pair[1]])
			{
				ch[ch_idx].adjacent_hexa[j]=&ch[i];
				break;
			}
		}
	}
}
/*!
 * @brief CatanPointの隣接CPを検索
 * @param [cp_idx] CatanPointのインデックス
 */
static void IN_setCP_AdjacentCP(int cp_idx)
{
	int p_idx=0;
	//for(int i=0;i<3;i++){
	for(int j=0;j<sizeDebugSide;j++)
	{
		if(&cp[cp_idx]==cs[dcs[j]].p1)
		{
			cp[cp_idx].adjacent_point[p_idx++]=cs[dcs[j]].p2;
		}
		else if(&cp[cp_idx]==cs[dcs[j]].p2)
		{
			cp[cp_idx].adjacent_point[p_idx++]=cs[dcs[j]].p1;
		}
		if(p_idx==3)break;
	}
	//}
}

/*!
 * @brief CatanSideの隣接CSを検索
 * @param [cs_idx] CatanSideのインデックス
 */
static void IN_setCS_AdjacentCS(int cs_idx)
{
	/* p1側の隣接CSを検索 */
	int p_idx=0;
	for(int i=0;i<sizeDebugSide;i++)
	{
		if(i==cs_idx)continue;
		if(cs[cs_idx].p1==cs[dcs[i]].p1 || cs[cs_idx].p1==cs[dcs[i]].p2)
		{
			cs[cs_idx].adjacent_side[p_idx++]=&cs[dcs[i]];
		}
		if(p_idx==2)break;
	}
	/* p2側の隣接CSを検索 */
	for(int i=0;i<sizeDebugSide;i++)
	{
		if(i==cs_idx)continue;
		if(cs[cs_idx].p2==cs[dcs[i]].p1 || cs[cs_idx].p2==cs[dcs[i]].p2)
		{
			cs[cs_idx].adjacent_side[p_idx++]=&cs[dcs[i]];
		}
		if(p_idx==4)break;
	}
}

void IN_GetIndex_ChildCPofCS(int cs_idx,int &cp_idx1,int &cp_idx2)
{
	cp_idx1=cs[cs_idx].p1->id;
	cp_idx2=cs[cs_idx].p2->id;

}
void IN_GetIndex_ChildCPofCH(int ch_idx,int &cp_idx1,int &cp_idx2)
{
	cp_idx1=ch[ch_idx].child_pt[0]->id;
	cp_idx2=ch[ch_idx].child_pt[3]->id;

}
 /*!
 * @brief マップ上の盗賊の移動・描画。資源の移動も行う
 * @param [] 
 */
bool Map_MoveThief(int player,int ch_idx,int enemy_player)
{
	CatanHexagon *tmp_prev_ch=&ch[cur_CH_OF_THIEF],*tmp_cur_ch=&ch[ch_idx];
	ch[cur_CH_OF_THIEF].isThiefThere=false;
	ch[ch_idx].isThiefThere=true;
#if DEBUG_DRAW_CH_CONTENT
	Dsp_DrawCHContent(tmp_prev_ch->id,tmp_prev_ch->rscType,tmp_prev_ch->numHexa,tmp_prev_ch->isThiefThere);
	//cur_CH_OF_THIEF=ch_idx;
	Dsp_DrawCHContent(tmp_cur_ch->id,tmp_cur_ch->rscType,tmp_cur_ch->numHexa,tmp_cur_ch->isThiefThere);
#else
	Rsc_MoveResource(player,enemy_player);
	Dsp_ClearMap();
	Map_DrawMap();
	Dsp_UpdateMainWindow();
#endif
	cur_CH_OF_THIEF=ch_idx;
	return true;
}

void Map_BuildHouse(int player,int cp_idx){
	//CatanPoint cp_tmp=cp[cp_idx];
	cp[cp_idx].development=TYPE_DEV_HOUSE;
	cp[cp_idx].owner=player;
}
/*!
 * @brief 初期配置で建てる家の位置を記憶する。
 * @param [numTurn] 初期配置のターン(0/1)
 * @param [player] 現在のプレイヤー
 * @param [cp_idx] 家を建てた場所
 */
void Map_SetFirstSecondHouse(int numTurn,int player,int cp_idx)
{
		if(numTurn==0)
	{
		FirstHouse[player]=cp_idx;
	}
	else
	{
		SecondHouse[player]=cp_idx;
	}
}
void Map_BuildRoad(int player, int cs_idx){
	//CatanSide cs_tmp=cs[cs_idx];
	cs[cs_idx].isRoadThere=true;
	cs[cs_idx].owner=player;
}
void Map_BuildFactory(int cp_idx){
	cp[cp_idx].development=TYPE_DEV_FACTORY;
}
int Map_GetLongestRoadPlayer(void)
{
	return longest_road_player;
}
/*!
 * @brief Mapにおける家を建設する際のエラー条件
 * @param [player] 0-3 プレイヤ
 * @param [cp_idx] CPのID
 * @retval 家を建てられないときtrue
 */
bool Map_IsErrorBuildHouse(int player, int cp_idx)
{
	CatanPoint cp_tmp=cp[cp_idx];
	/* 空き地じゃない */
	if(cp_tmp.development!=TYPE_DEV_END)return true;
	/* 自分の道がひかれていない */
	if(!Map_IsMyRoadAroundCP(player,cp_idx))return true;
	/* 周りに家がある */
	if(Map_IsHouseAroundCP(cp_idx))return true;
	//if(cp_tmp.adjacent_point[0]->isBuildingThere || cp_tmp.adjacent_point[1]->isBuildingThere || cp_tmp.adjacent_point[2]->isBuildingThere ) return true;
	return false;
}
bool Map_IsErrorBuildHouseFirst(int player, int cp_idx)
{
	bool isError=false;
	CatanPoint cp_tmp=cp[cp_idx];
	/* 空き地じゃない */
	if(cp_tmp.development!=TYPE_DEV_END)isError=true;
	/* 周りに家がある */
	if(Map_IsHouseAroundCP(cp_idx))isError=true;
	//if(cp_tmp.adjacent_point[0]->isBuildingThere || cp_tmp.adjacent_point[1]->isBuildingThere || cp_tmp.adjacent_point[2]->isBuildingThere ) return true;
	return isError;
}
/*!
 * @brief Mapにおける工場を建設する際のエラー条件
 * @param [player] 0-3 プレイヤ
 * @param [cp_idx] CPのID
 * @retval 工場を建てられないときtrue
 */
bool Map_IsErrorBuildFactory(int player, int cp_idx)
{
	bool flg = false;
	CatanPoint cp_tmp=cp[cp_idx];
	/* 自分の家が建っていない */
	if(cp_tmp.development != TYPE_DEV_HOUSE || cp_tmp.owner != player)flg = true;
	return flg;
}
/*!
 * @brief Mapにおける道を建設する際のエラー条件
 * @param [player] 0-3 プレイヤ
 * @param [cs_idx] CSのID
 * @retval 道を建てられないときtrue
 */
bool Map_IsErrorBuildRoad(int player, int cs_idx)
{
	bool isError=false;
	CatanSide cs_tmp=cs[cs_idx];
	/* すでに道がある */
	if(cs_tmp.isRoadThere)isError=true;
	/* 周りに自分の道がない */
	bool isMyRoadNearThere=false;
	for(int i=0;i<4;i++)
	{
		if(cs_tmp.adjacent_side[i]->owner==player)isMyRoadNearThere=true;
	}
	if(!isMyRoadNearThere)isError=true;

	return isError;
}
bool Map_IsErrorBuildRoadFirst(int cs_idx,int player,int numHouse)
{
	bool isError=false;
	int cp_idx=numHouse==0?FirstHouse[player]:SecondHouse[player];
	if(cs[cs_idx].p1!=&cp[cp_idx]&&cs[cs_idx].p2!=&cp[cp_idx])isError=true;
	return isError;
}
bool Map_IsHouseThere(int cp_idx)
{
	return cp[cp_idx].development==TYPE_DEV_HOUSE;

}
/*!
 * @brief ユーザが港を持っていないエラーを検出
 * @param [player] 現在のプレイヤ
 * @param [change_num] 資源を差し出す枚数
 * @param [input_rsc_hand] 資源の種類
 */
bool Map_IsErrorExchange(int player,int change_num,int input_rsc_hand)
{
	bool isError=false;
	bool haveHarbor=false;
	if(change_num==3)
	{
		for(int i=TYPE_HRB_31;i<NUM_OF_HARBOR;i++)
		{
			int cs_idx=CsIdxLst_Of_Harbor[i];
			CatanPoint *tmp_cp=cs[cs_idx].p1;
			if(tmp_cp->development!=TYPE_DEV_END&&tmp_cp->owner==player)haveHarbor=true;
			tmp_cp=cs[cs_idx].p2;
			if(tmp_cp->development!=TYPE_DEV_END&&tmp_cp->owner==player)haveHarbor=true;
		}
	}
	else if(change_num==2)
	{
		int cs_idx=CsIdxLst_Of_Harbor[input_rsc_hand];
		CatanPoint *tmp_cp=cs[cs_idx].p1;
		if(tmp_cp->development!=TYPE_DEV_END&&tmp_cp->owner==player)haveHarbor=true;
		tmp_cp=cs[cs_idx].p2;
		if(tmp_cp->development!=TYPE_DEV_END&&tmp_cp->owner==player)haveHarbor=true;
	}else{
		haveHarbor=true;
	}
	return !haveHarbor;
}
/*!
 * @brief 盗賊を移動させるときのエラー条件
 * @param [ch_idx] 移動先CH
 * @param [enemy_player] 敵プレイヤ
 */
bool Map_IsErrorMoveThief(int player,int ch_idx, int enemy_player)
{
	/* 入力のエラー判断 */
	if(ch_idx==NO_OBJ_ID||enemy_player==NO_PLAYER)return true;
	bool isError=false;
	/* 現在のマス */
	if(ch[ch_idx].isThiefThere)isError=true;
	bool isEnemyThere=false;
	bool isDevelopmentThere=false;
	for(int i=0;i<6;i++){
		if(ch[ch_idx].child_pt[i]->development!=TYPE_DEV_END)isDevelopmentThere=true;
		if(ch[ch_idx].child_pt[i]->owner==enemy_player)isEnemyThere=true;
	}
	if(!isEnemyThere)isError=true;
	if(!isDevelopmentThere)isError=true;/* 冗長・建物があるか */
	if(ch_idx==DESERT_CH)isError=true;/* 中央 */
	return isError;
}


/*!
 * @brief 自分の道がひかれている
 * @param [] 
 */
static bool Map_IsMyRoadAroundCP(int player, int cp_idx)
{
	for(int i=0;i<3;i++)
	{
		if(cp[cp_idx].parent_side[i]==NULL)continue;
		if(cp[cp_idx].parent_side[i]->isRoadThere && cp[cp_idx].parent_side[i]->owner == player)return true;
	}
	return false;
}
/*!
 * @brief 周りに家がある
 * @param [] 
 */
static bool Map_IsHouseAroundCP(int cp_idx)
{
	for(int i=0;i<3;i++)
	{
		if(cp[cp_idx].adjacent_point[i]==NULL)continue;
		if(cp[cp_idx].adjacent_point[i]->development!=TYPE_DEV_END)return true;
	}
	return false;
}
void Map_ProduceResourceFirst(void)
{
	for(int player=0;player<NUM_OF_PLAYERS;player++)
	{
		CatanPoint *cp_tmp=&cp[SecondHouse[player]];
		for(int i=0;i<3;i++)
		{
			CatanHexagon *ch_tmp=cp_tmp->parent_hexa[i];
			if(ch_tmp==NULL)continue;
			Map_ProduceResourceCPCH(ch_tmp,cp_tmp);
		}
	}
}
/*!
 * @brief ダイスの目にしたがって資源を振り分ける（生産する）
 * @param [numDice] ダイスの目 
 */
void Map_ProduceResource(int numDice){
	for(int h_idx=0;h_idx<NUM_OF_HEXAGONS;h_idx++)
	{
		if(!ch[h_idx].isThiefThere && /* 盗賊がいない */
			ch[h_idx].rscType!=TYPE_RSC_END &&/* 中央マス以外 */
			ch[h_idx].numHexa == numDice )/* ダイスの目が一緒 */
		{
			Map_ProduceResourceChildCP(h_idx);
		}
	}//for CH
}
/*!
 * @brief CHの子CPに資源を振り分ける
 * @param [ch_idx] CHのインデックス
 */
static void Map_ProduceResourceChildCP(int ch_idx)
{
	for(int p_idx=0;p_idx<6;p_idx++)
	{
		Map_ProduceResourceCPCH(&ch[ch_idx],ch[ch_idx].child_pt[p_idx]);
	}//for 子CP
}
/*!
 * @brief 資源を生産する
 * @param [parent_ch] 資源情報を持つ親CH 
 * @param [child_cp] 建物情報を持つ子CP
 */
static void Map_ProduceResourceCPCH(CatanHexagon *parent_ch,CatanPoint *child_cp)
{
	if(child_cp->development==TYPE_DEV_HOUSE)
	{
			Rsc_MasterToHand(child_cp->owner,parent_ch->rscType,1);
	}
	else if(child_cp->development==TYPE_DEV_FACTORY)
	{
			Rsc_MasterToHand(child_cp->owner,parent_ch->rscType,2);
	}
}
void Map_GetInfo_Dummy(int player,string *val)
{


}
/*!
 * @brief CSがLONGEST ROADの端・端点になっているかどうか判定する。
 * @param [cs_idx] 注目CS
 * @param [cp_idx] 注目CSの子CPの一つ
 * @param [player] カレントプレイヤ
 */
static int Map_IsAnchorCS(int cs_idx,int cp_idx,int player)
{
	CatanPoint *cp_tmp=&cp[cp_idx];
	CatanSide *cs_tmp=&cs[cs_idx];
	int type_LR=TYPE_LR_ANCHOR;
	bool isAnchor=true;
	/* 自分の道がない */
	if(!(cs_tmp->isRoadThere)||(cs_tmp->owner!=player))
	{
		isAnchor=false;
		type_LR=TYPE_LR_NOT_MINE;
	}
	/* 建物がないOR自分の建物 */
	else if(cp_tmp->development==TYPE_DEV_END||cp_tmp->owner==player)
	{
		for(int i=0;i<3;i++)
		{
			/* 隣のCS */
			CatanSide *adj_cs = cp_tmp->parent_side[i];
			if(adj_cs==cs_tmp)continue;
			if(adj_cs==NULL)continue;
			/* 隣に道がある	 */
			if(adj_cs->isRoadThere&&adj_cs->owner==player)
			{
				isAnchor=false;
				/* たかだか2Route */
				type_LR= type_LR == TYPE_LR_1ROUTE ? TYPE_LR_2ROUTE : TYPE_LR_1ROUTE;
			}
		}
	}
	//return isAnchor;
	return type_LR;
}
static void Map_CoutCheck(int *check)
{
	for(int i=0;i<NUM_OF_SIDES;i++)
	{
		cout<<i<<":"<<check[i]<<" ";

	}
	cout<<endl;
}
/*!
 * @brief CSの隣になるCSをvectorに追加
 * @param [cs_idx] 注目CS
 * @param [cp_idx] 注目CSの子CPの一つ
 * @param [player] カレントプレイヤ
 */
static int Map_GetAdjacentRoad(int cs_idx,int cp_idx,int player,int *check,vector<int> &vecAdjRoad)
{
	CatanPoint *cp_tmp=&cp[cp_idx];
	CatanSide *cs_tmp=&cs[cs_idx];
	/* 建物がないOR自分の建物 */
	if(cp_tmp->development==TYPE_DEV_END||cp_tmp->owner==player)
	{
		for(int i=0;i<3;i++)
		{
			/* 隣のCS */
			CatanSide *adj_cs = cp_tmp->parent_side[i];
			if(adj_cs==cs_tmp)continue;
			if(adj_cs==NULL)continue;
			/* 隣に道がある&& 未チェック	 */
			Map_CoutCheck(check);
			if(adj_cs->isRoadThere && adj_cs->owner==player && check[adj_cs->id]==0)
			{
				vecAdjRoad.push_back(adj_cs->id);
				/* たかだか2Route */
			}
		}
	}
	return vecAdjRoad.size();
}
int Map_ScanAnchorCS(vector<int> &vecAnchorCS,int player)
{
	for(int i=0;i<NUM_OF_SIDES;i++)
	{
		CatanSide *cs_tmp=&cs[i];
		bool isAnchor=false;
		for(int j=0;j<2;j++)
		{
			int cp_idx=j==0 ? cs_tmp->p1->id : cs_tmp->p2->id;
			if(Map_IsAnchorCS(cs_tmp->id,cp_idx,player)==TYPE_LR_ANCHOR)isAnchor=true;
		}
		if(isAnchor)vecAnchorCS.push_back(cs_tmp->id);
	}
	return vecAnchorCS.size();
}

/*!
 * @brief 与えられた始点に対し距離を計算する。
 * @param [] 
 */
int Map_CalcRouteLength(int cs_idx_start,int cp_idx_start,int player)
{
	int check[NUM_OF_SIDES]={0};
	int length=1;
	int cp_idx_dst=cs[cs_idx_start].p1->id != cp_idx_start ? cs[cs_idx_start].p1->id : cs[cs_idx_start].p2->id;
	CatanPoint *cp_dst=&cp[cp_idx_dst];
	/* 他人の建物がある */
	if(cp_dst->development!=TYPE_DEV_END&&cp_dst->owner!=player)
	{
		return 1;
	}
	check[cs_idx_start]=1;
	return Map_RecLength(cs_idx_start,cp_idx_dst,player,check,length);
}
//(CS) -> (CP) -> ?
/*!
 * @brief 再帰関数。端点に到達したら辿った経路の長さを返す。チェック済みかどうかも見る。
 * @param [cs_idx] 注目CPに来る時に通ったCS
 * @param [cp_idx] 注目CP
 * @param [player] 現在のプレイヤ
 * @param [check] チェック済みのCS情報（分岐ごとにコピー）
 * @param [length] 経路の長さ(cs_idx含む)
 */ 
static int Map_RecLength(int cs_idx,int cp_idx,int player,int *check,int length)
{
	CatanSide *cs_tmp=&cs[cs_idx];
	CatanPoint *cp_tmp=&cp[cp_idx];
	int type_LR=Map_IsAnchorCS(cs_idx,cp_idx,player);
	vector<int> adjCS;
	Map_GetAdjacentRoad(cs_idx,cp_idx,player,check,adjCS);
	if(adjCS.size()==0)
	{
		return length;
	}
	else if(adjCS.size()==1)
	{
		CatanSide *cs_dst = &cs[adjCS[0]];
		CatanPoint *cp_dst = cs_dst->p1 == cp_tmp ? cs_dst->p2 : cs_dst->p1;
		check[cs_dst->id]=1;
		return Map_RecLength(cs_dst->id,cp_dst->id,player,check,length+1);
	}
	else if(adjCS.size()==2)
	{
		int tmp_length=0,tmp_length2;
		for(int i=0;i<2;i++)
		{
			/* 隣のCPに移る */
			CatanSide *cs_dst = &cs[adjCS[i]];
			CatanPoint *cp_dst = cs_dst->p1 == cp_tmp ? cs_dst->p2 : cs_dst->p1;
			if(i==0)
			{
				/* 一回目 */
				int *check2=(int*)malloc(sizeof(int)*NUM_OF_SIDES);
				memcpy(check2,check,sizeof(int)*NUM_OF_SIDES);
				check2[cs_dst->id]=1;
				tmp_length2=Map_RecLength(cs_dst->id,cp_dst->id,player,check2,length+1);
				free(check2);
			}
			else
			{
				/* 二回目 */
				check[cs_dst->id]=1;
				tmp_length2=Map_RecLength(cs_dst->id,cp_dst->id,player,check,length+1);
			}
			tmp_length = tmp_length < tmp_length2 ? tmp_length2 : tmp_length;
		}//for
		return tmp_length;
	}/* 隣の道の数 */
}/*  */
/*!
 * @brief LongestRoadを計算する。
 * @param [player] 対象プレイヤ
 */
int Map_CalcLongestRoad(int player)
{
	vector<int> vecAnchorCS;
	Map_ScanAnchorCS(vecAnchorCS,player);
	int length=0;
	for(vector<int>::iterator it=vecAnchorCS.begin();it!=vecAnchorCS.end();it++)
	{
		//int cs_idx=*it;
		CatanSide *cs_tmp=&cs[*it];
		for(int j=0;j<2;j++)
		{
			int cp_idx=j==0 ? cs_tmp->p1->id : cs_tmp->p2->id;
			if(Map_IsAnchorCS(*it,cp_idx,player)==TYPE_LR_ANCHOR)
			{
				int tmp_length=Map_CalcRouteLength(*it,cp_idx,player);
				length = tmp_length > length ? tmp_length : length;
			}
		}
	}
	/* 環状のルートについて計算する。 */
	return length;
}
/*!
 * @brief CHの描画関数にMAPが持ってる情報を渡す
 * @param [ch_idx] 描画対象のCH
 */
static void Map_DrawCHContent(int ch_idx)
{
	Dsp_DrawCHContent(ch_idx,ch[ch_idx].rscType,ch[ch_idx].numHexa,ch[ch_idx].isThiefThere);
}
void Map_DrawRoad(int cs_idx)
{
	Dsp_DrawRoad(cs_idx,cs[cs_idx].isRoadThere,cs[cs_idx].owner,cs[cs_idx].type_hrb);
	Map_DrawHouseFactory(cs[cs_idx].p1->id);
	Map_DrawHouseFactory(cs[cs_idx].p2->id);
}
void Map_DrawHouseFactory(int cp_idx)
{
	Dsp_DrawHouseFactory(cp_idx,cp[cp_idx].owner,cp[cp_idx].development);
}
void Map_TestCase1ForDev_BuildHouse(){
	cp[9].development=TYPE_DEV_HOUSE;
	cp[9].owner=0;
	cs[10].isRoadThere=true;
	cs[10].owner=0;
	cs[6].isRoadThere=true;
	cs[6].owner=0;
	cs[14].isRoadThere=true;
	cs[14].owner=1;

}


void Map_TestCase1ForCrd_Soldier(){
	cp[9].development=TYPE_DEV_HOUSE;
	cp[9].owner=0;
	cs[10].isRoadThere=true;
	cs[10].owner=0;
	cs[6].isRoadThere=true;
	cs[6].owner=0;
	cs[14].isRoadThere=true;
	cs[14].owner=1;
}

void Map_TestCase1ForLongestRoad()
{
	cp[9].development=TYPE_DEV_HOUSE;
	cp[9].owner=0;
	cs[10].isRoadThere=true;
	cs[10].owner=0;
	cs[15].isRoadThere=true;
	cs[15].owner=0;
cs[19].isRoadThere=true;
	cs[19].owner=0;
	cs[16].isRoadThere=true;
	cs[16].owner=0;

}
void Map_TestCase2ForLongestRoad()
{
	cp[9].development=TYPE_DEV_HOUSE;
	cp[9].owner=0;
	cs[10].isRoadThere=true;
	cs[10].owner=0;
	cs[15].isRoadThere=true;
	cs[15].owner=0;
cs[19].isRoadThere=true;
	cs[19].owner=0;
	cs[16].isRoadThere=true;
	cs[16].owner=0;
cs[8].isRoadThere=true;
	cs[8].owner=0;
cs[4].isRoadThere=true;
	cs[4].owner=0;
	cs[7].isRoadThere=true;
	cs[7].owner=0;

}