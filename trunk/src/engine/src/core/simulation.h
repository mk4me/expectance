/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "global.h"

#ifndef _GEN_SIMULATION_H
#define _GEN_SIMULATION_H

namespace ft
{
	/**
	 * Class Simulation: class responsible for calculating simulation time 
	 **/
	class Simulation
	{
	public:
		Simulation(void){/*empty*/};
		virtual ~Simulation(void){/*empty*/};
		/// \brief Gives actual time in milliseconds or microseconds
		static unsigned long long getTick();
		/// \brief Gives actual time with millisecond precision
		static unsigned long long getmsTick();
		/// \brief Gives actual time with microsecond precision
		static unsigned long long getusTick();
		//! set Simulation precision flag
		static ft::TimePrecision getTimePrecision();
		//! get Simulation precision flag
		static void setTimePrecision(const ft::TimePrecision tp);

	private:
		static ft::TimePrecision m_timePrecision;
		#ifdef WIN32
			static unsigned long long currentTime;
		#else
			timeval currentTime;
		#endif
	};
};

#endif //_GEN_SIMULATION_H