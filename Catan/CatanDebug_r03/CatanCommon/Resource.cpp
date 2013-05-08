/*!
* @file Resouce.cpp
* @brief ���Y�����̊Ǘ�
* @author semitsu
* @date 12.10.01
*/
#include "Resource.h"
/* random */
#include "Act.h"
#include <iostream>
/* --�ϐ��錾-------------------------------------------------------------- */

/*�����̎�� enum�^�Ő�����*/
static int MasterResource[TYPE_RSC_END]={NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE};    /* type  NUM_OF_EACH_RESOURCE�ɏ����������H */
static int prev_MasterResource[TYPE_RSC_END]={NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE,NUM_OF_EACH_RESOURCE};    /* type  NUM_OF_EACH_RESOURCE�ɏ����������H */
 //static int PlayerResource[NUM_OF_PLAYERS][TYPE_RSC_END]={{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}}; /* player * type */
#if DEBUG_GAME
static int PlayerResource[NUM_OF_PLAYERS][TYPE_RSC_END]={{1,1,1,2,4},{1,1,1,5,5},{1,1,1,1,1},{1,1,1,1,1}}; /* player * type */
#else
static int PlayerResource[NUM_OF_PLAYERS][TYPE_RSC_END]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}; /* player * type */
#endif
static int prev_PlayerResource[NUM_OF_PLAYERS][TYPE_RSC_END]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}; /* player * type */
/* �K�v�Ȏ����̎�ނƐ� */
static int tblRscTypeForHouse[]={TYPE_RSC_LUMBER,TYPE_RSC_BRICK,TYPE_RSC_WOOL,TYPE_RSC_GRAIN};
static int tblRscCostForHouse[]={1,1,1,1};
static const int RscSizeHouse=4;
static int tblRscTypeForRoad[]={TYPE_RSC_LUMBER,TYPE_RSC_BRICK};
static int tblRscCostForRoad[]={1,1};
static const int RscSizeRoad=2;
static int tblRscTypeForFactory[]={TYPE_RSC_GRAIN,TYPE_RSC_ORE};
static int tblRscCostForFactory[]={2,3};
static const int RscSizeFactory=2;
static int tblRscTypeForDevCard[]={TYPE_RSC_WOOL,TYPE_RSC_GRAIN,TYPE_RSC_ORE};
static int tblRscCostForDevCard[]={1,1,1};
static const int RscSizeDevCard=3;
static int tblRscCostForExchange[3][2]={{2,1},{3,1},{4,1}};
static int tblRscCostFor31Exchange[]={3,1};
static int tblRscCostFor41Exchange[]={4,1};
static const int RscSizeExchange=2;

/* --�֐��錾-------------------------------------------------------------- */
static void Rsc_CostResourceOnHand(int player,int *type,int *cost,const int size);
static bool Rsc_IsErrorResourceOnHand(int player,int *type,int *cost,const int size);
static void Rsc_AddResourceMaster( int *type,int *cost,const int size);
static void Rsc_HandToHand(int player,int enemy,int rscType);
//static void Rsc_MasterToHand(int player,int rscType);
//static void Rsc_HandToMaster(int player,int rscType);

/* --�֐��L�q-------------------------------------------------------------- */

void Rsc_SaveState(void)
{
	memcpy(prev_MasterResource,MasterResource,sizeof(int)*TYPE_RSC_END);
	memcpy(prev_PlayerResource,PlayerResource,sizeof(int)*NUM_OF_PLAYERS*TYPE_RSC_END);
}

