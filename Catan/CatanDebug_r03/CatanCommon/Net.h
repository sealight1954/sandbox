/*!
 * @file W.cpp
 * @brief 
 * @author semitsu
 * @date 
 */
#ifndef __NET_H
#define __NET_H
//common.hとかで定義
//#define NUM_OF_PLAYERS 2
#include <winsock2.h>
#include <stdio.h>
#include "common.h"

/* パケットのヘッダ関係の情報　const char とかとdefineが混ざっている。。2013.02.23 */
const char packetHeaderSize=2;
#define PACKET_MODE_IDX (1)
#define PACKET_SIZE_IDX (0)

const u_short port=11111;
//const char server_ip_dest[]="192.168.11.6";
const enum SocketError
{
	SOCKET_NO_ERROR=0,
	SOCKET_BIND_ERROR=-1,
	SOCKET_LISTEN_ERROR=-2,
	SOCKET_ACCEPT_ERROR=-3,
	SOCKET_SEND_ERROR=-4
};

/* パケットの種類 */
enum PacketType {
	/* 正常パケット */
	PACKET_TYPE_VALID,
	/* 無視パケット */
	PACKET_TYPE_UNKNOWN,
	/* 異常パケット */
	PACKET_TYPE_ERROR,
	/* 状態違い */
	PACKET_GRAMMER_ERROR
	/* 入力待ち違い */
	//PACKET_TYPE_WRONG_INPUT
};

/* Net_Client.cpp内のNet_ProcessPacketのM_PacketModeとリンク */
enum PacketMode {
	PACKET_MODE_INPUT,
	PACKET_MODE_START_MESSAGE,
	PACKET_MODE_EMPTY
};
enum PacketType Net_ProcessPacket(const char *BT_PacketData, char C_PacketBytes);
int Net_SendInputPacket(CatanInput input,SOCKET *s);
int Net_SendEmptyPacket(SOCKET *s);
int Net_SendStartMessagePacket(SOCKET *s);

//enum 
#define DPRINTF printf
#endif