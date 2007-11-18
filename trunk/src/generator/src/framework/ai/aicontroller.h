/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AICONTROLER_H
#define _GEN_AICONTROLER_H

#include "../timeline/timelinemodifier.h"
#include "goal.h"

namespace ft
{
	/**
	 * Class AIController: class responsible for goal realization. It works as modifier of timeline.
     * 
     **/
    class AIController : public TimeLineModifier
    {
    public:
        AIController();
        virtual ~AIController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

        void setControlledGoal(Goal* goal) { m_controlledGoal = goal; }
        Goal* getControlledGoal() { return m_controlledGoal; }

        void setGoalReached(bool isReached) { m_goalReached = isReached; }
        bool isGoalReached() { return m_goalReached; } 

    private:
        Goal* m_controlledGoal;

        bool m_goalReached;
    };
};

#endif //_GEN_AICONTROLER_H