/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _GEN_DEBUG_H
#define _GEN_DEBUG_H


#include <iostream>

#define DBG(x) Debug::out(x)

#define DEBUG_MESSAGES false

#define FT_PREFIX "<FT>"

namespace ft
{
	//! A Debug class
	/*!
	 *	Provides a set of methods and properties that help debug your code. 
	 */
    class Debug
    {
    public:
        static bool ON;
        static bool WARN;
        static bool ERR;

        static int APP;

        static void LoadLevelsFromConfig();

    };
};

#endif //_GEN_DEBUG_H
