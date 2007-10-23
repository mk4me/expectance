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
	 * Class RunState: class represents RUN activity of avatar 
     * 
     **/
    class RunState : public ControlState
    {
    public:
        RunState():ControlState(STATE_RUN_ID) { /* empty */ }
        virtual ~RunState(void) { /* empty */ }

        virtual void Init(MovableAvatar* avatar);
        virtual void Entry(MovableAvatar* avatar, ControlState* oldState);
        virtual void Exit(MovableAvatar* avatar, ControlState* newState);
        virtual void Reset(MovableAvatar* avatar);

        virtual std::string toString();
    private:
        TimeLineMotion* m_tlRun;
        TimeLineMotion* m_tlStopRun;
    };
};

#endif //_GEN_CONTROL_RUNSTATE_H