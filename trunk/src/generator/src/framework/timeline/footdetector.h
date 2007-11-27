/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_FOOT_DETECTOR_H
#define _GEN_FOOT_DETECTOR_H

#include "timelinemodifier.h"
#include "scene/visualizationmanager.h"

namespace ft
{
	class FootDetector : public TimeLineModifier
	{
	public:
		bool FOOTPLANT_TRACE;
		FootDetector(void);
		virtual ~FootDetector(void);
        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);
		//void AddFootLimits(float FootminY, float FootmaxY, float ToeminY, float ToemaxY);
		const float* getLimits(const std::string& motionName);
		bool AddLimits(const std::string& motionName, const float* limits);

	private:
		TraceLine *tracer_L, *tracer_R;
		float footLimits[4]; // order of limits: FootminY, FootmaxY, ToeminY, ToemaxY
		std::map<std::string, const float*> m_limits;
	};
};
#endif //_GEN_FOOT_DETECTOR_H
