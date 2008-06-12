/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GIL_AVATAR_H
#define _FT_GIL_AVATAR_H

#include "gil_interface.h"
#include "gil_animation.h"
#include "gil_animexecution.h"

namespace ft { namespace gil  {

	//!An interface for avatar in environment.
	/*!
	 * 
	 */
	class EVOLUTION_API GIL_Avatar : public GIL_Interface
    {
	public:
		static const std::string iface_name;

		/**
		 * \brief This method is called at every frame of simulation after animation update
		 *
		 * \param float elapsedSeconds - time elapsed sice last update
		 **/
		virtual void Update(const double elapsedSeconds) = 0;

		/**
		 * \brief Gets animation with given name
		 *
		 * \param std::string animName - name of animation
		 * \return ft::GIL_Animation* - requested animation or NULL if the is no animation with this name for this avatar
		 **/
		virtual GIL_Animation* getAnimation(const std::string& animName) = 0;

		/**
		 * \brief Request avatar to play once animation of given identifier
		 * \param int animID - indentifier of animation
		 * \param int fade_in - time of fading in 
		 * \param int fade_out - time of fading out
		 * \return ft::GIL_AnimExecution* - animation execution objec
		 **/
		virtual GIL_AnimExecution* PlayAnimation(int animID, float fade_in, float fade_out) = 0;

		/**
		 * \brief Request avatar to stop animation of given identifier
		 * \param int animID - indentifier of animation
		 * \param int fade_out - time of fading out
		 **/
		virtual void StopAnimation(int animID, float fade_out) = 0;
		
		/**
		 * \brief Request avatar to play cyclic animation of given identifier
		 * \param int animID - indentifier of animation
		 * \param int fade_in - time of fading in 
		 * \param int fade_out - time of fading out
		 * \return ft::GIL_AnimExecution* - animation execution objec
		 **/
		virtual GIL_AnimExecution* PlayCyclicAnimation(int animID, float fade_in) = 0;

		/**
		 * \brief Request avatar to stop cyclic animation of given identifier
		 * \param int animID - indentifier of animation
		 * \param int fade_out - time of fading out
		 **/
		virtual void StopCyclicAnimation(int animID, float fade_out) = 0;

		/**
		 * \brief Check if animation execution object is still valid for avatar
		 * \param GIL_AnimExecution* animExec - animation execution object
		 * \return bool - true if execution object is still valid, false otherwise
		 **/
		virtual bool isAnimValid(GIL_AnimExecution* animExec) = 0;

		/**
		 * \brief Returns time of animation playing currenlty by avatar
		 * \param GIL_AnimExecution* animExec - animation execution object
		 * \return float  - animation time
		 **/
		virtual float getAnimTime(GIL_AnimExecution* anim_exec) = 0;

		/**
		 * \brief Returns duration of animation playing currenlty by avatar
		 * \param GIL_AnimExecution* animExec - animation execution object
		 * \return float  - animation duration
		 **/
		virtual float getAnimDuration(GIL_AnimExecution* anim_exec) = 0;

		/**
		 * \brief Returns speed factor which influences on simulation speed
		 * \return float  - speed factor 
		 **/
		virtual float getCurrSpeedFactor() = 0;

		/**
		 * \brief Checks if there is any animation being played at current frame of simulation
		 * \return bool - true if any animation id being played, false otherwise
		 **/
		virtual bool isEmptyFrame() = 0;

    };
}  } //namespace ft::gil
#endif //_FT_GIL_AVATAR_H