/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_WALKACTION_H
#define _GEN_WALKACTION_H

#include <string>
#include "action.h"

namespace ft
{
	/**
	 * Class WalkAction: class represents WALK activity of avatar
     * 
     **/
    class WalkAction : public Action
    {
    public:
        WalkAction():Action(ACTION_WALK_ID) { /* empty */ }
        virtual ~WalkAction(void) { /* empty */ }

        virtual void Init(PhysicsAvatar* avatar);
        virtual void Entry(PhysicsAvatar* avatar, Action* oldAction);
        virtual void Exit(PhysicsAvatar* avatar, Action* newAction);
        virtual void Reset(PhysicsAvatar* avatar);

        virtual std::string toString();

    private:
        TimeLineMotion* m_tlWalk;
        TimeLineMotion* m_tlStopWalk;
    };
};

#endif //_GEN_WALKACTION_H