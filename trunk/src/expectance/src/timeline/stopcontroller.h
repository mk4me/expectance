/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_STOP_CONTROLLER_H
#define _GEN_STOP_CONTROLLER_H

#include "evolution/controller.h"
#include "evolution/timelinecontext.h"
//#include "scene/line.h"



namespace ft
{
	class StopController : public Controller
	{
	public:
		StopController(void);
		virtual ~StopController(void);
        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);
        virtual std::string toString(); //OVERRIDEN

		void setStop(bool value) {m_stop = value;};
		bool isStopped() {return m_stop;};
		bool isFrontLeg(Avatar* av);
		bool STOPCONTROLLER_TRACE;

	private:
		bool m_stop;
///		Line *m_rootRFootVector, *m_directionVector;
		////bool m_leftFootPlant, m_rightFootPlant;
		////float m_footLimits[4]; // order of limits: HeelminY, HeelmaxY, ToeminY, ToemaxY
		////std::map<std::string, const float*> m_limits;
		
	};
}
#endif //_GEN_STOP_CONTROLLER_H


