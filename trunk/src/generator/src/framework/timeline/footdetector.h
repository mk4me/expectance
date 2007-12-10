/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_FOOT_DETECTOR_H
#define _GEN_FOOT_DETECTOR_H

#include "timelinemodifier.h"


namespace ft
{
	class FootDetector : public TimeLineModifier
	{
	public:
		bool FOOTPLANT_TRACE;
		FootDetector(void);
		virtual ~FootDetector(void);
        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);
		//! get set of limits for foot location on the floor on the base of motion name
		const float* getLimits(const std::string& motionName);
		//! add set of limits for motion labeled by motionName		
		bool AddLimits(const std::string& motionName, const float* limits);

	private:
		TraceLine *m_tracerL, *m_tracerR;
		bool m_leftFootPlant, m_rightFootPlant;
		float m_footLimits[4]; // order of limits: HeelminY, HeelmaxY, ToeminY, ToemaxY
		std::map<std::string, const float*> m_limits;
		
	};
};
#endif //_GEN_FOOT_DETECTOR_H
