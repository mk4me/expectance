/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_FOOT_DETECTOR_H
#define _GEN_FOOT_DETECTOR_H

#include "timelinemodifier.h"
#include "../scene/visualizationmanager.h"

namespace ft
{

	class FootDetector : public TimeLineModifier
	{
	public:
		bool FOOTPLANT_TRACE;
		FootDetector(void);
		virtual ~FootDetector(void);
        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

	private:
		TraceLine *tracer_L, *tracer_R;
		bool lf,rf;
		CalVector ltmin,rtmin;
		CalQuaternion ltq, rtq;

	};
};
#endif //_GEN_FOOT_DETECTOR_H
