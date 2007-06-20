/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "debug.h"

using namespace ft;

void Debug::out(std::string text)
{
    std::cout << FT_PREFIX << " "<<text.c_str() << std::endl;
}