void Rsc_LoadState(void)
{
	memcpy(MasterResource,prev_MasterResource,sizeof(int)*TYPE_RSC_END);
	memcpy(PlayerResource,prev_PlayerResource,sizeof(int)*NUM_OF_PLAYERS*TYPE_RSC_END);
}
void Rsc_MoveResource(int dst_player,int src_player)
{
	int sum=Rsc_GetSumOfRscInHand(src_player);
	/* ��D��vector�ł���킷 */
	//vector<int> vecCrd;
	//int idx=0;
	//for(int i=0;i<TYPE_RSC_END;i++)
	//	for(int j=0;j<PlayerResource[src_player][i];j++)
	//		vecCrd.push_back(i);
	// /* check */
	//if(vecCrd.size()!=sum)std::cout<<"error RscInHandNum"<<endl;
	//int rand_num=Act_Rand(sum-1);
	
	if(sum!=0)
	{
		int cat=Act_RandCategory(PlayerResource[src_player],5);
		Rsc_HandToHand(dst_player,src_player,cat);
	}
}
/*!
* @brief �����̐U�蕪���i���Y�j���s��
* @param [player]  �������l������v���C���[
* @param [rscType] �l�����鎑���̎��
* @param [num] �l�����鎑���̗�
*/
void Rsc_MasterToHand(int player, int rscType, int num)
{
	/* �R�D�Ɏ������Ȃ��ꍇ�A�S���̎�������� */
	for(int i=0;i<num;i++)
	{
		if(MasterResource[rscType]==0)
		{
			for(int i=0;i<NUM_OF_PLAYERS;i++)
			{
				MasterResource[rscType]+=PlayerResource[i][rscType];
				PlayerResource[i][rscType]=0;
			}
			break;
		}
		/* ������U�蕪���� */
		MasterResource[rscType]--;
		PlayerResource[player][rscType]++;
	}
}
static void Rsc_HandToHand(int player,int enemy,int rscType)
{
	PlayerResource[player][rscType]++;
	PlayerResource[enemy][rscType]--;

}
/*!
* @brief �A�N�V�����ɔ��������̏�����s��
* @param [player] �v���C��
* @param [type] ����鎑���̎�ނ̔z��
* @param [cost] �����̏���ʂ̔z��
* @param [size] �����̎�ސ� 
*/
void Rsc_ExchangeDevCard(int player)
{
	Rsc_CostResourceOnHand(player,tblRscTypeForDevCard,tblRscCostForDevCard,RscSizeDevCard);
}
/*!
 * @brief ��������������
 * @param [player] ���݂̃v���C��
 * @param [numRscHand] ����鎑���̖���
 * @param [type_hand_rsc] �����o�������̎��
 * @param [type_master_rsc] �l�����鎑���̎��
 */
void Rsc_Exchange(int player,int numRscHand,int type_hand_rsc,int type_master_rsc)
{
	//static int type_tbl[]={type_hand_rsc,type_master_rsc};
	//static int getNumTblPlayer[]={numRscHand,1};
	//static int getNumTblMaster[]={1,numRscHand};
	//Rsc_CostResourceOnHand(player,tblRscCostForExchange[numRscHand,tblRscCostForRoad,RscSizeRoad);
	PlayerResource[player][type_hand_rsc]-=/*PlayerResource[player][type_hand_rsc]-*/numRscHand;
	PlayerResource[player][type_master_rsc]++;
	MasterResource[type_master_rsc]--;
	MasterResource[type_hand_rsc]+=numRscHand;
}

/*!
 * @brief �v���C���ƃv���C���ł̌����B����������true,���Ȃ�������false
 * @param [player1] 
 * @param [player2] 
 * @param [rscOut1] player1�������o�������̔z��i�S��ޕ��j
 * @param [rscOut2] player2�������o�������̔z��i�S��ޕ��j
 */
bool Rsc_Exchange_1_1(int player1,int player2 , int* rscOut1,int* rscOut2)
{
int rscTable[]={TYPE_RSC_LUMBER,TYPE_RSC_BRICK,TYPE_RSC_WOOL,TYPE_RSC_GRAIN,TYPE_RSC_ORE};
			if(Rsc_IsErrorResourceOnHand(player1,rscTable,rscOut1,TYPE_RSC_END)||
				Rsc_IsErrorResourceOnHand(player2,rscTable,rscOut2,TYPE_RSC_END))
			{
				return false;
			}
			else
			{
				/* input.player��rscIn�������Ă� && cur_player��rscOut�����Ă� */
			}
}

void Rsc_BuildRoad(int player){
	Rsc_CostResourceOnHand(player,tblRscTypeForRoad,tblRscCostForRoad,RscSizeRoad);
	Rsc_AddResourceMaster(tblRscTypeForRoad,tblRscCostForRoad,RscSizeRoad);
}
void Rsc_BuildFactory(int player){
	Rsc_CostResourceOnHand(player,tblRscTypeForFactory,tblRscCostForFactory,RscSizeFactory);
	Rsc_AddResourceMaster(tblRscTypeForFactory,tblRscCostForFactory,RscSizeFactory);
}
void Rsc_BuildHouse(int player){
	Rsc_CostResourceOnHand(player,tblRscTypeForHouse,tblRscCostForHouse,RscSizeHouse);
	Rsc_AddResourceMaster(tblRscTypeForHouse,tblRscCostForHouse,RscSizeHouse);
}
/*!
 * @brief �Ɛ�J�[�h
 * @param [player] �g�p��
 * @param [rscType] �Ɛ肷�鎑���̎��
 */
