/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TRANSLATION_MODIFIER_H
#define _GEN_TRANSLATION_MODIFIER_H

#include "timelinemodifier.h"
#include "../utility/Cal3dMathsConversion.h"

namespace ft
{
    class TranslationModifier : public TimeLineModifier
    {
    public:
        TranslationModifier();
        virtual ~TranslationModifier(void) { /* empty */ }

        void Apply(float elapsedSeconds, Avatar* avatar);
    private:
        CalVector m_vTranslation;
        CalVector m_vLastPos;
        CalQuaternion m_vRotation;

    };
};


#endif //_GEN_TRANSLATION_MODIFIER_H