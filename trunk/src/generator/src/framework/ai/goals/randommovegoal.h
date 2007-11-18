/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_RANDOMMOVEGOAL_H
#define _GEN_AI_RANDOMMOVEGOAL_H

#include "../goal.h"

namespace ft
{
	/**
	 * Class RandomMoveGoal: the goal is to perform random move from time to time
     * 
     **/
    class RandomMoveGoal : public virtual Goal
    {
    public:
        RandomMoveGoal();
        virtual ~RandomMoveGoal(void);

        int getActionToPerform();

    private:
        int counter;
    };
};

#endif //_GEN_AI_RANDOMMOVEGOAL_H