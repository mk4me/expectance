/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "debug.h"
#include "../core/config.h"

using namespace ft;

std::ofstream Debug::FILE_OUT;

std::string Debug::ERR_STR = "ERR:";
std::string Debug::WARN_STR = "WARN:";

bool Debug::ON = false;
bool Debug::WARN = false;   
bool Debug::ERR = false;

//////// ADD  BELOW YOUR DEBUG FLAG INITIALIZATION //////////////

int Debug::APP = 0;
int Debug::MODEL_LOADING = 0;
int Debug::UPDATE = 0;
int Debug::RENDER = 0;
int Debug::FPS = 0;
int Debug::CAMERA = 0;
int Debug::INPUT = 0;

//////// //////////////

void Debug::InitDebug()
{
#ifdef _DEBUGS_TO_FILE
    FILE_OUT.open("logs.txt"); 
#endif

    LoadLevelsFromConfig();
}

void Debug::Destroy()
{
#ifdef _DEBUGS_TO_FILE
    FILE_OUT.close();
#endif
}

int Debug::ReadLevelFromConfig(const char * debugName)
{
    int level = 0;
    if (Config::getInstance()->IsKey(debugName))
        level = Config::getInstance()->GetIntVal(debugName);
    return level;
}

void Debug::LoadLevelsFromConfig()
{
    ON = (ReadLevelFromConfig("DEBUG_ON")==1);

    if (ON)
    {
        WARN = (ReadLevelFromConfig("DEBUG_WARN")==1);
        ERR = (ReadLevelFromConfig("DEBUG_ERR")==1);

        //////// ADD  BELOW YOUR DEBUG FLAG LOADING form Config//////////////

        APP = ReadLevelFromConfig("DEBUG_APP");
        MODEL_LOADING = ReadLevelFromConfig("DEBUG_MODEL_LOADING");
        UPDATE = ReadLevelFromConfig("DEBUG_UPDATE");
        RENDER = ReadLevelFromConfig("DEBUG_RENDER");
        FPS = ReadLevelFromConfig("DEBUG_FPS");
        CAMERA = ReadLevelFromConfig("DEBUG_CAMERA");
        INPUT = ReadLevelFromConfig("DEBUG_INPUT");

        //////////////////////

    }
}