bool Rsc_Monopoly(int player, int rscType)	
{
	int amount=0;
	for(int i=0;i<NUM_OF_PLAYERS;i++)
	{
		if(i==player)continue;
		int tmp=PlayerResource[i][rscType];
		amount+=tmp;
		//Rsc_CostResourceOnHand(i,&rscType,&tmp,1);
		PlayerResource[i][rscType]=0;
	}	
	PlayerResource[player][rscType]+=amount;
	return true;
}
bool Rsc_IsErrorDoHarvest(int player,int rscType0,int rscType1)
{
	bool isError=false;
	if(rscType0==-1||rscType1==-1)isError=true;
	if(MasterResource[rscType0]==0)isError=true;
	if(MasterResource[rscType1]==0)isError=true;
	if(rscType0==rscType1&&MasterResource[rscType0]<2)isError=true;
	return isError;
}
bool Rsc_IsErrorDoHarvest1(int player,int rscType)
{
	bool isError=false;
	return MasterResource[rscType]==0;
}
bool Rsc_DoHarvest(int player,int rscType0,int rscType1)
{
	bool doHarvest=true;
	doHarvest=!Rsc_IsErrorDoHarvest(player,rscType0,rscType1);
	if(doHarvest){
		Rsc_MasterToHand(player,rscType0);
		Rsc_MasterToHand(player,rscType1);
	}
	return doHarvest;
}
bool Rsc_DoHarvest1(int player,int rscType)
{
	bool doHarvest=true;
	doHarvest=!Rsc_IsErrorDoHarvest1(player,rscType);
	if(doHarvest){
		Rsc_MasterToHand(player,rscType);
	}
	return doHarvest;
}

static void Rsc_CostResourceOnHand(int player, int *type,int *cost,const int size)
{
	for(int i=0;i<size;i++)
	{
		PlayerResource[player][type[i]] -= cost[i];
	}
}
static void Rsc_AddResourceMaster( int *type,int *cost,const int size)
{
	for(int i=0;i<size;i++)
	{
		MasterResource[type[i]] += cost[i];
	}

}
void Rsc_HandToMaster(int player,int rscType)
{
	PlayerResource[player][rscType]--;
	MasterResource[rscType]++;
}
//static void Rsc_MasterToHand(int player,int rscType)
//{
//	PlayerResource[player][rscType]++;
//	MasterResource[rscType]--;
//
//}

/*!
* @brief �Ƃ����Ă�ۂ̎����G���[����
* @param [player] 0-3 �v���C�� 
* �莝�������Ɋւ��ăG���[������Ƃ�true
*/
bool Rsc_IsErrorBuildHouse(int player)
{
	/* ����������Ȃ� */
	if(Rsc_IsErrorResourceOnHand(player,tblRscTypeForHouse,tblRscCostForHouse,RscSizeHouse))return true;
	return false;
}
bool Rsc_IsErrorBuildFactory(int player)
{
	if(Rsc_IsErrorResourceOnHand(player,tblRscTypeForFactory,tblRscCostForFactory,RscSizeFactory))return true;
	return false;
}
bool Rsc_IsErrorBuildRoad(int player)
{
	if(Rsc_IsErrorResourceOnHand(player,tblRscTypeForRoad,tblRscCostForRoad,RscSizeRoad))return true;
	return false;
}
bool Rsc_IsErrorExchangeDevCard(int player)
{
	
	if(Rsc_IsErrorResourceOnHand(player,tblRscTypeForDevCard,tblRscCostForDevCard,RscSizeDevCard))return true;
	return false;
}
bool Rsc_IsErrorExchange(int player,int numRscHand,int type_rsc_hand,int type_rsc_master)
{
	bool isError=false;
	if(PlayerResource[player][type_rsc_hand]<numRscHand ||
		(MasterResource[type_rsc_master]==0))
		isError=true;
	return isError;
}
/*!
* @brief �v���C���̎���������ĂȂ��G���[���o
* @param [player] �v���C�� 
* @param [*type] �����̎�ނ̔z�� 
* @param [*cost] ���ꂼ��̎����̕K�v�� 
* @param [size] �����̎�ސ� 
*/
static bool Rsc_IsErrorResourceOnHand(int player, int *type_tbl,int *cost,const int size)
{
	for(int i=0;i<size;i++)
	{
		if(PlayerResource[player][type_tbl[i]] < cost[i]) return true;
	}
	return false;
}
bool Rsc_IsErrorResourceOnHandZero(int player,int rscType)
{
	return PlayerResource[player][rscType]==0;
}
bool Rsc_IsErrorResourceOnMasterZero(int rscType)
{
	return MasterResource[rscType]==0;
}
int Rsc_GetPlayerResourceInfo(int player,int type)
{
	return PlayerResource[player][type];
}
/*!
 * @brief �R�D�̎����̏���Ԃ��B
 * @param [player] ���݂̃v���C�����B�g��Ȃ��B�i�֐��|�C���^�̊֌W�ŃC���^�t�F�[�X�����낦��j
 * @param [val] �������̊i�[��B�Ԃ�l
 */
