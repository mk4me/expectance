/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CONSTRAINT_H
#define _FT_CONSTRAINT_H

#include "avatar.h"

namespace ft
{
	/**
	 * An Constraint class - represents constraint for avatar behaviour (element of behaviour rule)
	 **/
    class EVOLUTION_API Constraint
    {
	public:
		/**
	     * \brief Default constructor
         **/
        Constraint();

		/**
	     * \brief Default destructor
         **/
        virtual ~Constraint(void);

		/**
	     * \brief Release all resources related to this oconstraint object
		 * This callback can be overriden in deriven specific constraint but it is recommended to 
		 * call from overriden method the base method form ft::Constraint class.
         **/
        virtual void Destroy(void);

		/**
	     * \brief Callback to check if given avatar satisfies this constraint
		 * This callback can be overriden in deriven specific constraint but it is recommended to 
		 * call from overriden method the base method form ft::Constraint class.
		 * \param Avatar *av - avatar for which this constraint is to be checked
		 * return bool - true if avatar satisfies constraint, false otherwise
         **/
        virtual bool Check(Avatar *av);
    };

}
#endif //_FT_CONSTRAINT_H
