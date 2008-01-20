 /*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */
#include "magnetcontroller.h"
#include "../motion/movableavatar.h"
#include "cdmanager.h"
#include "utility/mathutil.h"

#include "scene/scenemanager.h"
#include "utility/vishelper.h"


using namespace ft;

/// \brief constructor
MagnetController::MagnetController()
{
    DRAW_MAGNET_CURVE = ((Config::getInstance()->IsKey("magnet_curve")) && (Config::getInstance()->GetIntVal("magnet_curve")==1));

    if (DRAW_MAGNET_CURVE)
    {
	    curve_curr_angle = new DataCollector(toString() + "curve_curr_angle");
        SceneManager::getInstance()->AddDataObject(curve_curr_angle);
        curve_curr_angle->HidePoints();       curve_curr_angle->setColor(VisualizationHelper::COLOR_RED);
        curve_curr_angle->setDrawScale(0.2f);   curve_curr_angle->setLegendLabel("current angle");

	    curve_cummForce_angle = new DataCollector(toString() + "curve_cummForce_angle");
        SceneManager::getInstance()->AddDataObject(curve_cummForce_angle);
        curve_cummForce_angle->HidePoints();       curve_cummForce_angle->setColor(VisualizationHelper::COLOR_GREEN);
        curve_cummForce_angle->setDrawScale(0.2f);   curve_cummForce_angle->setLegendLabel("cummulative force angle");

		m_forceVector = new Line("ForceHelper");
		SceneManager::getInstance()->AddObject(m_forceVector);
		m_forceVector->setArrow(true).setColor(CalVector(1,0,0));

		m_directionVector = new Line("directionHelper");
		SceneManager::getInstance()->AddObject(m_directionVector);
		m_directionVector->setArrow(true);

		m_boundaryParameters = new CircularArc("boundaryHelper");
		SceneManager::getInstance()->AddObject(m_boundaryParameters);
		m_boundaryParameters->setRadius(50).setDisk(true).setColor(CalVector(1,1,1), 0.3f);

		m_boundaryScope = new Circle("boundaryHelper");
		SceneManager::getInstance()->AddObject(m_boundaryScope);
		m_boundaryScope->setRadius(800).setDisk(true).setSegmentsNumber(50).setColor(CalVector(0,0.001f,0.001f), 0.1f);	
	} 
}

/// \brief destructor
MagnetController::~MagnetController(void)
{
	if(DRAW_MAGNET_CURVE)
    {
        curve_curr_angle->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_curr_angle);

        curve_cummForce_angle->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_cummForce_angle);

		SceneManager::getInstance()->RemoveObject(m_forceVector);
		SceneManager::getInstance()->RemoveObject(m_directionVector);
		SceneManager::getInstance()->RemoveObject(m_boundaryParameters);
		SceneManager::getInstance()->RemoveObject(m_boundaryScope);
	}
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

	// get current cummulative force and direction angle
	CalVector _tmpCummForce  = CollisionDetectionManager::getInstance()->getObjectCummulativeForce(av->DynamicObjectID, 800.0);
	CalQuaternion _dirAngleQ = av->getGlobalRotationOffset();
	float _dirAngleValue = CalQuatToQuat(_dirAngleQ).Yangle();
	// compute angle for cummulative force vector
	float _forceAngValue = UTIL_GetVectorsAngle(_tmpCummForce,CalVector(1,0,0));
	
	float sign = UTIL_GetSignForDirChange(_tmpCummForce,CalVector(1,0,0));
	CalQuaternion _forceAngleQ = QuatToCalQuat( Quat(sign*_forceAngValue, Vec(0,1,0)) );
	// set new value for direction angle according to forceAngle
	_dirAngleQ.blend(0.1f, _forceAngleQ);

	// apply changes to global direciton
	//timeLineContext->getAvatar()->setGlobalRotationOffset(_dirAngleQ);

	//_dbg << "Wektor sily F=(" <<_tmpCummForce.x << _tmpCummForce.z << ") avatara" << av->getName() << std::endl;
	//_dbg << "Wektor sily F=(" <<_forceAngValue << endl; // << _direction.z << ") avatara" << av->getName() << std::endl;

	//

	//for (long int i = 0; i < CollisionDetectionManager::getInstance()->)
	//float currSpeedFactor = av->getCurrSpeedFactor();
    //float destSpeedFactor = av->getDestSpeedFactor();
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


    if(DRAW_MAGNET_CURVE)
    {
        curve_cummForce_angle->AddValue(RadToDeg(_forceAngValue));
        curve_curr_angle->AddValue(RadToDeg(_dirAngleValue));
		std::ostringstream st;
		st.precision(0);
		CalVector _start =  av->getPosition();
		CalVector _endDir = av->getDirection(); 
		CalVector _endForce =_tmpCummForce*100;
		_endDir*=50; // scale
		_endDir+=_start; // offset according to start point
		_endForce+= _start;
		_start.y = _endDir.y =  _endForce.y = 1;


		st << std::fixed << "Av (" << av->getPosition().x <<", " << av->getPosition().z <<")";
		st << " F["<< _endForce.length() <<"]" << std::endl;
		//string _hlpr = Avatar"x="+StringHelper::itos(av->getPosition().x) + "z="+StringHelper::itos(av->getPosition().z);
		av->setAnnotation(st.str());
		m_boundaryParameters->setEnd(av->getDirection());
		m_boundaryParameters->setStart(_tmpCummForce);
		m_boundaryParameters->setPosition(av->getPosition());
		m_boundaryScope->setPosition(av->getPosition());

		m_directionVector->setPosition(_start);
		m_directionVector->setEnd(_endDir);

		m_forceVector->setPosition(_start);
		m_forceVector->setEnd(_endForce);
	}
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
