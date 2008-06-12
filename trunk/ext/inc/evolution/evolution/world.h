/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_WORLD_H
#define _FT_WORLD_H

#include "avatar.h"
#include "rule.h"

#include <string>

/** 
* \brief API exposed for use by environment (e.g. Game) in which *Evolution* package is used.
*/
namespace ft
{
	/**
	 * An World class - keeps references to all avatars on the scene
	 **/
    class EVOLUTION_API World
    {
	public:
		/**
	 	 * \brief Adds avatar to world
		 *
         * \param Avatar * av - avatar to add
		 * \return bool - true if avatar added successfuly, false otehrwise
		 */
		virtual bool AddAvatar(Avatar * av) = 0;

		/**
	 	 *\brief Removes avatar from world
		 *
         *\param Avatar * av - avatar to remove
		 *\return bool - true if avatar removed successfuly, false otehrwise
		 */
		virtual bool RemoveAavatar(Avatar * av) = 0;

		/**
	 	 *  \brief Removes avatar from world
		 *
         * \param const std::string& id - id of avatar to remove
		 * \return bool - true if avatar removed successfuly, false otehrwise
		 */
		virtual bool RemoveAavatar(const std::string& id) = 0;

		/**
	     * \brief Switch ON the AI functionality in world
         **/
		virtual void StartThinking()=0;

		/**
	     * \brief Adds goal to possible goals that avatars can try to reach
		 * \param Goal* pGoal - goal to add
         **/
        virtual bool AddGoal(Goal* pGoal) = 0;

		/**
	     * \brief Remove goal from possible goals that avatars can try to reach
		 * \param Goal* pGoal - goal to remove
         **/
        virtual bool RemoveGoal(Goal* pGoal) = 0;

		/**
	     * \brief Adds rule which avatars in his world should obey
		 * \param Rule* rule - rule to add
         **/
        virtual bool AddRule(Rule* rule) = 0;

		/**
		 * \brief This method should be called at every frame of simulation
		 *
		 * \param float elapsedSeconds - time elapsed sice last frame update
		 **/
		virtual void Update(const float elapsedSeconds) = 0;

    };

}
#endif //_FT_WORLD_H
