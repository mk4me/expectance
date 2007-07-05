/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "translationmodifier.h"

using namespace ft;

TranslationModifier::TranslationModifier()
{
    m_vTranslation = CalVector(0,0,0);
    m_vRotation = CalQuaternion();
	
	tracer = dynamic_cast<TraceLine*>(ft::SceneManager::getInstance()->getObjectByName("TL1")); //for debug
}

void TranslationModifier::Apply(float elapsedSeconds,Avatar* avatar)
{
    TimeLineModifier::Apply(elapsedSeconds, avatar);

    // Here apply this modifier
       CalSkeleton *skel = avatar->GetCalModel()->getSkeleton();
        CalBone *bone = skel->getBone(0);

        CalVector currPos = bone->getTranslation();
        CalVector diff = currPos - m_vLastPos;

        CalQuaternion currRotatation = bone->getRotation();
//        CalQuaternion rotDiff = currRotatation - m_vLastRotation;
        
        
        if ( diff.length() > 50)
        {
            CalVector add = m_vLastPos;
            add.y = 0;
            m_vTranslation += add;
		tracer->AddPoint(m_vTranslation); //for debug
        }

        bone->setRotation(currRotatation * m_vRotation);

        m_vLastPos = currPos;;
        currPos += m_vTranslation;
        bone->setTranslation(currPos);
        bone->calculateState();
}

void TranslationModifier::Reset()
{
    TimeLineObject::Reset();

    m_vTranslation = CalVector(0,0,0);
    m_vRotation = CalQuaternion();
}