#include "common.h"
#include "Net.h"
#include "Act.h"
#include "State.h"

/* static 関数の宣言 */
static bool Net_IsPacketError(const char *BT_PacketData, char C_PacketBytes);
static enum PacketType Net_ProcessInputPacket(const char *packet_data,char packet_size);
static enum PacketType Net_ProcessStartMessagePacket(const char *packet_data,char packet_size);
static enum PacketType Net_ProcessEmptyPacket(const char *packet_data,char packet_size);

static bool Net_IsInputPacketError(int mode_l,int mode_m,CatanInput input);
//static bool Net_IsInputPacketError(const char *packet_data,char packet_size);
static bool Net_IsStartMessagePacketError(const char *packet_data,char packet_size);
static char gbuf[MAX_BUF_SIZE];

/*!
 * @brief パケット処理
 * @param [] 
 */
enum PacketType Net_ProcessPacket(const char *BT_PacketData, char C_PacketBytes)
{
	typedef enum PacketType (*PACKET_PROCESS_FUNCTION)(const char *, char);

	/* Net.hのPacketModeの順番と合うように 2013.02.23 */
	static const PACKET_PROCESS_FUNCTION AT_ProcessFunctions[] = {Net_ProcessInputPacket, Net_ProcessStartMessagePacket, Net_ProcessEmptyPacket/*, Net_ProcessInputPacketImmediately*/};
	/* Net_GeneratePacketHeader */
	char M_PacketMode;

	if (Net_IsPacketError(BT_PacketData, C_PacketBytes)) {
		return PACKET_TYPE_ERROR;
	}
	
	M_PacketMode = (BT_PacketData[PACKET_MODE_IDX]);
	
	if (M_PacketMode >= 0 && M_PacketMode < sizeof(AT_ProcessFunctions) / sizeof(PACKET_PROCESS_FUNCTION)) {
		return AT_ProcessFunctions[M_PacketMode](BT_PacketData + packetHeaderSize, C_PacketBytes - packetHeaderSize);
	}
	return PACKET_TYPE_UNKNOWN;
}

static bool Net_IsPacketError(const char *BT_PacketData, char C_PacketBytes)
{
	unsigned char C_DataBytes;
	
	if (BT_PacketData[PACKET_SIZE_IDX]!=C_PacketBytes)
	{
		return true;
	}
	
	//C_DataBytes = (BT_PacketData[2] - '0') * 10 + (BT_PacketData[3] - '0');
	//if (C_DataBytes > 10) {
	//	return true;
	//}
	
	return false;
}

/*!
 * @brief 入力情報パケットの処理。成功ならtrueを、失敗ならfalseを返す。
 * @param [packet_data] 
 * @param [packet_size] 
 */
static enum PacketType Net_ProcessInputPacket(const char *packet_data,char packet_size)
{
/* header 部分は切り詰めてもらう */
	int tmp_mode_l=packet_data[0];
	int tmp_mode_m=packet_data[1];
	int input_idx=2;
	CatanInput input={
		packet_data[input_idx+0],
		packet_data[input_idx+1],
		packet_data[input_idx+2],
		packet_data[input_idx+3],
		true/* isInput */
	};
	//input.but_num=packet_data[1];
	//input.obj.typeObj=packet_data[2];
	//input.obj.id=packet_data[3];
	if(Net_IsInputPacketError(tmp_mode_l,tmp_mode_m,input))
	{
		return PACKET_GRAMMER_ERROR;
	}
	St_Event(input/*,waitFlg,waitMapFlg*/);

	return PACKET_TYPE_VALID;
}

/*!
 * @brief クライアント側はサーバから来た入力を検査せずに実行せないかんと思ったが、
 *        そうでもないか。inputにプレイヤ情報あるし。2013.02.17
 * @param [] 
 */

//static enum PacketType Net_ProcessInputPacketImmediately(const char *packet_data,char packet_size)
//{
//	int mode_l=packet_data[0];
//	int mode_m=packet_data[1];
//	int input_idx=2;
//	CatanInput input={
//		packet_data[input_idx+0],
//		packet_data[input_idx+1],
//		packet_data[input_idx+2],
//		packet_data[input_idx+3]	
//	};
//	St_Event(input/*,waitFlg,waitMapFlg*/);
//
//	return PACKET_TYPE_VALID;
//}

