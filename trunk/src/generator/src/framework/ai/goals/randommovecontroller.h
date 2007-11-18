/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_RANDOMMOVECONTROLER_H
#define _GEN_AI_RANDOMMOVECONTROLER_H

#include "../aicontroller.h"

namespace ft
{
	/**
	 * Class RandomMoveController: class responsible control of RandomMoveGoal execution
     * 
     **/
    class RandomMoveController : public AIController
    {
    public:
        RandomMoveController(float desiredGoalTime);
        virtual ~RandomMoveController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    
    private:
        float m_sumTime;
        float m_desiredGoalTime;
    };
};

#endif //_GEN_AI_RANDOMMOVECONTROLER_H