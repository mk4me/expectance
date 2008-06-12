/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_AVATAR_H
#define _FT_AVATAR_H

#include "../gil/gil_avatar.h"
#include "controller.h"
#include "motion.h"

namespace ft
{
	/**
	 * An Avatar class - base interface for all humanoid characters. It represents avatar in Evolution
	 * package. For each avatar its implementation in environment (e.g. Game) can be extracted by Avatar::getImplementation() mathod.
	 **/
	class EVOLUTION_API Avatar
    {
    public:

		/**
	     * \brief Switch ON or OFF the AI functionality for this Avatar
         * \param bool - if true the AI is turn ON, otherwise the AI is turn OFF
         **/
		virtual void SetThink(bool think) = 0;

		/**
	     * \brief Returns implementation of this Avatar created in environment (e.g. Game) - under GIL layer
         * \return GIL_Avatar* - implementation of Avatar under GIL
         **/
		virtual GIL_Avatar* getImplementation() = 0;

		/**
	     * \brief Starts simulation of Avatar. It causes starting simulation of motion and AI
         **/
		virtual void StartSimulation() = 0;

		/**
	     * \brief Extracts animation of given name from implementation of Avatar in environment (under GIL)
		 * \param const std::string& animName - name of extracted animation
         * \return GIL_Avatar* - implementation of Avatar under GIL
         **/
		virtual GIL_Animation* getAnimation(const std::string& animName) = 0;

		/**
	     * \brief Sets interrupting flag for all motions on timeline that have been not executed yet
         **/
		virtual void InterruptUnexecutedMotions() = 0;

		/**
	     * \brief Removes all motions form timeline that have been not executed yet
         **/
		virtual void RemoveUnexecutedMotions() = 0;

		/**
	     * \brief Request avatar to execute action of given idetifier
         * \Param int id* - identifier of requested action
         **/
		virtual bool SetActionById(int id) = 0;

		/**
	     * \brief returns identifier of action that is currently being executed by avatar.
         * \return int id* - identifier of current action or -1 if none action is currently being executed.
         **/
		virtual int getCurrActionId() = 0;

		/**
	     * \brief Adds motion on the timeline of this avatar. Motion is added at the and of motions sequence.
         * \return Motion* motion - motion to add
         **/
		virtual void AddMotion(Motion* motion) = 0;

		/**
	     * \brief Adds controller to the timeline of this avata
         * \return Controller* controller - controller to add
         **/
		virtual void AddController(Controller* controller) = 0;

		/**
	     * \brief Pass control event to avatar.
         * \return int controlEvent - controller to add
         **/
		virtual void ControlEvent(int controlEvent) = 0;

    };
}

#endif //_FT_AVATAR_H
