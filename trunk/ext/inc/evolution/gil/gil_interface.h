/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GIL_INTERFACE_H
#define _FT_GIL_INTERFACE_H

#include "../platform.h"

#include <string>

namespace ft 
{ 
	/** 
	* 
	* \brief API which must be implemented by environment (e.g. Game) in which *Evolution* package is used.
	* Interfaces exposed by this API are then used by classes in *Evolution* package.
	*/
	namespace gil { 

		//!Interface for GIL object.
		/*!
		 * An interface which must be implemented by objects from GIL to be used in Evolution
		 */
		class EVOLUTION_API GIL_Interface
		{
		};
	} //namespace gil
} //namespace ft
#endif //_FT_GIL_INTERFACE_H