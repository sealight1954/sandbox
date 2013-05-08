
/*!
 * @file Net_Server.h
 * @brief サーバー側の通信機能
 * @author semitsu
 * @date 2013.02.14
 */
#ifndef __NET_SERVER_H
#define __NET_SERVER_H
#include <stdio.h>
#include <winsock2.h>
int NetS_RecieveLoop(void);
//int Net_Connect(void);
int IN_InitNetS(void);
#endif//__NET_SERVER_H