#include <iostream>

#include "common.h"
#include "Map.h"
#include "Disp.h"
#include "Act.h"
#include "Resource.h"
#include "Development.h"
#include "DevCard.h"
int
	main(int argc, char *argv[])
{
	//InitRand();
	// ‰æ‘œ‚ğ‰Šú‰»
	IN_InitMap();
	IN_InitMapContents();
	IN_InitSetDispMap();
	Map_DrawMap();
	IN_InitAct();
#if DEBUG_GAME
	Map_TestCase2ForLongestRoad();
	Map_DrawMap();
	Dsp_UpdateMainWindow_Debug();
	Map_CalcLongestRoad(0);
#endif
	//Dev_BuildHouse(0,0);
	//Dev_BuildRoad(0,0);
	//Crd_Soldier(2,9,3);
	int key;
	while(1) {
		key = cv::waitKey(0);
		// 'Esc'‚ª‰Ÿ‚³‚ê‚½ê‡‚ÉI—¹
		if(key==27) break;
	}
}