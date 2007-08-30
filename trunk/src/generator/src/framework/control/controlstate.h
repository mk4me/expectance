/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_CONTROLSTATE_H
#define _GEN_CONTROL_CONTROLSTATE_H

#include "../motion/movableavatar.h"
#include <string>

namespace ft
{
    static const int STATE_IDLE_ID = 0; 
    static const int STATE_WALK_ID = 1;
    static const int STATE_RUN_ID = 2;
	/**
	 * Class ControlState: base class for states of avatar
     * 
     **/
    class ControlState
    {
    public:
        ControlState(int state_id) { m_state_id = state_id; }
        virtual ~ControlState(void) { /* empty */ }

        int getId() { return m_state_id; }

        virtual void Init(MovableAvatar* avatar);
        virtual void Entry(MovableAvatar* avatar);
        virtual void Exit(MovableAvatar* avatar);
        virtual void Reset(MovableAvatar* avatar);

        virtual std::string toString();

    private:
        int m_state_id;
    };
};

#endif //_GEN_CONTROL_CONTROLSTATE_H