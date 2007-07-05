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

        CalVector currPos = bone->getTranslation();


        CalQuaternion currRotatation = bone->getRotation();
//        CalQuaternion rotDiff = currRotatation - m_vLastRotation;
        
        // rotate root bone quaternion
        //Quat addRot = Quat(degToRad(-30.0f), Vec(0,1,0));
		//m_vRotation = QuatToCalQuat(addRot);
		//CalQuaternion y_axis_90(0.0f, 0.7071067811f,0.0f, 0.7071067811f);
        
		currRotatation *= avatar->m_vRotation;
        bone->setRotation(currRotatation);
        // rotate root bone displacement



        //bone->setRotation(currRotatation * m_vRotation);

        CalVector diff = currPos - m_vLastPos;
		m_vTranslation.y = 0;
        m_vLastPos = currPos;
        diff *=avatar->m_vRotation;

		m_vTranslation += diff;


       
		//if ( diff.length() > 50)
        {
            //CalVector add = m_vLastPos;
            //add.y = 0;
            //m_vTranslation += add;
            //m_vTranslation = m_vLastPos;
			tracer->AddPoint(m_vTranslation); //for debug
        }

        currPos = m_vTranslation;
		bone->setTranslation(currPos);
        bone->calculateState();
}

void TranslationModifier::Reset()
{
    TimeLineObject::Reset();

    m_vTranslation = CalVector(0,0,0);
    //m_vRotation = CalQuaternion();
}