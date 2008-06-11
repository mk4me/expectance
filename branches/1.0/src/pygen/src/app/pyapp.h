/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_PYAPP_H
#define _FT_PYAPP_H

#include <string>

void runApp();

void setPropStr(const std::string& key, std::string val);
void setPropInt(const std::string& key, int val);
void setPropFloat(const std::string& key, float val);

std::string getPropStr(const std::string& key);
int getPropInt(const std::string& key);
float getPropFloat(const std::string& key);

void dumpProps();

void test();

#endif //_FT_PYAPP_H
