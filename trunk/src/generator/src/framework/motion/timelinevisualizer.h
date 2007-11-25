/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_VISUALIZER_H
#define _GEN_TIMELINE_VISUALIZER_H

#include "../timeline/timelinemodifier.h"

#include "scene/visualizationmanager.h"

namespace ft
{
	/**
	 * Class TimeLineVisualizer: specific modifier responsible for visualization of timeline execution
	 **/
    class TimeLineVisualizer : public TimeLineModifier
    {
    public:
        static bool TRACK_ON;
        bool TRACK_TIMELINE_STATES; 

        TimeLineVisualizer();
        virtual ~TimeLineVisualizer(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

    private:
		TraceLine *tracer_timeline_states;
    };
};

#endif //_GEN_TIMELINE_VISUALIZER_H