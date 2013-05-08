/*!
 * @file Error.h
 * @brief ÉGÉâÅ[èåè summarize
 * @author semitsu
 * @date 12.10.25
 */
#ifndef __ERROR_H
#define __ERROR_H
bool Err_IsErrorBuildRoad1(int cur_player);
bool Err_IsErrorBuildRoad2(int cur_player,int cs_idx);
bool Err_IsErrorBuildRoadCrd(int cur_player,int cs_idx);
bool Err_IsErrorBuildFactory1(int cur_player);
bool Err_IsErrorBuildFactory2(int cur_player,int cp_idx);

#endif //__ERROR_H