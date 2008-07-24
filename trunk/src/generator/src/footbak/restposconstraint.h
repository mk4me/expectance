/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _FT_RESTPOSCONSTRAINT_H
#define _FT_RESTPOSCONSTRAINT_H

#include "evolution/constraint.h"

namespace ft
{
	/**
	 * Class RestPosConstraint: this class represents constrait for limit area of motion
     * 
     **/
    class RestPosConstraint: public Constraint 
    {
    public:
        RestPosConstraint();
        virtual ~RestPosConstraint(void);
        
        virtual bool Check(Avatar *av);
    };
}

#endif //_FT_RESTPOSCONSTRAINT_H
