/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "message.h"

using namespace ft;

std::string Message::_GET_MSG_NAME(int msg)
{
    std::string strMsg;

    switch(msg)
    {
    case MSG_CONTROL_PAUSE: strMsg = "MSG_CONTROL_PAUSE"; break;
    case MSG_PROPERTY_LOD: strMsg = "MSG_PROPERTY_LOD"; break;
    case MSG_PROPERTY_RENDER_METHOD: strMsg = "MSG_PROPERTY_RENDER_METHOD"; break;
	case MSG_PROPERTY_SHADOW: strMsg = "MSG_PROPERTY_SHADOW"; break;
    default: strMsg = "<unknown>"; break;
    }
    
    return strMsg;
}
