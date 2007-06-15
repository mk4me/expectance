/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_DEBUG_H
#define _GEN_DEBUG_H


#include <iostream>

#define DBG(x) Debug::out(x)

#define FT_PREFIX "<FT>"

namespace ft
{
    class Debug
    {
    public:
        static void out(std::string text);

    };
};

#endif //_GEN_DEBUG_H
