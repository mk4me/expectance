/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_GlobalMsgSender_H
#define _GEN_GlobalMsgSender_H

#include "msgsender.h"

namespace ft
{
	/**
	 * Class GlobalMsgSender: manages global messages flow 
	 **/
    class GlobalMsgSender : public MsgSender
    {
    public:
        GlobalMsgSender(void) { /*empty*/}
	    virtual ~GlobalMsgSender(void) { /*empty*/}
        
        static GlobalMsgSender* getInstance();
        static void DestroyInstance();

    private:
        
        static GlobalMsgSender* m_instance;
    };
}

#endif //_GEN_GlobalMsgSender_H