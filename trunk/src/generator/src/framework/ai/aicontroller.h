/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AICONTROLER_H
#define _GEN_AICONTROLER_H

#include "../timeline/timelinemodifier.h"

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
    };
};

#endif //_GEN_AICONTROLER_H