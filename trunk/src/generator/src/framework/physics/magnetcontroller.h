/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_PHYSICS_MAGNETCONTROLLER_H
#define _GEN_PHYSICS_MAGNETCONTROLLER_H

#include "../timeline/timelinemodifier.h"

namespace ft
{
	/**
	 * Class MagnetController: class responsible for controling direction of avatar. It makes each avatar behaving as magnet. It works as modifier of timeline.
     * 
     **/
    class MagnetController : public TimeLineModifier
    {
    public:
        MagnetController();
        virtual ~MagnetController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

        virtual std::string toString(); //OVERRIDEN

        //DataCollector *curve_curr_speed;
        //DataCollector *curve_min_speed;
        //DataCollector *curve_max_speed;
    };
}

#endif //_GEN_PHYSICS_MAGNETCONTROLLER_H