/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_RULE_H
#define _FT_RULE_H

#include "goal.h"
#include "constraint.h"

namespace ft
{
	/**
	 * An Rule class - this class represents the rule which connects contraint with goal
	 **/
	class EVOLUTION_API Rule: public Entity
    {
    public:
		/**
	     * \brief Constructor
         * \param Constraint* constraint - constraint related to this rule
		 * \param Goal* goal - goal which will be reached if constrained is satisfied
         **/
        Rule(Constraint* constraint, Goal* goal) { setConstraint(constraint); setGoal(goal); }

		/**
	     * \brief Default destructor
		 */
        virtual ~Rule(void) { /* empty */ }

		/**
	     * \brief Sets constraint related to this rule
         * \param Constraint* constraint - constraint related to this rule
		 */
        void setConstraint(Constraint* constraint) { m_constraint = constraint; }

		/**
	     * \brief Returns constraint related to this rule
         * \return Constraint* constraint - constraint related to this rule
		 */
        Constraint* getConstraint() { return m_constraint; }

		/**
	     * \brief Sets goal which will be reached if constrained is satisfied
         * \param Goal* goal - goal to set
		 */
        void setGoal(Goal* goal) { m_goal = goal; }

		/**
	     * \brief Returns goal which will be reached if constrained is satisfied
         * \return Goal* - goal related to this rule
		 */
        Goal* getGoal() { return m_goal; }

    private:
        Constraint* m_constraint;
        Goal* m_goal;
    };
}
#endif //_FT_RULE_H
