/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_CONTEXT_H
#define _GEN_TIMELINE_CONTEXT_H

#include "../avatar/avatar.h"

namespace ft
{
    static const int ADD_AS_NEXT                   = 0x00000004; 
    static const int REMOVE_AFTER_EXECUTION        = 0x00000008; 
    static const int STOP_IMMEDIATE                = 0x00000010; 

	/**
	 * Class TimeLineContext: represents set of parameters affecting TimeLine execution
	 **/
    class TimeLineContext
    {
    public:
        TimeLineContext(void); 
        virtual ~TimeLineContext(void);
        virtual void Destroy(void);

        void SetValuesByFlag(int positive_flag, int negative_flag);

        void SetDefaultFlags();

        void setAvatar(Avatar* avatar) { m_avatar = avatar; }
        Avatar* getAvatar() { return m_avatar; }

        Avatar *m_avatar;

        int where_to_add;
        bool remove_after_execution;
        bool stop_immediate;
    };
};

#endif //_GEN_TIMELINE_CONTEXT_H