/*!
 * @file Error.cpp
 * @brief ÉGÉâÅ[èåèÇêßå‰
 * @author semitsu
 * @date 12.10.25
 */
#include "Error.h"
#include "Development.h"
#include "Act.h"
#include "Map.h"
//bool Err_IsErrorBuildHouseFirst(int cur_player,int cp_idx)
//{
//
//
//}
bool Err_IsErrorBuildRoad1(int cur_player)
{
	bool isError=false;
	if(Dev_IsErrorBuildRoad_(cur_player))isError=true;
	if(Rsc_IsErrorBuildRoad(cur_player))isError=true;
	return isError;
}
bool Err_IsErrorBuildRoad2(int cur_player,int cs_idx)
{
	bool isError=false;
	if(Map_IsErrorBuildRoad(cur_player,cs_idx))isError=true;
	return isError;
}
bool Err_IsErrorBuildRoadCrd(int cur_player,int cs_idx)
{
	bool isError=false;
	if(Map_IsErrorBuildRoad(cur_player,cs_idx))isError=true;
	if(Dev_IsErrorBuildRoad_(cur_player))isError=true;
	return isError;
}

bool Err_IsErrorBuildFactory1(int cur_player)
{
	bool isError=false;
	if(Rsc_IsErrorBuildFactory(cur_player))isError=true;
	//if(Dev_IsErrorBuildFactory(cur_player))isError=true;
	return isError;
}
bool Err_IsErrorBuildFactory2(int cur_player,int cp_idx)
{
	bool isError=false;
	if(Map_IsErrorBuildRoad(cur_player,cp_idx))isError=true;
	return isError;
}