/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _SCENE_TRANSFORM_H
#define _SCENE_TRANSFORM_H

#include "../core/baseobject.h"
#include "traceline.h"


namespace ft
{
	/* A Transform class 
	 * Base class for objects responsible for transformation of motion data from original for to application scene
	 */
    class ENGINE_API Transform : public BaseObject
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
        const CalQuaternion& getForwardDiff() { return m_qForwardDiff; }
        void setForwardDiff(const CalQuaternion& forwardDiff) { m_qForwardDiff = forwardDiff; }

        const CalQuaternion& getEndForwardDiff() { return m_qForwardEndDiff; }
        void setEndForwardDiff(const CalQuaternion& endForwardDiff) { m_qForwardEndDiff = endForwardDiff; }

        const CalQuaternion& getAnimEndDiff() { return m_qAnimEndDiff; }
        void setAnimEndDiff(const CalQuaternion& animEndDiff) { m_qAnimEndDiff = animEndDiff; }


        const std::string getType() { return m_type; }

        void InitForType(const CalVector& origPos, const CalQuaternion& origQuat, bool source_3dsmax);
        void InitForAnim(CalCoreAnimation* coreAnim, bool source_3dsmax);
        
        float CalculateAnimForward(CalCoreAnimation* coreAnim, int iterCount, bool from_begin);

        TraceLine* getTraceLine() {return m_forward_trace; }
        void Trace(const CalVector& pos);
    private:
        void Init(const CalVector& origPos, float forwardAngle, float endForwardAngle, bool source_3dsmax);
        std::string m_type;

        CalVector m_vOrigPosition;  //original position
        CalVector m_vPosOffset; //position offset 

        CalVector m_vOrigForward; //original forward vector
        CalQuaternion m_qForwardDiff; //difference angle between original forward vector and scene vector

        CalQuaternion m_qForwardEndDiff; //difference angle between animation forward vector at the and of anim and scene vector (only for motions)

        CalQuaternion m_qAnimEndDiff;

        bool m_rotationFrom3dsmax;   //flag if there was rotation -90 deg. around X (caused by 3ds max source)

        TraceLine* m_forward_trace;
    };
}

#endif //_SCENE_TRANSFORM_H
