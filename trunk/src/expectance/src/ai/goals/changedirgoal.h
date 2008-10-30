/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_CHANGEDIRGOAL_H
#define _GEN_AI_CHANGEDIRGOAL_H

#include "evolution/goal.h"
#include <cal3d/cal3d.h>

namespace ft
{
	/**
	 * Class ChangeDirController: class responsible control of ChangeDirGoal execution
     * 
     **/
    class ChangeDirController : public GoalController
    {
    public:
        ChangeDirController(float minAngle, float maxAngle);
        virtual ~ChangeDirController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

		std::string toString();

    protected:
        float m_fAngle;
        bool m_changeToLeft;  //true if changing direction to LEFT, false if cahnging to RIGHT
        float m_changeSpeed;

		bool m_motionEnforced; // if motion was eforced when avatar had been in IDLE

    };


	/**
	 * Class ChangeDirGoal: the goal is to change direction by avatar
     * 
     **/
    class ChangeDirGoal : public virtual Goal
    {
    public:
        ChangeDirGoal();
        virtual ~ChangeDirGoal(void);

		virtual void OnEnter(Avatar* av);
		virtual void OnExit(Avatar* av);
        int getActionToPerform();
        virtual GoalController* CreateController(Avatar *av);
    };
}

#endif //_GEN_AI_CHANGEDIRGOAL_H
