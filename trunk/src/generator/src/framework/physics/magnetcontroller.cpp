 /*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */
#include "magnetcontroller.h"
#include "../motion/movableavatar.h"
//#include "utility/vishelper.h"
#include "cdmanager.h"
using namespace ft;

/// \brief constructor
MagnetController::MagnetController()
{
 
}

/// \brief destructor
MagnetController::~MagnetController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void MagnetController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);

    MovableAvatar* av = (MovableAvatar*)timeLineContext->getAvatar();

    float currSpeedFactor = av->getCurrSpeedFactor();
    float destSpeedFactor = av->getDestSpeedFactor();
    //float minSpeedFactor;
    //float maxSpeedFactor;
    

    //define currents limits
    //if (timeLineContext->currMotion != NULL)
    //{
    //    if (timeLineContext->exec_state == EXEC_STATE_OVERLAP && timeLineContext->prevMotion != NULL)
    //    {
    //        //INTERPOLATION
    //        float timeFactor =  timeLineContext->currAnimTime/timeLineContext->prevOverlap;
    //        minSpeedFactor = (1 - timeFactor)*timeLineContext->prevMotion->getMinSpeedfactor() 
    //                            + timeFactor*timeLineContext->currMotion->getMinSpeedfactor();
    //        maxSpeedFactor = (1 - timeFactor)*timeLineContext->prevMotion->getMaxSpeedfactor() 
    //                            + timeFactor*timeLineContext->currMotion->getMaxSpeedfactor();
    //    }
    //    else
    //    {
    //        minSpeedFactor = timeLineContext->currMotion->getMinSpeedfactor();
    //        maxSpeedFactor = timeLineContext->currMotion->getMaxSpeedfactor();
    //    }

    //}
    //else
    //{
    //    minSpeedFactor = av->getSpeedFactorMin();
    //    maxSpeedFactor = av->getSpeedFactorMax();
    //}

    ////adjust dest speed to limits
    //if (destSpeedFactor < minSpeedFactor)
    //    destSpeedFactor = minSpeedFactor;
    //if (destSpeedFactor > maxSpeedFactor)
    //    destSpeedFactor = maxSpeedFactor;

    ////go with currSpeed to destSpeed
    //if (currSpeedFactor != destSpeedFactor)
    //{
    //    if (currSpeedFactor < destSpeedFactor)
    //    {
    //        currSpeedFactor += SPEEDFACTOR_CHANGE;
    //        currSpeedFactor = currSpeedFactor > destSpeedFactor? destSpeedFactor : currSpeedFactor;
    //    }
    //    else 
    //    {
    //        currSpeedFactor -= SPEEDFACTOR_CHANGE;
    //        currSpeedFactor = currSpeedFactor < destSpeedFactor ? destSpeedFactor : currSpeedFactor;
    //    }
    //    av->setCurrSpeedFactor(currSpeedFactor);
    //}

    //av->setDestSpeedFactor(destSpeedFactor);
    //av->setSpeedFactorMin(minSpeedFactor);
    //av->setSpeedFactorMax(maxSpeedFactor);


    //if(DRAW_SPEED_CURVE)
    //{
    //    curve_curr_speed->AddValue(av->getCurrSpeedFactor());
    //    curve_min_speed->AddValue(minSpeedFactor);
    //    curve_max_speed->AddValue(maxSpeedFactor);
    //}
}


/// \brief Resets parameters of this modifier
void MagnetController::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

}

/**
 * \brief Returns string representation of this MagnetController
 *
 * \return std::string - string representation
 **/
std::string MagnetController::toString()
{
    std::string result = TimeLineModifier::toString() + "[MagnetController]";
    return result;
}
