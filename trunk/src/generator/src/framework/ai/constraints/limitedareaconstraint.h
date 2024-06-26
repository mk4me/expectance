/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_LIMITEDAREACONSTRAINT_H
#define _GEN_AI_LIMITEDAREACONSTRAINT_H

#include "evolution/constraint.h"

namespace ft
{
	/**
	 * Class LimitedAreaConstraint: this class represents constrait for limit area of motion
     * 
     **/
    class LimitedAreaConstraint : public Constraint 
    {
    public:
        LimitedAreaConstraint();
        virtual ~LimitedAreaConstraint(void);
        
        virtual bool Check(Avatar *av);
    };
}

#endif //_GEN_AI_LIMITEDAREACONSTRAINT_H
