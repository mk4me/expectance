/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_LCS_MODIFIER_H
#define _GEN_LCS_MODIFIER_H

#include "timelinemodifier.h"

#include "scene/visualizationmanager.h"

namespace ft
{
	/**
	 * Class LCSModifier: specific TimeLineModifier responsible for global translation of avatar on the scene
	 **/
    class LCSModifier : public TimeLineModifier
    {
    public:
        bool TRACE_TRANSLATION; 
        bool TRACE_ANIM_ORIENT;   //sky-blue  
        bool TRACE_ROOT_ROTATION;   //white
        bool TRACE_FINAL_ORIENT;  //yellow
        bool TRACE_FINAL_DIR;     //purple
        bool TRACE_AXIS;   //red, green, blue
        bool LOCAL_DEBUG;

        bool INTERPOLATION;
        bool REST_TRANS_CALC;  //if rest of translation lost at anim changes should be applied
        bool ANIM_DIR_CALC; //if animation dir should affect global rotation

        LCSModifier();
        virtual ~LCSModifier(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

    private:
        void ApplyAnimDirectionToGlobalRotation(CalQuaternion& qGlobalRotOffset, CalVector& currPos, TimeLineContext * timeLineContext);
        CalQuaternion CalculateCurrentRootOrientAroundY(CalBone *rootBone);
//        void TraceRotation(TraceLine *traceLine, CalVector vBaseDir, CalVector pos, CalQuaternion rot, float line_length, CalVector vColor);
//        void TraceVector(TraceLine *traceLine, CalVector vVector, CalVector pos, float line_length, CalVector vColor);

        CalVector m_vLastPos;
        CalVector m_vLastPrevPos;  //used only for OVERLAP state

        CalVector m_vLastAnimDir;
        float m_fAnimRot;

		TraceLine *tracer_translation;

        TraceLine *tracer_X;
        TraceLine *tracer_Y;
        TraceLine *tracer_Z;

        TraceLine *tracer_root_orient;
        TraceLine *tracer_anim_orient;
        TraceLine *tracer_final_orient;
        TraceLine *tracer_final_dir;
       

        //temp variables
        int counter;
    };
};


#endif //_GEN_LCS_MODIFIER_H