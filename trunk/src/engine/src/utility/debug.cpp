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
int Debug::TIMELINE = 0;
int Debug::ACTION = 0;
int Debug::AI = 0;
int Debug::UPDATE = 0;
int Debug::MOTION  = 0;
int Debug::CONTROL = 0;
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

void Debug::LoadLevelsFromConfig()
{
    if (Config::getInstance()->IsKey("DEBUG_ON"))
        ON = (Config::getInstance()->GetIntVal("DEBUG_ON") == 1);

    if (ON)
    {
        if (Config::getInstance()->IsKey("DEBUG_WARN"))
            WARN = (Config::getInstance()->GetIntVal("DEBUG_WARN") == 1);
        if (Config::getInstance()->IsKey("DEBUG_ERR"))
            ERR = (Config::getInstance()->GetIntVal("DEBUG_ERR") == 1);

        //////// ADD  BELOW YOUR DEBUG FLAG LOADING form Config//////////////

        if (Config::getInstance()->IsKey("DEBUG_APP"))
            APP = Config::getInstance()->GetIntVal("DEBUG_APP");
        if (Config::getInstance()->IsKey("DEBUG_MODEL_LOADING"))
            MODEL_LOADING = Config::getInstance()->GetIntVal("DEBUG_MODEL_LOADING");
        if (Config::getInstance()->IsKey("DEBUG_TIMELINE"))
            TIMELINE = Config::getInstance()->GetIntVal("DEBUG_TIMELINE");
        if (Config::getInstance()->IsKey("DEBUG_ACTION"))
            ACTION = Config::getInstance()->GetIntVal("DEBUG_ACTION");
        if (Config::getInstance()->IsKey("DEBUG_AI"))
            AI = Config::getInstance()->GetIntVal("DEBUG_AI");
        if (Config::getInstance()->IsKey("DEBUG_UPDATE"))
            UPDATE = Config::getInstance()->GetIntVal("DEBUG_UPDATE");
        if (Config::getInstance()->IsKey("DEBUG_MOTION"))
            MOTION = Config::getInstance()->GetIntVal("DEBUG_MOTION");
        if (Config::getInstance()->IsKey("DEBUG_CONTROL"))
            CONTROL = Config::getInstance()->GetIntVal("DEBUG_CONTROL");
        if (Config::getInstance()->IsKey("DEBUG_RENDER"))
            RENDER = Config::getInstance()->GetIntVal("DEBUG_RENDER");
        if (Config::getInstance()->IsKey("DEBUG_FPS"))
            FPS = Config::getInstance()->GetIntVal("DEBUG_FPS");
        if (Config::getInstance()->IsKey("DEBUG_CAMERA"))
            CAMERA = Config::getInstance()->GetIntVal("DEBUG_CAMERA");
        if (Config::getInstance()->IsKey("DEBUG_INPUT"))
            INPUT = Config::getInstance()->GetIntVal("DEBUG_INPUT");

        //////////////////////

    }
}


