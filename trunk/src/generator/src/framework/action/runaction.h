/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_RUNACTION_H
#define _GEN_RUNACTION_H

#include <string>
#include "action.h"

namespace ft
{
	/**
	 * Class RunAction: class represents RUN activity of avatar 
     * 
     **/
    class RunAction : public Action
    {
    public:
        RunAction():Action(ACTION_RUN_ID) { /* empty */ }
        virtual ~RunAction(void) { /* empty */ }

        virtual void Init(PhysicsAvatar* avatar);
        virtual void Entry(PhysicsAvatar* avatar, Action* oldAction);
        virtual void Exit(PhysicsAvatar* avatar, Action* newAction);
        virtual void Reset(PhysicsAvatar* avatar);

        virtual std::string toString();
    private:
        TimeLineMotion* m_tlRun;
        TimeLineMotion* m_tlStopRun;
    };
}

#endif //_GEN_RUNACTION_H
