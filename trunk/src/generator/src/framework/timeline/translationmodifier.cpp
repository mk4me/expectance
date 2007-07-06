/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "translationmodifier.h"

using namespace ft;

TranslationModifier::TranslationModifier()
{
    m_vTranslation = CalVector(0,0,0);
	m_vLastPos = CalVector(0,0,0);
    //m_vRotation = CalQuaternion();
	
	tracer = dynamic_cast<TraceLine*>(ft::SceneManager::getInstance()->getObjectByName("TL1")); //for debug
}


void TranslationModifier::Apply(float elapsedSeconds,Avatar* avatar)
{
    TimeLineModifier::Apply(elapsedSeconds, avatar);

    // Here apply this modifier
       CalSkeleton *skel = avatar->GetCalModel()->getSkeleton();
        CalBone *bone = skel->getBone(0);

        CalQuaternion currRotatation = bone->getRotation();
		currRotatation *= avatar->m_vRotation;
        bone->setRotation(currRotatation);

        CalVector currPos = bone->getTranslation();

        CalVector diff = currPos - m_vLastPos;

        diff *=avatar->m_vRotation;

        float diff_lenght = diff.length();

        if ( diff_lenght < 50)
            m_vTranslation += diff;

        m_vLastPos = currPos;
        currPos += m_vTranslation;
		
        bone->setTranslation(m_vTranslation);

        bone->calculateState();
}

void TranslationModifier::Reset()
{
    TimeLineObject::Reset();

     m_vTranslation = CalVector(0,0,0);
    //m_vRotation = CalQuaternion();
}