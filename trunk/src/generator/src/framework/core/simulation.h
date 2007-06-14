//----------------------------------------------------------------------------//
// Simulation.h                                                               //
// Copyright (C) 2007 Future Today                                            //
// author M.Kulbacki                                                          //
//----------------------------------------------------------------------------//
#pragma once
#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation
{
public:
	Simulation(void);
	virtual ~Simulation(void);
	static unsigned int GetTick();
};

#endif