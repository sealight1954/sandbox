/*!
 * @file CatanClient main.cpp
 * @brief 
 * @author semitsu
 * @date 2013.02.15
 */
#include "common.h"
#include "Map.h"
#include "Disp.h"
#include "Net_Client.h"
#include "Act_Client.h"
int main( int argc, char *argv[] ){
	//IN_InitNetS();

		// ‰æ‘œ‚ğ‰Šú‰»
	IN_InitMap();
	IN_InitMapContents();
	IN_InitSetDispMap();
	Map_DrawMap();
	IN_InitAct_Client();
	/* ‚¢‚ë‚ñ‚È‰Šú‰»‚ğÏ‚Ü‚¹‚Ä‚¨‚­ */
	IN_InitNetC();
	/* ƒQ[ƒ€ŠJn */
	//Net_Connect();
	NetC_RecieveLoop();
	return 0;
}