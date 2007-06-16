/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */


#pragma once
#ifndef _GEN_SIMULATION_H
#define _GEN_SIMULATION_H

namespace ft
{
	class Simulation
	{
	public:
		Simulation(void);
		virtual ~Simulation(void);
		static unsigned int GetTick();
	};
};

#endif //_GEN_SIMULATION_H