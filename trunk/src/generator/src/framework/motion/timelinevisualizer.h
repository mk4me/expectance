/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_VISUALIZER_H
#define _GEN_TIMELINE_VISUALIZER_H

#include "../timeline/timelinemodifier.h"

namespace ft
{
	/**
	 * Class TimeLineVisualizer: specific modifier responsible for visualization of timeline execution
	 **/
    class TimeLineVisualizer : public TimeLineModifier
    {
    public:
        static bool TRACK_TIMELINE; 

        TimeLineVisualizer();
        virtual ~TimeLineVisualizer(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

        virtual std::string toString(); //OVERRIDEN

    private:
		TraceLine *tracer_timeline_states;
    };
};

#endif //_GEN_TIMELINE_VISUALIZER_H