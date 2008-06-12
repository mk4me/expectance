/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "gendebug.h"
#include "evolution/evodbg.h"

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
	EvoDBG::setTimelineLevel(TIMELINE);
    ACTION = ReadLevelFromConfig("DEBUG_ACTION");
	EvoDBG::setActionLevel(ACTION);
    AI = ReadLevelFromConfig("DEBUG_AI");
	EvoDBG::setAILevel(AI);
    MOTION = ReadLevelFromConfig("DEBUG_MOTION");
	EvoDBG::setMotionLevel(MOTION);
    CONTROL = ReadLevelFromConfig("DEBUG_CONTROL");
    FOOTSTEP = ReadLevelFromConfig("DEBUG_FOOTSTEP");
}
