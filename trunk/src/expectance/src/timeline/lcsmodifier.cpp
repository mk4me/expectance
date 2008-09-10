/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */
#include "lcsmodifier.h"
#include "cal3d/coretrack.h"
#include <math.h>
#include "../avatar/osgavatar.h"
#include "../utility/stringhelper.h"

#include "gil/gil_animexecution.h"

using namespace ft;

/// \brief constructor
LCSModifier::LCSModifier()
{
}

/// \brief destructor
LCSModifier::~LCSModifier(void)
{
}


/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void LCSModifier::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    UpdateRotation(elapsedSeconds, timeLineContext);
    UpdateTranslation(elapsedSeconds, timeLineContext);
}

////////////////// ROTATION ///////////////////////////////

void LCSModifier::UpdateRotation(float elapsedSeconds, TimeLineContext * timeLineContext)
{
	//TODO:  implement rotation calculation
	OsgAvatar* avImpl = (OsgAvatar*)timeLineContext->getAvatarImpl();
}

//////////////////// TRANSLATION ///////////////////////////////

void LCSModifier::UpdateTranslation(float elapsedSeconds, TimeLineContext * timeLineContext)
{
	//TODO:  implement rotation calculation	
	OsgAvatar* avImpl = (OsgAvatar*)timeLineContext->getAvatarImpl();

	CalBone *bone = avImpl->getOsgModel()->getCalModel()->getSkeleton()->getBone(0);

	if (timeLineContext->currAnim != NULL)
	{
		//  use anim transform here
	}

    
	CalVector currPos;
	
	if (timeLineContext->currAnim != NULL)
	{
		Cal3dAnimExecution* animExec = static_cast<Cal3dAnimExecution*>(timeLineContext->currAnim);
		CalAnimation* anim = dynamic_cast<CalAnimation*>(  animExec->getAnimation() );
		Transform* currTransform = GetTransformForAnim(anim, static_cast<OsgAvatar*>(timeLineContext->getAvatarImpl()));

		CalAnimation* currAnim = ((Cal3dAnimExecution*)timeLineContext->currAnim)->getAnimation();
		CalVector trans(bone->getTranslation());
		CalVector  origPos = currTransform->getOrigPosition();
		trans.x -= origPos.x; //kierunek - w lewo
		trans.y -= origPos.y; //kierunek -do tylu
		//trans.x += 300; // w lewo
		//trans.y += 300; // do tylu
		//trans.z += 200;  // do gory
		bone->setTranslation(trans);
		bone->calculateState();

		if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle)
		{
			CalVector diff;
			if (timeLineContext->anim_changed)
			{
				avImpl->setGlobalRotation( osg::Quat(2,osg::Vec3d(0,0,1)) ); //
				std::cout << " anim changed " << endl;
				if (timeLineContext->prevMotion != NULL)
				{
					GIL_AnimData* animData = timeLineContext->prevMotion->getAnimData();
					Transform* prevTransform = static_cast<MotionData*>(animData)->getTransform();
					diff = prevTransform->getEntireTranslation();
					std::cout << " --- prev motion not null " << endl;
				}
				if (timeLineContext->prevAnim != NULL)
				{
/*					Cal3dAnimExecution* prevAnimExec = static_cast<Cal3dAnimExecution*>(timeLineContext->prevAnim);
					CalAnimation* prevAnim = dynamic_cast<CalAnimation*>(  prevAnimExec->getAnimation() );
					Transform* prevTransform = GetTransformForAnim(anim, static_cast<OsgAvatar*>(timeLineContext->getAvatarImpl()));
					diff = prevTransform->getEntireTranslation();
					std::cout << " --- prev not null " << endl;
			*/

				}
				
			}
			else
			{
				diff = currTransform->getEntireTranslation();
				std::cout << " new ceycle " << endl;
			}

	
			osg::Vec3d pos = avImpl->getPosition();
			osg::Vec3d newPos = osg::Vec3d(pos.x() + diff.x, pos.y() + diff.y, pos.z() + diff.z);
			avImpl->setPosition(newPos);
		}
	}

	if (timeLineContext->currAnim != NULL)
	{
		CalAnimation* currAnim = ((Cal3dAnimExecution*)timeLineContext->currAnim)->getAnimation();
		CalVector tmpPos;
		CalQuaternion tmpRot;
		CalCoreAnimation* coreAnim = currAnim->getCoreAnimation();
		coreAnim->getCoreTrack(0)->getState(timeLineContext->currAnimTime, tmpPos, tmpRot);
		static int counter = 0;

		if (counter%10==0)
		{
			osg::Vec3d pos = avImpl->getPosition();
//			std::cout << " anim (" << tmpPos.x << " , " << tmpPos.y << " , " << tmpPos.z << ")" 
//				<< std::endl << "  _osg(" << pos.x() << " , " << pos.y() << " , " << pos.z() << ")" << std::endl;
		}
		counter++;
	}

	
/*    if (timeLineContext->currAnim != NULL)
	{
		CalAnimation* currAnim = ((Cal3dAnimExecution*)timeLineContext->currAnim)->getAnimation();
		CalCoreAnimation* coreAnim = currAnim->getCoreAnimation();
		coreAnim->getCoreTrack(0)->getState(timeLineContext->currAnimTime, currPos, tmpRot);

        if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle)
        {
            m_vLastPos = currPos;
        }
	}
    else
    {
        //if (LOCAL_DEBUG) _dbg << " Original pos " << endl;
        currPos = bone->getTranslation();
//        if (currTransform != NULL)
//        {
//            currPos += currTransform->getPosOffset();
//        }
        m_vLastPos = currPos;
    }

    CalVector diff = currPos - m_vLastPos;
    float y_to_set = currPos.y;

	avImpl->

    CalVector vCurrAvatarPosition = avImpl->getPosition();
    vCurrAvatarPosition += diff;
    vCurrAvatarPosition.y = y_to_set;   

    m_vLastPos = currPos;
*/

}

 Transform* LCSModifier::GetTransformForAnim(CalAnimation* anim, OsgAvatar* avatar)
{
    Transform* resultTransform = NULL;

	OsgAvatarType* type = avatar->getAvatarType();
    Animation* mot = type->GetMotion( StringHelper::GetFileNameForPath(anim->getCoreAnimation()->getFilename()));

    if (mot != NULL && mot->getAnimData() != NULL && ((MotionData*)mot->getAnimData())->getTransform() != NULL)
    {
		resultTransform  = ((MotionData*)mot->getAnimData())->getTransform();
    }
    else
    {
//        if (GenDebug::ERR)
//            _dbg << GenDebug::ERR_STR << "LCSModifier::GetTransformForAnim: empty motion or transform for curr anim " << anim->getCoreAnimation()->getFilename() << endl;
    }

    return resultTransform;
}

