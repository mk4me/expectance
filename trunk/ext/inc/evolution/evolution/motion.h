/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_MOTION_H
#define _FT_MOTION_H

#include "blender.h"

namespace ft
{
	/**
	 * An Motion class - represents distinguishable part of the timeline
	 **/
	class EVOLUTION_API Motion: public TimeLineObject
    {
	public:
		/**
	     * \brief Default constructor
		 */
        Motion(void);

		/**
	     * \brief Default destructor
		 */
        virtual ~Motion(void) { /* empty */ }

		/**
	     * \brief Release all resources related to this motion object
         **/
        virtual void Destroy(void);

		/**
	     * \brief Assigns animation to motion
		 * \param GIL_Animation* animation - animation to assign 
         **/
        void setAnimation(GIL_Animation* animation) { m_motionRef = animation; }

		/**
	     * \brief Returns animation assigned to motion
		 * \return GIL_Animation* animation - animation to assign 
         **/
        GIL_Animation* getAnimation() { return m_motionRef; } 

		/**
	     * \brief Sets blender to motion
		 * \param Blender* blender - blender to set
         **/
        void setBlender(Blender* blender) { m_blender = blender; } 

		/**
	     * \brief Returns blender of this motion
		 * \return Blender* - blender of this motion
         **/
        Blender* getBlender() { return m_blender; }

		/**
	     * \brief Sets flag defining that animalion assigned to this motion is cyclic 
		 * \param bool set - flag defining that animation is cyclic
         **/
        void setAnimLoop(bool set) { m_animLoop = set; }

		/**
	     * \brief Returns flag defining that animalion assigned to this motion is cyclic 
		 * \return bool - true if assigned animation is cyclic, false otherwise
         **/
        bool isAnimLoop() { return m_animLoop; }

		/**
	     * \brief Sets number of loops for cyclic animation
		 * \param int number - number of loops
         **/
        void setLoopNumber(int number) { m_loopNumber = number; }

		/**
	     * \brief Returns number of loops for cyclic animation
		 * \return int - number of loops
         **/
        int getLoopNumber() { return m_loopNumber; } 

		/**
	     * \brief Sets flag defining that if previous motin on timeline is cyclic it should be interrupted after 
		 * finishing current loop
		 * \param bool set - flag defining motion is interrupting
         **/
        void setInterupting(bool set) { m_interrupting = set; }

		/**
	     * \brief Returns flag defining that if previous motin on timeline is cyclic it should be interrupted after 
		 * finishing current loop
		 * \return bool - true if motion is interrupting, false otherwise 
         **/
        bool isInterupting() { return m_interrupting; }

		/**
	     * \brief Dumps current state of motion. Ovveriden from ft::TimeLineObject
		 * \param int depth - indentation in dumping text
		 */
        void Dump(int depth);

		/**
	     * \brief Returns string representation of motion
		 * \return const std::string& - string representation
		 */
        virtual std::string toString(); 

		/**
	     * \brief Creates new motion object which can be used for clonning of this object
         * \return TimeLineObject* - new motion object
         **/
        virtual TimeLineObject* CreateInstance();

		/**
	     * \brief Clones this object
         * \return TimeLineObject* - new motion object tha is clone of this one
         **/
        virtual TimeLineObject* Clone();

    protected:
        GIL_Animation* m_motionRef; //reference to motion which is represented by this object
        Blender* m_blender;

        bool m_animLoop;  // if the motion is looped
        int m_loopNumber; // defines length of looped motion by number of cycles (if value is -1 it is infinite)
        bool m_interrupting;  // indicates if this motion should immediately interuppt the previous motion
    };

}
#endif //_FT_MOTION_H
