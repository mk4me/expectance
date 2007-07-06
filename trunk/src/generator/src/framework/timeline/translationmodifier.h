/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TRANSLATION_MODIFIER_H
#define _GEN_TRANSLATION_MODIFIER_H

#include "timelinemodifier.h"

#include "../scene/scenemanager.h"

namespace ft
{
    class TranslationModifier : public TimeLineModifier
    {
    public:
        bool TRACE;

        TranslationModifier();
        virtual ~TranslationModifier(void);

        void Apply(float elapsedSeconds, Avatar* avatar);

        virtual void Reset(); // resets current object and its children

    private:
        bool m_translationInited;
        CalVector m_vTranslation;
        CalVector m_vLastPos;
        //CalQuaternion m_vRotation;
		TraceLine *tracer_translation;
        TraceLine *tracer_curr_pos;

    };
};


#endif //_GEN_TRANSLATION_MODIFIER_H