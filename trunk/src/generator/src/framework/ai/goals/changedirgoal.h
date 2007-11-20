/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_CHANGEDIRGOAL_H
#define _GEN_AI_CHANGEDIRGOAL_H

#include "../goal.h"
#include "../aicontroller.h"
#include <cal3d/cal3d.h>

namespace ft
{
	/**
	 * Class ChangeDirController: class responsible control of ChangeDirGoalexecution
     * 
     **/
    class ChangeDirController : public AIController
    {
    public:
        ChangeDirController(float minAngle, float maxAngle);
        virtual ~ChangeDirController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

    private:
        float m_fAngle;
        bool m_changeToLeft;  //true if changing direction to LEFT, false if cahnging to RIGHT
        float m_changeSpeed;

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

        virtual bool ExecuteAction(ActionAvatar *av);
        int getActionToPerform();
        virtual AIController* CreateController(ActionAvatar *av);
    };
};

#endif //_GEN_AI_CHANGEDIRGOAL_H