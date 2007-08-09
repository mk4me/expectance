/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_DEBUG_H
#define _GEN_DEBUG_H


#include <iostream>

#define DBG(x) Debug::out(x)

#define DEBUG_MESSAGES true

#define FT_PREFIX "<FT>"

namespace ft
{
	/**
	 * Class Debug: <describe the behavior and responsibility of this class>
	 **/
    class Debug
    {
    public:
        static void out(std::string text);

    };
};

#endif //_GEN_DEBUG_H
