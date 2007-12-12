/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_CONTEXT_H
#define _GEN_TIMELINE_CONTEXT_H

#include "../avatar/avatar.h"
#include "../motion/motion.h"

namespace ft
{
    //EVENTS from TimeLineExecutor
    static const int EXEC_EVENT_NONE                 = 0; 
    static const int EXEC_EVENT_STATE_CHANGED        = 1; 

    //STATES of TimeLineExecutor
    static const int EXEC_STATE_NOT_INITED = 0; 
    static const int EXEC_STATE_WAIT = 1; 
    static const int EXEC_STATE_SINGLE = 2;
    static const int EXEC_STATE_OVERLAP = 3;
    static const int EXEC_STATE_FADE_IN = 4;
    static const int EXEC_STATE_FADE_OUT = 5;
    static const int EXEC_STATE_TERMINATED = 6;


	/**
	 * Class TimeLineContext: represents set of parameters affecting TimeLine execution
	 **/
    class TimeLineContext
    {
    public:
        TimeLineContext(void) { /* empty */ }
        virtual ~TimeLineContext(void) { /* empty */ }
        virtual void Destroy(void);

        void setAvatar(Avatar* avatar) { m_avatar = avatar; }
        Avatar* getAvatar() { return m_avatar; }

        Avatar *m_avatar;

        int where_to_add;
        bool remove_after_execution;
        bool stop_immediate;

        // RUNTIME parameters
        Motion* prevMotion;
        CalAnimation* prevAnim;
        Motion* currMotion;
        CalAnimation* currAnim;

        float prevAnimTime;
        float prevAnimDuration;

        float currAnimTime;
        float currAnimDuration;

        float prevOverlap;
        float currOverlap;

        int exec_state;
        int exec_event;
        bool anim_changed;
        bool anim_new_cycle;
        bool anim_stopped;
    };
};

#endif //_GEN_TIMELINE_CONTEXT_H