static bool Net_IsInputPacketError(int mode_l,int mode_m,CatanInput input)
{
	int waitPlayer,cur_mode_l,cur_mode_m;
	waitPlayer=Act_GetWaitPlayer();
	cur_mode_l=St_GetModeL();
	cur_mode_m=St_GetModeM();
	//int inputPlayer=packet_data[0];
	/* 入力待ちのプレイヤでない　かつ　全員待ちでない */
	if(waitPlayer!=input.player&&waitPlayer!=NUM_OF_PLAYERS)
	{
		return true;
	}
	if(cur_mode_l!=mode_l||cur_mode_m!=mode_m)
	{
		return true;
	}
	/* CatanInputの部分も見るべきだが見てない。 */
	return false;
}
/*!
 * @brief Game開始の合図を送る 2013.02.23
 * @param [packet_data] NULL
 * @param [packet_size] 0
 */
static enum PacketType Net_ProcessStartMessagePacket(const char *packet_data,char packet_size)
{
	/* mode_l,mode_mをMODE_L_INにセットする */
	CatanInput input={0};//ダミー
	St_Event(input);
	return PACKET_TYPE_VALID;
}
/*!
 * @brief 何もしない。ProcessStartMessagePacketとはModeのみ違う 2013.02.23
 * @param [packet_data] NULL
 * @param [packet_size] 0
 */
static enum PacketType Net_ProcessEmptyPacket(const char *packet_data,char packet_size)
{
	return PACKET_TYPE_VALID;
}

/*!
 * @brief パケットのヘッダを作る
 * @param [buf] 情報の格納先
 * @param [size] パケットのサイズ(ヘッダ含む)
 * @param [M_PacketMode]
 */
int Net_GeneratePacketHeader(char *buf,char size,char M_PacketMode)
{
	buf[PACKET_SIZE_IDX]=size;
	buf[PACKET_MODE_IDX]=M_PacketMode;
	return packetHeaderSize;//header size

}
int Net_SendPacket(SOCKET *s, const char *buf,int packet_size)
{
	if(SOCKET_ERROR==send(*s, buf, packet_size, 0))
	{
		printf("socket_error send\n");
		//exit(-1);
		return SOCKET_SEND_ERROR;
	}
	//send(*s,buf,6,0);
	return SOCKET_NO_ERROR;
}
/*!
 * @brief 入力情報を送信
 *        データ部 |0|1|2|3|4|5|
 * mode_l-----------| | | | | | 
 * mode_m-------------| | | | | 
 * CatanInput-----------|-----|
 * @param [] 
 */
int Net_SendInputPacket(CatanInput input,SOCKET *s)
{
	//char buf[MAX_BUF_SIZE];
	char packet_size=8;
	int data_idx=Net_GeneratePacketHeader(gbuf,packet_size,PACKET_MODE_INPUT);
	//buf[0]=5;//size
	/* Process_PacketData内のM_PacketModeとリンク */
	//buf[1]=0;
	gbuf[data_idx+0]=St_GetModeL();
	gbuf[data_idx+1]=St_GetModeM();
	gbuf[data_idx+2]=input.player;
	gbuf[data_idx+3]=input.obj.typeObj;
	gbuf[data_idx+4]=input.obj.id;
	gbuf[data_idx+5]=input.but_num;
	DPRINTF("send packet\n");
	return Net_SendPacket(s,gbuf,packet_size);
}

/*!
 * @brief サーバに送った入力が文法エラーだった時用。
 *        なにも処理をしないパケットを返す。
 * @param [] 
 */
int Net_SendEmptyPacket(SOCKET *s)
{
	char packet_size=2;
	int data_idx=Net_GeneratePacketHeader(gbuf,packet_size,PACKET_MODE_EMPTY);
	/*if(SOCKET_ERROR==send(*s, gbuf, packet_size, 0))
	{
		printf("socket_error send\n");
		return SOCKET_SEND_ERROR;
	}
	return SOCKET_NO_ERROR;*/
	return Net_SendPacket(s,gbuf,packet_size);
}

int Net_SendStartMessagePacket(SOCKET *s)
{
	char packet_size=2;
	int data_idx=Net_GeneratePacketHeader(gbuf,packet_size,PACKET_MODE_START_MESSAGE);
	return Net_SendPacket(s,gbuf,packet_size);

}