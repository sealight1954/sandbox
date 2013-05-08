/*!
 * @file CatanServer main.cpp
 * @brief 
 * @author semitsu
 * @date 2013.02.14
 */
#include "common.h"
#include "Map.h"
#include "Disp.h"
#include "Act_Server.h"
#include "Net_Server.h"
int main( int argc, char *argv[] ){
	/* ‚¢‚ë‚ñ‚È‰Šú‰»‚ğÏ‚Ü‚¹‚Ä‚¨‚­ */
	IN_InitMap();
	IN_InitMapContents();
	IN_InitSetDispMap();
	Map_DrawMap();
	IN_InitAct_Server();
	Dsp_UpdateMainWindow_Debug();
	IN_InitNetS();
	/* ƒQ[ƒ€ŠJn */
	//Net_Connect();
	printf("receive loop start\n");
	NetS_RecieveLoop();
	return 0;
}