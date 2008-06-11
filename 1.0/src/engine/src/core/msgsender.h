/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MSGSENDER_H
#define _GEN_MSGSENDER_H

#include "msglistener.h"
#include "../utility/stlhelper.h"

namespace ft
{
	/**
	 * Class MsgSender: class which can send messages to listeners
	 **/
    class ENGINE_API MsgSender
    {
    public:
        MsgSender(void) { /*empty*/}
	    virtual ~MsgSender(void) { /*empty*/}

        void SendMsg(Message* msg, bool deleteAfterSent);  //request to send a message to registered objects

        void AddMsgListener(MsgListener* listener);
        bool  RemoveMsgListener(MsgListener* listener);

        void Destroy();

    private:
        STL_WrappedVector<MsgListener*> m_vListeners;
        
    };

}

#endif //_GEN_MSGSENDER_H
