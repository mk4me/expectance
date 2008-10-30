/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_RANDOMMOVEGOAL_H
#define _GEN_AI_RANDOMMOVEGOAL_H

#include "evolution/goal.h"

namespace ft
{
    /**
	 * Class RandomMoveController: class responsible control of RandomMoveGoal execution
     * 
     **/
    class RandomMoveController : public GoalController
    {
    public:
        RandomMoveController(float desiredGoalTime);
        virtual ~RandomMoveController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

		std::string toString();
    private:
        float m_sumTime;
        float m_desiredGoalTime;
    };

	/**
	 * Class RandomMoveGoal: the goal is to perform random move from time to time
     * 
     **/
    class RandomMoveGoal : public virtual Goal
    {
    public:
        RandomMoveGoal();
        virtual ~RandomMoveGoal(void);

		virtual void OnEnter(Avatar* av);
		virtual void OnExit(Avatar* av);
        virtual int getActionToPerform();
        virtual GoalController* CreateController(Avatar *av);
    };
}

#endif //_GEN_AI_RANDOMMOVEGOAL_H
