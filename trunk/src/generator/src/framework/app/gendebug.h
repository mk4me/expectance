/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _GEN_GENDEBUG_H
#define _GEN_GENDEBUG_H

#include "utility/debug.h"

namespace ft
{
	//! A Debug class
	/*!
	 *	Provides a set of methods and properties that help debug your code. 
	 */
    class GenDebug : public Debug
    {
    public:

        //////// ADD  BELOW YOUR DEBUG FLAG //////////////

        static int TIMELINE;
        static int ACTION;
        static int AI;
        static int MOTION;
        static int CONTROL;

        static void LoadLevelsFromConfig();
    };
};

#endif //_GEN_GENDEBUG_H
