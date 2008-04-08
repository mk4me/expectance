/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_PROPERTY_H
#define _FT_PROPERTY_H

#include <string>
#include "prophandler.h"

namespace ft
{
    static const std::string UNDEFINED_PROPERTY = "<UNDEFINED_PROPERTY>";

    /**
     * Class Property: represents single property
     **/
    class ENGINE_API Property
    {
    public:
        Property(){ handler = NULL;  str_val = UNDEFINED_PROPERTY; }
        PropHandler* handler;
        std::string str_val;
    };
}

#endif //_FT_PROPERTY_H