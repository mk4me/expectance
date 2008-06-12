/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_CONTEXT_H
#define _GEN_TIMELINE_CONTEXT_H

#include "gil/gil_avatar.h"
#include "gil/gil_animation.h"
#include "gil/gil_animexecution.h"

namespace ft
{
	using namespace gil;

	/**
	 * Class TimeLineContext: represents set of parameters affecting TimeLine execution
	 **/
    class EVOLUTION_API TimeLineContext
    {
    public:

		//EVENTS from TimeLineExecutor

		/**
	     * \brief No event at current frame from TimeLineExecutor
		 */
		static const int EXEC_EVENT_NONE                 = 0; 

		/**
	     * \brief Event that state of TimeLineExecutor changed at current frame of simulation
		 */
		static const int EXEC_EVENT_STATE_CHANGED        = 1; 


		//STATES of TimeLineExecutor

		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_NOT_INITED = 0; 
		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_WAIT = 1; 
		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_SINGLE = 2;
		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_OVERLAP = 3;
		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_FADE_IN = 4;
		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_FADE_OUT = 5;
		/**
	     * \brief Possible State of TimeLineExecutor
		 */
		static const int EXEC_STATE_TERMINATED = 6;

		/**
	     * \brief Default constructor
		 */
        TimeLineContext(void) { /* empty */ }

		/**
	     * \brief Default destructor
		 */
        virtual ~TimeLineContext(void) { /* empty */ }

		/**
	     * \brief Sets pointer to implementation of avatar in environment
		 * \param GIL_Avatar* avatar - pointer to avatar implementation
		 */
        void setAvatar(GIL_Avatar* avatar) { m_avatar = avatar; }

		/**
	     * \brief Returns pointer to implementation of avatar in environment
		 * \return GIL_Avatar* - pointer to avatar implementation
		 */
		GIL_Avatar* getAvatar() { return m_avatar; }

		/**
	     * \brief Flag defines if motions should be removed from timline after their execution
		 */
        bool remove_after_execution;
		/**
	     * \brief Flag defines if current motion should be finished immediately after entry to 'terminated' state
		 */
        bool stop_immediate;

        // RUNTIME parameters

		/**
	     * \brief Motion that is predecessor of currently being executed on timeline
		 */
        GIL_Animation* prevMotion;
		/**
	     * \brief Animation assigned to motion that is predecessor of currently being executed on timeline
		 */
        GIL_AnimExecution* prevAnim;
		/**
	     * \brief Motion currently being executed on timeline
		 */
        GIL_Animation* currMotion;
		/**
	     * \brief Animation assigned to motion currently being executed on timeline
		 */
        GIL_AnimExecution* currAnim;

		/**
	     * \brief Execution time of animation assigned to motion that is predecessor of currently being executed on timeline
		 */
        float prevAnimTime;
		/**
	     * \brief Duration of animation assigned to motion that is predecessor of currently being executed on timeline
		 */
        float prevAnimDuration;

		/**
	     * \brief Execution time of animation assigned to motion currently being executed on timeline
		 */
        float currAnimTime;
		/**
	     * \brief Duration of animation assigned to motion currently being executed on timeline
		 */
        float currAnimDuration;

		/**
	     * \brief Blending time for motion that is predecessor of currently being executed on timeline
		 */
        float prevOverlap;
		/**
	     * \brief Blending time for motion currently being executed on timeline
		 */
        float currOverlap;

		/**
	     * \brief Current state of TimeLineExecutor
		 */
        int exec_state;
		/**
	     * \brief Event from TimeLineExecutor at current frame of simulation
		 */
        int exec_event;
		/**
	     * \brief Information that animation changed at current frame of simulation
		 */
        bool anim_changed;
		/**
	     * \brief Information that new cycle of started at current frame of simulation
		 */
        bool anim_new_cycle;
		/**
	     * \brief Information that animation stopped at current frame of simulation
		 */
        bool anim_stopped;

	private:
        GIL_Avatar* m_avatar;
    };
}

#endif //_GEN_TIMELINE_CONTEXT_H
