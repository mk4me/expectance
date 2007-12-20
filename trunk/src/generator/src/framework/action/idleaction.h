/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_IDLEACTION_H
#define _GEN_IDLEACTION_H

#include <string>
#include "action.h"

namespace ft
{
	/**
	 * Class IdleAction: class represents IDLE activity of avatar
     * 
     **/
    class IdleAction : public Action
    {
    public:
        IdleAction():Action(ACTION_IDLE_ID) { /* empty */ }
        virtual ~IdleAction(void) { /* empty */ }

        virtual void Init(PhysicsAvatar* avatar);
        virtual void Entry(PhysicsAvatar* avatar, Action* oldAction);
        virtual void Exit(PhysicsAvatar* avatar, Action* newAction);
        virtual void Reset(PhysicsAvatar* avatar);

        virtual std::string toString();

    private:
        TimeLineMotion* m_tlIdle;
    };
}

#endif //_GEN_IDLEACTION_H