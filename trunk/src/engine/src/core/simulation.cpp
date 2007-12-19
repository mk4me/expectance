/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */


#include "simulation.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif


using namespace ft;

#ifdef WIN32
	unsigned long long Simulation::currentTime;
#else
	timeval currentTime;
#endif

ft::TimePrecision Simulation::m_timePrecision = ft_Microseconds;



unsigned long long Simulation::getTick()
{
	if (Simulation::m_timePrecision == ft_Miliseconds)
		return getmsTick();
	else
		return getusTick();
}

unsigned long long Simulation::getmsTick()
{
#ifdef _WIN32
	return GetTickCount();
#else
  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}

unsigned long long Simulation::getusTick()
{
#ifdef WIN32
    static bool onetime = 0;
    static unsigned long long freq = 0;
    
    if(!onetime) {
        
        onetime = true;
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    }
    
	QueryPerformanceCounter((LARGE_INTEGER*)&Simulation::currentTime);
    
	Simulation::currentTime *= 1000000;
	Simulation::currentTime /= freq;
    
	return (unsigned long)Simulation::currentTime;
#else
    gettimeofday(&currentTime, 0);
    
    return (unsigned long)((currentTime.tv_sec) / 1000)
            + (unsigned long)((currentTime.tv_usec));
#endif
}

ft::TimePrecision Simulation::getTimePrecision()
{
	return m_timePrecision;
}

void Simulation::setTimePrecision(const ft::TimePrecision tp)
{
	m_timePrecision = tp;
}