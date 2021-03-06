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

		// 画像を初期化
	IN_InitMap();
	IN_InitMapContents();
	IN_InitSetDispMap();
	Map_DrawMap();
	IN_InitAct_Client();
	/* いろんな初期化を済ませておく */
	IN_InitNetC();
	/* ゲーム開始 */
	//Net_Connect();
	NetC_RecieveLoop();
	return 0;
}