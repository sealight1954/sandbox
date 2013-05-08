/*!
 * @file Resouce.cpp
 * @brief 
 * @author semitsu
 * @date 
 */
#ifndef __NET_CLIENT_H
#define __NET_CLIENT_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "common.h"
int NetC_SendInputPacket(CatanInput input);
//int NetC_SendInputPacket(CatanInput input)
void IN_InitNetC(void);
int NetC_RecieveLoop(void);

#endif//__NET_CLIENT_H