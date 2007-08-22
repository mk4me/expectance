/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_CONTEXT_H
#define _GEN_TIMELINE_CONTEXT_H

#include "../avatar/avatar.h"

namespace ft
{
    static const int ADD_COPY                      = 0x00000001; 
    static const int ADD_AS_LAST                   = 0x00000002; 
    static const int ADD_AS_NEXT                   = 0x00000004; 
    static const int REMOVE_AFTER_EXECUTION        = 0x00000008; 
    static const int DELETE_OBJECTS_AFTER_REMOVING = 0x0000000F; 
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

        bool add_as_last;
        bool add_ad_next;
        bool add_copy;
        int where_to_add;
        bool remove_after_execution;
        bool delete_objects_after_remove_from_list;
        bool stop_immediate;
    };
};

#endif //_GEN_TIMELINE_CONTEXT_H