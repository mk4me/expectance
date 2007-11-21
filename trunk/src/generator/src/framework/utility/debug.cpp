/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "debug.h"
#include "../core/config.h"

using namespace ft;

bool Debug::ON = false;
bool Debug::WARN = false;
bool Debug::ERR = false;

int Debug::APP = 0;

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

        if (Config::getInstance()->IsKey("DEBUG_APP"))
            APP = Config::getInstance()->GetIntVal("DEBUG_APP");
    }
}


