/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GOAL_H
#define _FT_GOAL_H

#include "goalcontroller.h"
#include "avatar.h"
#include "entity.h"

namespace ft
{
    static const int PRIORITY_LOW      = 0; 
    static const int PRIORITY_MEDIUM   = 1;
    static const int PRIORITY_HIGH     = 2;
    static const int PRIORITY_CRITICAL = 3;

	/**
	 * An Goal class - represents goal of avatar which is the base element of avatar behaviour
	 * 
	 **/
	class EVOLUTION_API Goal: public Entity
    {
	public:
		/**
	     * \brief Default constructor
		 */
		Goal(void);
		/**
	     * \brief Default destructor
		 */
		virtual ~Goal(void);

		/**
	     * \brief Release all resources related to this controller object
		 * This callback can be overriden in deriven specific goal but it is recommended to 
		 * call from overriden method the base method form ft::Goal class.
         **/
        virtual void Destroy(void);

		/**
	     * \brief Executes action related to this goal 
		 * \param Avatar* av - avatar for whic action is to perform
		 * \return bool - true if action executed successfuly, false otherwise
		 */
        virtual bool ExecuteAction(Avatar* av);

		/**
	     * \brief Returns action related to this goal 
		 * \return id - identifier of action
		 */
        virtual int getActionToPerform();

		/**
	     * \brief Creates controller that will be used to reach this goal.
		 * This callback is intended to be overriden in deriven specific goal.
		 * \return id - identifier of action
		 */
        virtual GoalController* CreateController(Avatar* av);

		/**
	     * \brief Sets priority for this goal
		 * \param int priority - priority of goal
		 */
        void setPriority(int priority) { m_priority = priority; }

		/**
	     * \brief Returns priority of this goal
		 */
        int getPriority() { return m_priority; }

    private:
        int m_priority;
    };

}
#endif //_FT_GOAL_H
