/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "lcsmodifier.h"

using namespace ft;

/// \brief constructor
LCSModifier::LCSModifier()
{
    m_vTranslation = CalVector(0,0,0);
	m_vLastPos = CalVector(0,0,0);
    m_translationInited = false;
    //m_vRotation = CalQuaternion();

    TRACE = false;

    if (TRACE)
    {
        tracer_translation = new TraceLine(toString() + "Translation");
        tracer_curr_pos = new TraceLine(toString() + "AnimPosition");
        SceneManager::getInstance()->AddObject(tracer_translation);
        SceneManager::getInstance()->AddObject(tracer_curr_pos);
    }	
}

/// \brief destructor
LCSModifier::~LCSModifier(void)
{
    if (tracer_translation != NULL)
    {
        tracer_translation->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_translation);
    }
    if (tracer_curr_pos != NULL)
    {
        tracer_curr_pos->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_curr_pos);
    }

}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 **/
void LCSModifier::Apply(float elapsedSeconds,Avatar* avatar)
{
    TimeLineModifier::Apply(elapsedSeconds, avatar);

    // Here apply this modifier
       CalSkeleton *skel = avatar->GetCalModel()->getSkeleton();
        CalBone *bone = skel->getBone(0);

        CalQuaternion currRotatation = bone->getRotation();
		currRotatation *= avatar->m_vRotation;
        bone->setRotation(currRotatation);

        
        CalVector currPos = bone->getTranslation();

        //std::cout << " z : " << currPos.z << std::endl;

        if (!m_translationInited)
        {
            if (TRACE)
            {
                tracer_translation->ClearTrace();
                tracer_curr_pos->ClearTrace();
            }

            m_vTranslation = avatar->getStartPosition(); // - currPos;
            
            m_vTranslation.y = currPos.y;
            m_vLastPos = currPos;

            m_translationInited = true;

            if (TRACE)
            {
                tracer_translation->AddPoint(m_vTranslation + CalVector(0,70,0));
                tracer_translation->AddPoint(m_vTranslation + CalVector(0,80,0));

                tracer_curr_pos->AddPoint(currPos + CalVector(0,70,0));
                tracer_curr_pos->AddPoint(currPos + CalVector(0,80,0));
            }
        }

        CalVector diff = currPos - m_vLastPos;

        diff *=avatar->m_vRotation;

        float diff_lenght = diff.length();

        if ( diff_lenght < 50)
        {
            m_vTranslation += diff;
            m_vTranslation.y = currPos.y;

            if (TRACE)
            {
                tracer_translation->AddPoint(m_vTranslation);
                tracer_curr_pos->AddPoint(currPos + CalVector(0,50,0));
            }
        }

        m_vLastPos = currPos;
        bone->setTranslation(m_vTranslation);

        bone->calculateState();
}

/// \brief Resets parameters of this modifier
void LCSModifier::Reset(Avatar* avatar)
{
    TimeLineObject::Reset(avatar);

    m_translationInited = false;

    if (TRACE)
    {
        tracer_translation->ClearTrace();
        tracer_curr_pos->ClearTrace();
    }
     m_vTranslation = CalVector(0,0,0);
    //m_vRotation = CalQuaternion();
}

