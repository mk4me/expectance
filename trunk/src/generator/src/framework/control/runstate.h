/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_RUNSTATE_H
#define _GEN_CONTROL_RUNSTATE_H

#include <string>
#include "controlstate.h"

namespace ft
{
	/**
	 * Class RunState: class represents activity when avatar is idle
     * 
     **/
    class RunState : public ControlState
    {
    public:
        RunState():ControlState(STATE_IDLE_ID) { /* empty */ }
        virtual ~RunState(void) { /* empty */ }

        virtual void Init(MovableAvatar* avatar);
        virtual void Entry(MovableAvatar* avatar);
        virtual void Exit(MovableAvatar* avatar);
        virtual void Reset(MovableAvatar* avatar);

        virtual std::string toString();
    };
};

#endif //_GEN_CONTROL_RUNSTATE_H