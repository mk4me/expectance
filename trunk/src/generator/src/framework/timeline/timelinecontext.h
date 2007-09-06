/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_CONTEXT_H
#define _GEN_TIMELINE_CONTEXT_H

#include "../avatar/avatar.h"

namespace ft
{
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

        void setCurrAnimID(int anim_id) { m_currAnimID = anim_id; }
        int getCurrAnimID() { return m_currAnimID; }

        void setCurrAnimTime(float time) { m_currAnimTime = time; }
        float getCurrAnimTime() { return m_currAnimTime; }

        void setCurrAnimLoop(bool set) { m_currAnimLoop = set; }
        bool isCurrAnimLoop() { return m_currAnimLoop; }

        

        Avatar *m_avatar;

        int where_to_add;
        bool remove_after_execution;
        bool stop_immediate;

        int m_currAnimID;
        float m_currAnimTime;
        bool m_currAnimLoop;

        
    };
};

#endif //_GEN_TIMELINE_CONTEXT_H