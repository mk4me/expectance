/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_IDLESTATE_H
#define _GEN_CONTROL_IDLESTATE_H

#include <string>
#include "controlstate.h"

namespace ft
{
	/**
	 * Class IdleState: class represents activity when avatar is idle
     * 
     **/
    class IdleState : public ControlState
    {
    public:
        IdleState():ControlState(STATE_IDLE_ID) { /* empty */ }
        virtual ~IdleState(void) { /* empty */ }

        virtual void Init(MovableAvatar* avatar);
        virtual void Entry(MovableAvatar* avatar);
        virtual void Exit(MovableAvatar* avatar);
        virtual void Reset(MovableAvatar* avatar);

        virtual std::string toString();

    private:
        TimeLineMotion* m_tlIdle;
    };
};

#endif //_GEN_CONTROL_IDLESTATE_H