void Rsc_GetInfo_RscInMaster(int player,string *val)
{
	stringstream stream;
	for(int i=0;i<TYPE_RSC_END;i++)
	{
		stream.str("");
		stream<<MasterResource[i];
		val[i]=stream.str();
	}
}
/*!
 * @brief �v���C���̎����̏���Ԃ��B
 * @param [player] ���݂̃v���C�����B
 * @param [val] �������̊i�[��B�Ԃ�l
 */
void Rsc_GetInfo_RscInHand(int player,string *val)
{
	stringstream stream;
	int *rsc=PlayerResource[player];
	for(int i=0;i<TYPE_RSC_END;i++)
	{
		
		stream.str("");
		stream<<rsc[i];
		//stream<<PlayerResource[player][i];
		val[i]=player!=NO_PLAYER ? stream.str() : "-1";
	}
}
int Rsc_GetSumOfRscInHand(int player)
{
	int sum=0;
	for(int i=0;i<TYPE_RSC_END;i++)
	{
		sum+=PlayerResource[player][i];
	}
	return sum;
}
void Rsc_TestCase1ForBuildHouse(void){
	PlayerResource[0][TYPE_RSC_BRICK]=1;
	PlayerResource[0][TYPE_RSC_LUMBER]=1;
	PlayerResource[0][TYPE_RSC_WOOL]=1;
	PlayerResource[0][TYPE_RSC_GRAIN]=1;
}
void Rsc_TestCase2ForBuildHouse(void)
{
	PlayerResource[0][TYPE_RSC_BRICK]=0;
	PlayerResource[0][TYPE_RSC_LUMBER]=1;
	PlayerResource[0][TYPE_RSC_WOOL]=1;
	PlayerResource[0][TYPE_RSC_GRAIN]=1;
}
void Rsc_TestCase3ForBuildHouse(void){
	PlayerResource[0][TYPE_RSC_BRICK]=1;
	PlayerResource[0][TYPE_RSC_LUMBER]=0;
	PlayerResource[0][TYPE_RSC_WOOL]=1;
	PlayerResource[0][TYPE_RSC_GRAIN]=1;
}
void Rsc_TestCase4ForBuildHouse(void){
	PlayerResource[0][TYPE_RSC_BRICK]=1;
	PlayerResource[0][TYPE_RSC_LUMBER]=1;
	PlayerResource[0][TYPE_RSC_WOOL]=0;
	PlayerResource[0][TYPE_RSC_GRAIN]=1;
}
void Rsc_TestCase5ForBuildHouse(void){
	PlayerResource[0][TYPE_RSC_BRICK]=1;
	PlayerResource[0][TYPE_RSC_LUMBER]=1;
	PlayerResource[0][TYPE_RSC_WOOL]=1;
	PlayerResource[0][TYPE_RSC_GRAIN]=0;
}
void Rsc_TestCase1ForBuildRoad(void)
{
	PlayerResource[0][TYPE_RSC_BRICK]=1;
	PlayerResource[0][TYPE_RSC_LUMBER]=1;
}
void Rsc_TestCase2ForBuildRoad(void)
{
	PlayerResource[0][TYPE_RSC_BRICK]=0;
	PlayerResource[0][TYPE_RSC_LUMBER]=1;
}
void Rsc_TestCase3ForBuildRoad(void)
{
	PlayerResource[0][TYPE_RSC_BRICK]=1;
	PlayerResource[0][TYPE_RSC_LUMBER]=0;
}