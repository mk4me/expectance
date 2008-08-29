    /*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "speedcontroller.h"
#include "../avatar/calavatar.h"
#include "../avatar/calavatartype.h"
#include "utility/vishelper.h"
#include "scene/scenemanager.h"
#include "../motion/animation.h"

using namespace ft;

/// \brief constructor
SpeedController::SpeedController()
{
    DRAW_SPEED_CURVE = ((Config::getInstance()->IsKey("speed_curve")) && (Config::getInstance()->GetIntVal("speed_curve")==1));

    if (DRAW_SPEED_CURVE)
    {
	    curve_min_speed = new DataCollector(toString() + "curve_min_speed");
        SceneManager::getInstance()->AddDataObject(curve_min_speed);
        curve_min_speed->HidePoints();       curve_min_speed->setColor(VisualizationHelper::COLOR_RED);
        curve_min_speed->setDrawScale(20);   curve_min_speed->setLegendLabel("min speed factor");

	    curve_max_speed = new DataCollector(toString() + "curve_max_speed");
        SceneManager::getInstance()->AddDataObject(curve_max_speed);
        curve_max_speed->HidePoints();       curve_max_speed->setColor(VisualizationHelper::COLOR_GREEN);
        curve_max_speed->setDrawScale(20);   curve_max_speed->setLegendLabel("max speed factor");

	    curve_curr_speed = new DataCollector(toString() + "curve_curr_speed");
        SceneManager::getInstance()->AddDataObject(curve_curr_speed);
        curve_curr_speed->HidePoints();       curve_curr_speed->setColor(VisualizationHelper::COLOR_YELLOW);
        curve_curr_speed->setDrawScale(20);   curve_curr_speed->setLegendLabel("current speed factor");

    }

    SPEEDFACTOR_CHANGE = 0.01f;

}

/// \brief destructor
SpeedController::~SpeedController(void)
{
    if(DRAW_SPEED_CURVE)
    {
        curve_min_speed->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_min_speed);

        curve_max_speed->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_max_speed);
        
        curve_curr_speed->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_curr_speed);
    }
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

    CalAvatar* av = (CalAvatar*)timeLineContext->getAvatar();

    float currSpeedFactor = av->getCurrSpeedFactor();
    float destSpeedFactor = av->getDestSpeedFactor();
    float minSpeedFactor;
    float maxSpeedFactor;
    

    //define currents limits
    if (timeLineContext->currMotion != NULL)
    {
		
		Animation* currMotion = ((CalAvatarType*)av->GetCalCoreModel())->GetMotion( timeLineContext->currMotion->getAnimName() );

        if (timeLineContext->exec_state == TimeLineContext::EXEC_STATE_OVERLAP && timeLineContext->prevMotion != NULL)
        {
			Animation* prevMotion = ((CalAvatarType*)av->GetCalCoreModel())->GetMotion( timeLineContext->prevMotion->getAnimName() );
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


    if(DRAW_SPEED_CURVE)
    {
        curve_curr_speed->AddValue(av->getCurrSpeedFactor());
        curve_min_speed->AddValue(minSpeedFactor);
        curve_max_speed->AddValue(maxSpeedFactor);
    }
}


/// \brief Resets parameters of this modifier
void SpeedController::Reset(TimeLineContext * timeLineContext)
{
//    TimeLineObject::Reset(timeLineContext);

    if(DRAW_SPEED_CURVE)
    {
        curve_curr_speed->Clear();
        curve_min_speed->Clear();
        curve_max_speed->Clear();
    }
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
