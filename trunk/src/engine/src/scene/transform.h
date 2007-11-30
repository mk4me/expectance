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

        const std::string getType() { return m_type; }
    private:
        std::string m_type;

        CalVector m_vOrigPosition;  //original position
        CalVector m_vPosOffset; //position offset 

        CalVector m_vOrigForward; //original forward vector
        CalVector m_vForwardDiff; //original forward vector
    
    };
};

#endif //_SCENE_TRANSFORM_H
