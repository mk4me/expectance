/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_LCS_MODIFIER_H
#define _GEN_LCS_MODIFIER_H

#include "timelinemodifier.h"


namespace ft
{
	/**
	 * Class LCSModifier: specific TimeLineModifier responsible for global translation of avatar on the scene
	 **/
    class LCSModifier : public TimeLineModifier
    {
    public:
        bool TRACE_TRANSLATION; 
        bool TRACE_TRANSFORM;   //sky-blue  
        bool TRACE_ROOT_ROTATION;   //white
        bool TRACE_TRANSFORM_END;  //yellow
        bool TRACE_FINAL_DIR;     //purple
        bool TRACE_AXIS;   //red, green, blue
        bool LOCAL_DEBUG;

        bool INTERPOLATION;
        bool REST_TRANS_CALC;  //if rest of translation lost at anim changes should be applied

        bool ANIM_DIR_CALC; //if animation dir should affect global rotation
        bool ANIM_DIR_CALC_FOR_CYCLES; //if animation dir should affect global rotation at the end of each cycle of loop anim

        LCSModifier();
        virtual ~LCSModifier(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);
        void UpdateRotation(float elapsedSeconds, TimeLineContext * timeLineContext);
        void UpdateTranslation(float elapsedSeconds, TimeLineContext * timeLineContext);


        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

    private:
        void ApplyAnimDirectionToGlobalRotation(TimeLineContext * timeLineContext);
        Transform* GetTransformForAnim(CalAnimation* anim, Avatar* avatar);
        Transform* GetTransformForType(Avatar* avatar);
        
        CalVector m_vLastPos;
        CalVector m_vLastPrevPos;  //used only for OVERLAP state

        CalVector m_vLastAnimDir;
        float m_fAnimRot;


        CalQuaternion m_qAnimDirOverlapStart;
        CalQuaternion m_qAnimDirOverlapDest;

   		DataCollector *curve_trans_Y;
        DataCollector *curve_root_rotation;


		TraceLine *tracer_translation;

        TraceLine *tracer_X;
        TraceLine *tracer_Y;
        TraceLine *tracer_Z;
        TraceLine *tracer_root_orient;
        TraceLine *tracer_transform;
        TraceLine *tracer_transform_end;
        TraceLine *tracer_final_dir;
       

        //temp variables
        int counter;
    };
};


#endif //_GEN_LCS_MODIFIER_H