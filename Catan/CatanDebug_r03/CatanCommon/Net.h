/*!
 * @file W.cpp
 * @brief 
 * @author semitsu
 * @date 
 */
#ifndef __NET_H
#define __NET_H
//common.h�Ƃ��Œ�`
//#define NUM_OF_PLAYERS 2
#include <winsock2.h>
#include <stdio.h>
#include "common.h"

/* �p�P�b�g�̃w�b�_�֌W�̏��@const char �Ƃ���define���������Ă���B�B2013.02.23 */
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

/* �p�P�b�g�̎�� */
enum PacketType {
	/* ����p�P�b�g */
	PACKET_TYPE_VALID,
	/* �����p�P�b�g */
	PACKET_TYPE_UNKNOWN,
	/* �ُ�p�P�b�g */
	PACKET_TYPE_ERROR,
	/* ��ԈႢ */
	PACKET_GRAMMER_ERROR
	/* ���͑҂��Ⴂ */
	//PACKET_TYPE_WRONG_INPUT
};

/* Net_Client.cpp����Net_ProcessPacket��M_PacketMode�ƃ����N */
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