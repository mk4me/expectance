/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "randomgen.h"

#include <time.h>
#include <stdlib.h>

using namespace ft;

int RandomGenerator::RanIntValue(int scope)
{
    static bool inited = false;
    if (!inited)
    {
        srand(NULL);
        inited = true;
    }

    scope = scope>0 ? scope:1;

    return rand() % scope;
}
