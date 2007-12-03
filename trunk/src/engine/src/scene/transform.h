/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _SCENE_TRANSFORM_H
#define _SCENE_TRANSFORM_H

#include <string>
#include "../utility/mathconversions.h"

namespace ft
{
	/* A Transform class 
	 * Base class for objects responsible for transformation of motion data from original for to application scene
	 */
    class Transform
    {
    public:
        Transform(const std::string type); 
	    virtual ~Transform(void);
        virtual void Dump();

        bool isRoationFrom3dsmax() { return m_rotationFrom3dsmax; }
        void setRoationFrom3dsmax(bool set) { m_rotationFrom3dsmax = set; }

        const CalVector& getOrigPosition() { return m_vOrigPosition; }
        void setOrigPosition(const CalVector& origPos) { m_vOrigPosition = origPos; }
        const CalVector& getPosOffset() { return m_vPosOffset; }
        void setPosOffset(const CalVector& posOffset) { m_vPosOffset = posOffset; }
        const CalVector& getOrigForward() { return m_vOrigForward; }
        void setOrigForward(const CalVector& origForward) { m_vOrigForward = origForward; }
        const CalVector& getForwardDiff() { return m_vForwardDiff; }
        void setForwardDiff(const CalVector& forwardDiff) { m_vForwardDiff = forwardDiff; }

        const std::string getType() { return m_type; }
    private:
        std::string m_type;

        CalVector m_vOrigPosition;  //original position
        CalVector m_vPosOffset; //position offset 

        CalVector m_vOrigForward; //original forward vector
        CalVector m_vForwardDiff; //original forward vector

        bool m_rotationFrom3dsmax;   //flag if there was rotation -90 deg. around X (caused by 3ds max source)
    };
};

#endif //_SCENE_TRANSFORM_H
