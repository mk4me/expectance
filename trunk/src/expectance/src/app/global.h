//----------------------------------------------------------------------------//
// global.h                                                                   //
// Copyright (C) 2008 Future Today                                            //
//                                                                            //
//----------------------------------------------------------------------------//


#ifndef GLOBAL_H
#define GLOBAL_H


#ifdef _WIN32
#include <windows.h>
#endif

namespace ft
{
    static const std::string FT_DATA_PATH = "../../data/";  
	static const std::string FT_MODELS_PATH = FT_DATA_PATH + "models/";  
}

#endif //GLOBAL_H



