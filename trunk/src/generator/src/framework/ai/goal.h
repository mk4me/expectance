/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_GOAL_H
#define _GEN_AI_GOAL_H

#include "core/baseobject.h"
#include "../action/actionavatar.h"
#include "aicontroller.h"

namespace ft
{
    static const int PRIORITY_LOW      = 0; 
    static const int PRIORITY_MEDIUM   = 1;
    static const int PRIORITY_HIGH     = 2;
    static const int PRIORITY_CRITICAL = 3;

	/**
	 * Class Goal: represents goal of avatar which is the base element of avatar behaviour
     * 
     **/
    class Goal : public virtual BaseObject
    {
    public:
        Goal();
        virtual ~Goal(void);
        virtual void Destroy(void);

        virtual bool ExecuteAction(ActionAvatar *av);

        virtual int getActionToPerform();
        virtual AIController* CreateController(ActionAvatar *av);

        void setPriority(int priority) { m_priority = priority; }
        int getPriority() { return m_priority; }

    private:
        int m_priority;
    };
};

#endif //_GEN_AI_GOAL_H