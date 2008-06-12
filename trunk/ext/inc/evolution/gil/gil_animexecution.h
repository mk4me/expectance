/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GIL_ANIM_EXECUTION_H
#define _FT_GIL_ANIM_EXECUTION_H

#include "gil_interface.h"

namespace ft { namespace gil {

	//!An interface for animation execution object in environment.
	/*!
	 * It is used to chceck the state of animation currently being played.
	 */
	class EVOLUTION_API GIL_AnimExecution : public GIL_Interface
    {
		//intentionally empty
    };
}  } //namespace ft::gil
#endif //_FT_GIL_ANIM_EXECUTION_H