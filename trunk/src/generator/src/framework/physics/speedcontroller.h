/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_PHYSICS_SPEEDCONTROLLER_H
#define _GEN_PHYSICS_SPEEDCONTROLLER_H

#include "../timeline/timelinemodifier.h"

namespace ft
{
	/**
	 * Class SpeedController: class responsible for controling speed of avatar. It works as modifier of timeline.
     * 
     **/
    class SpeedController : public TimeLineModifier
    {
    public:
        bool DRAW_SPEED_CURVE;

        float SPEEDFACTOR_CHANGE;  // defines how quik the curr_speed_factor should drive at dest_speed_factor

        SpeedController();
        virtual ~SpeedController(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

        virtual std::string toString(); //OVERRIDEN

        DataCollector *curve_curr_speed;
        DataCollector *curve_min_speed;
        DataCollector *curve_max_speed;
    };
}

#endif //_GEN_PHYSICS_SPEEDCONTROLLER_H