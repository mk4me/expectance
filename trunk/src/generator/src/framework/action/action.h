/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_ACTION_H
#define _GEN_ACTION_H

#include "../physics/physicsavatar.h"
#include <string>

namespace ft
{
    static const int ACTION_IDLE_ID = 0; 
    static const int ACTION_WALK_ID = 1;
    static const int ACTION_RUN_ID = 2;
	/**
	 * Class Action: base class for action that avatar can perform
     * 
     **/
    class Action
    {
    public:
        Action(int action_id) { m_action_id = action_id; }
        virtual ~Action(void) { /* empty */ }

        int getId() { return m_action_id; }

        virtual void Init(PhysicsAvatar* avatar);
        virtual void Entry(PhysicsAvatar* avatar, Action* oldAction);
        virtual void Exit(PhysicsAvatar* avatar, Action* newAction);
        virtual void Reset(PhysicsAvatar* avatar);

        virtual std::string toString();

    private:
        int m_action_id;
    };
};

#endif //_GEN_ACTION_H