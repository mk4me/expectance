/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_RULE_H
#define _GEN_AI_RULE_H

#include "constraint.h"
#include "goal.h"

namespace ft
{
	/**
	 * Class Rule: this class represents the rule whic connects contraint with goal
     * 
     **/
    class Rule : public virtual BaseObject
    {
    public:
        Rule(Constraint* constraint, Goal* goal) { setConstraint(constraint); setGoal(goal); }
        virtual ~Rule(void) { /* empty */ }

        void setConstraint(Constraint* constraint) { m_constraint = constraint; }
        Constraint* getConstraint() { return m_constraint; }

        void setGoal(Goal* goal) { m_goal = goal; }
        Goal* getGoal() { return m_goal; }

    private:
        Constraint* m_constraint;
        Goal* m_goal;
    };
}

#endif //_GEN_AI_RULE_H
