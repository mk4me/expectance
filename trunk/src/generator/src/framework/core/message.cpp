/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "message.h"

using namespace ft;

/**
 * \brief Returns string representation of messages
 *
 * \param int msg - identifier of message
 * \return std::string - string representation of message
 **/
std::string Message::_GET_MSG_NAME(int msg)
{
    std::string strMsg;

    switch(msg)
    {
    case MSG_CONTROL_START: strMsg = "MSG_CONTROL_START"; break;
    case MSG_CONTROL_STOP: strMsg = "MSG_CONTROL_STOP"; break;
    case MSG_CONTROL_TURN_LEFT: strMsg = "MSG_CONTROL_TURN_LEFT"; break;
    case MSG_CONTROL_TURN_RIGHT: strMsg = "MSG_CONTROL_TURN_RIGHT"; break;

    case MSG_CONTROL_PAUSE: strMsg = "MSG_CONTROL_PAUSE"; break;
    case MSG_PROPERTY_LOD: strMsg = "MSG_PROPERTY_LOD"; break;
    case MSG_PROPERTY_RENDER_METHOD: strMsg = "MSG_PROPERTY_RENDER_METHOD"; break;
	case MSG_PROPERTY_SHADOW: strMsg = "MSG_PROPERTY_SHADOW"; break;
    case MSG_TEST: strMsg = "MSG_TEST"; break;
//	case MSG_INPUT_KEY_PRESSED: strMsg ="MSG_INPUT_KEY_PRESSED"; break;
	case MSG_MENU_ITEM_SELECTED: strMsg ="MSG_MENU_ITEM_SELECTED"; break;       
    default: strMsg = "<unknown>"; break;
    }
    
    return strMsg;
}
