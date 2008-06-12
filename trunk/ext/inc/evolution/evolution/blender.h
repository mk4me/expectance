/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_BLENDER_H
#define _FT_BLENDER_H

#include "timelineobject.h"

namespace ft
{
	/**
	 * An Blender class - defines blending between two consequtive motions. Generally by overriding this method in 
	 * deriven class it can be used to define more aspects of connection between two motions.
	 **/
	class EVOLUTION_API Blender: public TimeLineObject
    {
    public:
		/**
	     * \brief Constructor
         * \param float overlap - blending duration 
         **/
        Blender(float overlap) { m_overlap = overlap; } 

		/**
	     * \brief Default destructor
         **/
        virtual ~Blender(void) { /* empty */ }

		/**
	     * \brief Sets blending duration
         * \param float overlap - new blending duration 
         **/
        void setOverlap(float overlap) { m_overlap = overlap; }

		/**
	     * \brief Returns blending duration defined by this blender
         * \return float - blending duration 
         **/
        float getOverlap() { return m_overlap; }

		/**
	     * \brief Creates new Blender object which can be used for clonning of this object
         * \return TimeLineObject* - new Blender object
         **/
        virtual TimeLineObject* CreateInstance();

		/**
	     * \brief Clones this object
         * \return TimeLineObject* - new Blender object tha is clone of this one
         **/
        virtual TimeLineObject* Clone();

    private:
        float m_overlap;
    };

}
#endif //_FT_BLENDER_H
