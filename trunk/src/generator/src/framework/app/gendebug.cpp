/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "gendebug.h"

using namespace ft;

int GenDebug::TIMELINE = 0;
int GenDebug::ACTION = 0;
int GenDebug::AI = 0;
int GenDebug::MOTION  = 0;
int GenDebug::CONTROL = 0;
int GenDebug::FOOTSTEP = 0;

void GenDebug::LoadLevelsFromConfig()
{
    TIMELINE = ReadLevelFromConfig("DEBUG_TIMELINE");
    ACTION = ReadLevelFromConfig("DEBUG_ACTION");
    AI = ReadLevelFromConfig("DEBUG_AI");
    MOTION = ReadLevelFromConfig("DEBUG_MOTION");
    CONTROL = ReadLevelFromConfig("DEBUG_CONTROL");
    FOOTSTEP = ReadLevelFromConfig("DEBUG_FOOTSTEP");
}