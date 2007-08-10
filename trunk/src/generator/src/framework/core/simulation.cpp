/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */


#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "simulation.h"

using namespace ft;

/**
 * Constructor for Simulation: 
 *
 **/
Simulation::Simulation()
{
}
/**
 * Destructor for Simulation
 *
 **/
Simulation::~Simulation()
{
}

/**
 * Returns the current millisecond tick count
 *
 * \return unsigned int - time in milliseconds
 **/
unsigned int Simulation::getTick()
{
#ifdef _WIN32
	return GetTickCount();
#else

  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}