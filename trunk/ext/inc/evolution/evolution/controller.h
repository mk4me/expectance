/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CONTROLLER_H
#define _FT_CONTROLLER_H

#include "timelinecontext.h"
#include "timelineobject.h"

namespace ft
{
	/**
	 * An Controller class - can be used to modify motions at every frame of its execution
	 * 
	 **/
	class EVOLUTION_API Controller: public TimeLineObject
    {
	public:

		/**
	     * \brief Called at every frame of simulation. Inside this method the whole job of controller 
		 * should be done.
		 * This callback can be overriden in deriven specific controller but it is recommended to 
		 * call from overriden method the base method form ft::Controller class.
		 * \param float elapsedSeconds - time elapsed since last frame
		 * \param TimeLineContext* timeLineContext - context of timeline at given frame of simulation
         **/
        virtual void Apply(float elapsedSeconds, TimeLineContext* timeLineContext) = 0;


		/**
	     * \brief Callback called to request controller to reset its state
		 * This callback is intended be overriden in deriven specific controller but it is recommended to 
		 * call from overriden method the base method form ft::Controller class.
         * \param TimeLineContext* timeLineContext - context of timeline at given frame of simulation
         **/
        virtual void Reset(TimeLineContext* timeLineContext) 
					{ // add functionality in dereived classes
					};

		/**
	     * \brief Release all resources related to this controller object
		 * This callback can be overriden in deriven specific controller but it is recommended to 
		 * call from overriden method the base method form ft::Controller class.
         **/
		virtual void Destroy(void) 
					{ // add functionality in dereived classes
					};

		/**
	     * \brief Returns string representation of controller
		 * \return std::string - string representation
		 */
        virtual std::string toString(); 
    };

}
#endif //_FT_CONTROLLER_H
