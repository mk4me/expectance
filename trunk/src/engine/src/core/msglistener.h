/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MSG_LISTENER_H
#define _GEN_MSG_LISTENER_H

#include "../core/platform.h"
#include "message.h"

namespace ft
{
	/**
	 * Class InputListener: class to implement by listeners for messages
	 **/
    class ENGINE_API MsgListener
    {
    public:
        MsgListener(void) { /*empty*/}
	    virtual ~MsgListener(void) { /*empty*/}
        virtual void OnMessage(Message* msg) { /*empty*/} // receives a message from MsgSender
    };
}

#endif //_GEN_MSG_LISTENER_H