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

		// �摜��������
	IN_InitMap();
	IN_InitMapContents();
	IN_InitSetDispMap();
	Map_DrawMap();
	IN_InitAct_Client();
	/* �����ȏ��������ς܂��Ă��� */
	IN_InitNetC();
	/* �Q�[���J�n */
	//Net_Connect();
	NetC_RecieveLoop();
	return 0;
}