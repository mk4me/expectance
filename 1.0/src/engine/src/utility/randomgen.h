/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_RANDOMGEN_H
#define _GEN_RANDOMGEN_H

#include "../core/platform.h"

namespace ft
{
	/**
	 * Class RandomGenerator: useful methods for generating random values
	 **/
    class ENGINE_API RandomGenerator
    {
    public:
        static int RanIntValue(int scope);
    };
}

#endif //_GEN_RANDOMGEN_H
