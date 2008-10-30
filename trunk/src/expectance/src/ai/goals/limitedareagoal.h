/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_LIMITEDAREAGOAL_H
#define _GEN_AI_LIMITEDAREAGOAL_H

#include "changedirgoal.h"
#include "../../avatar/osgavatar.h"
#include <cal3d/cal3d.h>

namespace ft
{
	/**
	 * Class LimitedAreaController: class responsible control of LimitedAreaGoal execution
     * 
     **/
    class LimitedAreaController : public ChangeDirController
    {
    public:
        LimitedAreaController(Avatar *av);
        virtual ~LimitedAreaController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

		std::string toString();

    };


	/**
	 * Class LimitedAreaGoal: the goal is to keep avatar in given area
     * 
     **/
    class LimitedAreaGoal : public virtual ChangeDirGoal
    {
    public:
        LimitedAreaGoal();
        virtual ~LimitedAreaGoal(void);
        virtual GoalController* CreateController(Avatar *av);
		virtual void OnEnter(Avatar* av);
		virtual void OnExit(Avatar* av);

    };
}

#endif //_GEN_AI_LIMITEDAREAGOAL_H
