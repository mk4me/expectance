//----------------------------------------------------------------------------//
// Simulation.h                                                               //
// Copyright (C) 2007 Future Today                                            //
// author M.Kulbacki                                                          //
//----------------------------------------------------------------------------//

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "simulation.h"

Simulation::Simulation()
{
}
Simulation::~Simulation()
{
}



/*----- Get the current millisecond tick count -----*/
unsigned int Simulation::GetTick()
{
#ifdef _WIN32
	return GetTickCount();
#else

  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
}