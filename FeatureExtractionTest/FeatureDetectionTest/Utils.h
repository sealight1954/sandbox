/*!  
 ************************************************************************************* 
 * \file 
 * 
 * \brief 
 *   
 * 
 * \date 
 *    
 *
 * \note
 *    Copyright (C) 2012 MITSUBISHI ELECTRIC CORPORATION ALL RIGHTS RESERVED
 *
 * \author 
 *    - 
 ************************************************************************************* 
 */
#ifndef __UTILS
#define __UTILS
#include <iostream>
#include <string>
#include <stdlib.h>
#include <direct.h>
#include <sys/stat.h>

using namespace std;

void StdSplitpath(const string &_path, string &_dir, string &_fname, string &_ext);
void checkMkdir(const string &_path);

#endif //__UTILS