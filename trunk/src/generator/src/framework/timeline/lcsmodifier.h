/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_LCS_MODIFIER_H
#define _GEN_LCS_MODIFIER_H

#include "timelinemodifier.h"

#include "../scene/visualizationmanager.h"

namespace ft
{
	/**
	 * Class LCSModifier: specific TimeLineModifier responsible for global translation of avatar on the scene
	 **/
    class LCSModifier : public TimeLineModifier
    {
    public:
        bool TRACE;
        bool LOCAL_DEBUG;
        bool INTERPOLATION;
        bool REST_TRANS_CALC;  //if rest of translation lost at anim changes should be applied

        LCSModifier();
        virtual ~LCSModifier(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);

        virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

    private:
        bool m_translationInited;
        CalVector m_vTranslation;
        CalVector m_vLastPos;
        CalVector m_vLastPrevPos;  //used only for OVERLAP state
        //CalQuaternion m_vRotation;
		TraceLine *tracer_translation;
        TraceLine *tracer_curr_pos;

        

        //temp variables
        int counter;
        int currAnimID;
        float currAnimTime;
    };
};


#endif //_GEN_LCS_MODIFIER_H