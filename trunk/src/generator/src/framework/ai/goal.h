/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_GOAL_H
#define _GEN_AI_GOAL_H

#include "../core/baseobject.h"
#include "../action/action.h"

namespace ft
{
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

         virtual int getActionToPerform();
    };
};

#endif //_GEN_AI_GOAL_H