/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_CONSTRAINT_H
#define _GEN_AI_CONSTRAINT_H

#include "../action/actionavatar.h"

namespace ft
{
	/**
	 * Class Constraint: this class represents constarint for avatar behaviour (element of behaviour rule)
     * 
     **/
    class Constraint
    {
    public:
        Constraint();
        virtual ~Constraint(void);
        virtual void Destroy(void);
        
        virtual bool Check(ActionAvatar *av);
    };
}

#endif //_GEN_AI_CONSTRAINT_H

