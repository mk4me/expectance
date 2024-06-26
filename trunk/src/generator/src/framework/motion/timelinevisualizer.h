/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_VISUALIZER_H
#define _GEN_TIMELINE_VISUALIZER_H

#include "evolution/controller.h"
#include "evolution/timelinecontext.h"
#include "scene/datacollector.h"
//#include "scene/traceline.h"

namespace ft
{
	/**
	 * Class TimeLineVisualizer: specific modifier responsible for visualization of timeline execution
	 **/
    class TimeLineVisualizer : public Controller
    {
    public:
        static bool CURVE_ANIM_WEIGHT; 

        TimeLineVisualizer();
        virtual ~TimeLineVisualizer(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

        

    private:
//		TraceLine *tracer_timeline_states;

        DataCollector *curve_prev_anim_weight;
        DataCollector *curve_curr_anim_weight;
    };
}

#endif //_GEN_TIMELINE_VISUALIZER_H
