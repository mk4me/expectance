/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GOALCONTROLLER_H
#define _FT_GOALCONTROLLER_H

#include "controller.h"

namespace ft
{
	/**
	 * An GoalController class - specific type of ft:Controller for AI use
	 * It is responsible for goal realization. It works as controller on timeline.
	 **/
	class EVOLUTION_API GoalController: public Controller
    {
	public:
		/**
	     * \brief Default constructor
		 */
        GoalController();
		/**
	     * \brief Default destructor
		 */
        virtual ~GoalController(void);

		/**
	     * \brief Called at every frame of simulation. Overrden from ft::Controller
		 * This callback can be overriden in deriven specific controller but it is recommended to 
		 * call from overriden method the base method form ft::GoalController class.
		 * \param float elapsedSeconds - time elapsed since last frame
		 * \param TimeLineContext* timeLineContext - context of timeline at given frame of simulation
         **/
        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

		/**
	     * \brief Callback called to request controller to reset its state Overrden from ft::Controller
		 * This callback is intended be overriden in deriven specific controller but it is recommended to 
		 * call from overriden method the base method form ft::GoalController class.
         * \param TimeLineContext* timeLineContext - context of timeline at given frame of simulation
         **/
        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children    

		/**
	     * \brief Sets flag defining of goal is reached or not
		 * \param bool isReached - true if goal is reached, false otherwise
		 */
        void setGoalReached(bool isReached) { m_goalReached = isReached; }

		/**
	     * \brief Returns flag defining of goal is reached or not
		 * \return bool - true if goal is reached, false otherwise
		 */
        bool isGoalReached() { return m_goalReached; } 

		/**
	     * \brief Returns string representation of goal controller 
		 * \return const std::string& - string representation
		 */
        virtual std::string toString();

    private:
        bool m_goalReached;
    };
}
#endif //_FT_GOALCONTROLLER_H
