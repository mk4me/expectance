/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "debug.h"

using namespace ft;

/**
 * out: <describe the responsibilities and behavior of this method>
 *
 * \param std::string text <argument description>
 **/
void Debug::out(std::string text)
{
    std::cout << FT_PREFIX << " "<<text.c_str() << std::endl;
}