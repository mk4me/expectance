/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_WALKSTATE_H
#define _GEN_CONTROL_WALKSTATE_H

#include <string>
#include "controlstate.h"

namespace ft
{
	/**
	 * Class WalkState: class represents walk activity
     * 
     **/
    class WalkState : public ControlState
    {
    public:
        WalkState():ControlState(STATE_WALK_ID) { /* empty */ }
        virtual ~WalkState(void) { /* empty */ }

        virtual void Init(MovableAvatar* avatar);
        virtual void Entry(MovableAvatar* avatar);
        virtual void Exit(MovableAvatar* avatar);
        virtual void Reset(MovableAvatar* avatar);

        virtual std::string toString();

    private:
        TimeLineMotion* m_tlWalk;
        TimeLineMotion* m_tlStopWalk;
    };
};

#endif //_GEN_CONTROL_WALKSTATE_H