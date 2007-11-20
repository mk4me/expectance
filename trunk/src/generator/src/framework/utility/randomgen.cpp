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
    //static int counter = 100;
    static bool inited = false;
    if (!inited)
    {   
        time_t t = clock();
        srand((unsigned)t);
        inited = true;
        //counter--;
    }
    //else
    //{
    //    counter = 100;
    //}

    scope = scope>0 ? scope:1;

    return rand() % scope;
}
