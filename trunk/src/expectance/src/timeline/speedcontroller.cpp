    /*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "speedcontroller.h"
#include "../avatar/osgavatar.h"
#include "../avatar/osgavatartype.h"
#include "../motion/animation.h"

using namespace ft;

/// \brief constructor
SpeedController::SpeedController()
{
    SPEEDFACTOR_CHANGE = 0.01f;

}

/// \brief destructor
SpeedController::~SpeedController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void SpeedController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    Controller::Apply(elapsedSeconds, timeLineContext);

    OsgAvatar* av = (OsgAvatar*)timeLineContext->getAvatarImpl();

    float currSpeedFactor = av->getCurrSpeedFactor();
    float destSpeedFactor = av->getDestSpeedFactor();
    float minSpeedFactor;
    float maxSpeedFactor;
    

    //define currents limits
    if (timeLineContext->currMotion != NULL)
    {
		
		Animation* currMotion = ((OsgAvatar*)av)->getAvatarType()->GetMotion( timeLineContext->currMotion->getAnimName() );

        if (timeLineContext->exec_state == TimeLineContext::EXEC_STATE_OVERLAP && timeLineContext->prevMotion != NULL)
        {
			Animation* prevMotion = ((OsgAvatar*)av)->getAvatarType()->GetMotion( timeLineContext->prevMotion->getAnimName() );
            //INTERPOLATION
            float timeFactor =  timeLineContext->currAnimTime/timeLineContext->prevOverlap;
            minSpeedFactor = (1 - timeFactor)*prevMotion->getMinSpeedfactor() 
                                + timeFactor*currMotion->getMinSpeedfactor();
            maxSpeedFactor = (1 - timeFactor)*prevMotion->getMaxSpeedfactor() 
                                + timeFactor*currMotion->getMaxSpeedfactor();
        }
        else
        {
            minSpeedFactor = currMotion->getMinSpeedfactor();
            maxSpeedFactor = currMotion->getMaxSpeedfactor();
        }
    }
    else
    {
        minSpeedFactor = av->getSpeedFactorMin();
        maxSpeedFactor = av->getSpeedFactorMax();
    }

    //adjust dest speed to limits
    if (destSpeedFactor < minSpeedFactor)
        destSpeedFactor = minSpeedFactor;
    if (destSpeedFactor > maxSpeedFactor)
        destSpeedFactor = maxSpeedFactor;

    //go with currSpeed to destSpeed
    if (currSpeedFactor != destSpeedFactor)
    {
        if (currSpeedFactor < destSpeedFactor)
        {
            currSpeedFactor += SPEEDFACTOR_CHANGE;
            currSpeedFactor = currSpeedFactor > destSpeedFactor? destSpeedFactor : currSpeedFactor;
        }
        else 
        {
            currSpeedFactor -= SPEEDFACTOR_CHANGE;
            currSpeedFactor = currSpeedFactor < destSpeedFactor ? destSpeedFactor : currSpeedFactor;
        }
        av->setCurrSpeedFactor(currSpeedFactor);
    }

    av->setDestSpeedFactor(destSpeedFactor);
    av->setSpeedFactorMin(minSpeedFactor);
    av->setSpeedFactorMax(maxSpeedFactor);
}


/// \brief Resets parameters of this modifier
void SpeedController::Reset(TimeLineContext * timeLineContext)
{
//    TimeLineObject::Reset(timeLineContext);
}

/**
 * \brief Returns string representation of this SpeedController
 *
 * \return std::string - string representation
 **/
std::string SpeedController::toString()
{
	//TODO:  ERROR when uncommentd: in  dbgheap.c : _CrtIsValidHeapPointer function
//    std::string result = Controller::toString() + "[SpeedController]";
//    return result;
	return "SpeedController";
}
