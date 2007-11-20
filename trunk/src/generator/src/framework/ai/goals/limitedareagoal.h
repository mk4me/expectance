/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_LIMITEDAREAGOAL_H
#define _GEN_AI_LIMITEDAREAGOAL_H

#include "changedirgoal.h"
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
        LimitedAreaController(ActionAvatar *av);
        virtual ~LimitedAreaController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

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
        virtual AIController* CreateController(ActionAvatar *av);
    };
};

#endif //_GEN_AI_LIMITEDAREAGOAL_H