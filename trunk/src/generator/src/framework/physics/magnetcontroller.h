/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_PHYSICS_MAGNETCONTROLLER_H
#define _GEN_PHYSICS_MAGNETCONTROLLER_H

#include "../timeline/timelinemodifier.h"
#include "scene/line.h"
#include "scene/circle.h"
#include "scene/circulararc.h"

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
		void setThreshold(const double threshold){m_threshold = threshold;};
		double getThreshold(){return m_threshold;};

	private:
		double m_threshold;   // threshold value 
		CalVector m_cumForce; // cummulative force

		DataCollector *curve_curr_angle;
        DataCollector *curve_cummForce_angle;
		bool DRAW_MAGNET_CURVE;
		Line *m_forceVector, *m_directionVector;
		CircularArc *m_boundaryParameters;
		Circle *m_boundaryScope;
	};
}

#endif //_GEN_PHYSICS_MAGNETCONTROLLER_H