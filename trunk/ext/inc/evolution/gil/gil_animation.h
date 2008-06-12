/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GIL_ANIMATION_H
#define _FT_GIL_ANIMATION_H

#include "gil_interface.h"
#include "gil_animdata.h"

namespace ft { namespace gil {

	//!An interface for animation from GIL.
	/*! 
	 * 
	 */
	class EVOLUTION_API GIL_Animation : public GIL_Interface
    {
	public:
		/**
	 	 * \brief Identifies feature of possibility of playing cyclic animation in environment
		 */
		static const std::string FEATURE_CYCLIC_ANIMATION;

		/**
	 	 * \brief Identifies feature of possibility of manage weights for animation in environment
		 */
		static const std::string FEATURE_WEIGHTS;

		/**
	 	 * \brief Returns animation name
		 * \return std::string - animation name
		 */
        virtual std::string getAnimName() = 0;

		/**
	 	 * \brief Returns animation identifier
		 * \return std::string - animation identifier
		 */
        virtual int getAnimID() = 0;

		/**
	 	 * \brief Informs if this object points real animation in environment 
		 * \return bool - true if this object points real animation, false otherwise
		 */
        virtual bool isNullAnim() = 0; 

		/**
	 	 * \brief Returns animation data object that can wrap any data from environment
		 * \return GIL_AnimData* - animation data object
		 */
		virtual GIL_AnimData* getAnimData() = 0;

		/**
	 	 * \brief Returns animation duration 
		 * \return float - animation duration
		 */
		virtual float getDuration() = 0;

		/**
	 	 * \brief Checks if given feature is available in environment
		 * \param const std::string& - string identifier of feature
		 * \return bool = true if given feature is available, false otherwise
		 */
		virtual bool isFeature(const std::string&) = 0;

    };
}  } //namespace ft::gil
#endif //_FT_GIL_ANIMATION_H