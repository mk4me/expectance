/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _FT_RESTPOSGOAL_H
#define _FT_RESTPOSGOAL_H

#include <cal3d/src/cal3d.h>
#include "evolution/goal.h"

namespace ft
{
	/**
	 * Class RestPosController: class responsible control of RestPosGoal execution
     * 
     **/
	class RestPosController : public GoalController
    {
    public:
        RestPosController(Avatar *av);
        virtual ~RestPosController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

	protected:
        float m_changeSpeed;

    };


	/**
	 * Class RestPosGoal: the goal is to keep avatar in rest position
     * 
     **/
	class RestPosGoal : public Goal
    {
    public:
        RestPosGoal();
        virtual ~RestPosGoal(void);
		virtual void OnEnter(Avatar* av);
		virtual void OnExit(Avatar* av);
        int getActionToPerform();
        virtual GoalController* CreateController(Avatar *av);
    };
}

#endif //_FT_RESTPOSGOAL_H
