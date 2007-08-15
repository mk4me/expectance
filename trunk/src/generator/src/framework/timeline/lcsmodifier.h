/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_LCS_MODIFIER_H
#define _GEN_LCS_MODIFIER_H

#include "timelinemodifier.h"

#include "../scene/scenemanager.h"

namespace ft
{
	/**
	 * Class LCSModifier: specific TimeLineModifier responsible for global translation of avatar on the scene
	 **/
    class LCSModifier : public TimeLineModifier
    {
    public:
        bool TRACE;

        LCSModifier();
        virtual ~LCSModifier(void);

        void Apply(float elapsedSeconds, Avatar* avatar);

        virtual void Reset(Avatar* avatar); // resets current object and its children

    private:
        bool m_translationInited;
        CalVector m_vTranslation;
        CalVector m_vLastPos;
        //CalQuaternion m_vRotation;
		TraceLine *tracer_translation;
        TraceLine *tracer_curr_pos;

    };
};


#endif //_GEN_LCS_MODIFIER_H