 /*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */
#include "magnetcontroller.h"
#include "cdmanager.h"
#include "utility/mathutil.h"
#include "../avatar/calavatar.h"
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
		m_boundaryScope->setRadius(50).setDisk(false).setSegmentsNumber(50).setColor(CalVector(0,0.001f,0.001f), 0.5f);	
	} 

	setThreshold(800);
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
	//algorithm for one frame
	//1. get current the smallest difference angle between cummulative force and direction vector
	//2. check sign
	//3. set turn letf if sign==-1 otherwise set turn right
	//Everything is working under condition that cummulative force exists (is > 0)

	double _currLenForce, _currArcF2D;
	float sign = 0;
	_currLenForce = _currArcF2D = sign = 0;

	Controller::Apply(elapsedSeconds, timeLineContext);
	
	CalAvatar* av = (CalAvatar*)timeLineContext->getAvatarImpl();

	CalVector _cummForce  = CollisionDetectionManager::getInstance()->getObjectCummulativeForce(av->DynamicObjectID, m_threshold);
	CalQuaternion _dirAngleQ = av->getGlobalRotationOffset();
	float _dirAngleValue = CalQuatToQuat(_dirAngleQ).Yangle();
	_currLenForce = _cummForce.length();
	if (_currLenForce > 0)
	{
		CalVector _currDir = av->getDirection();
	
		//(1.)
		_currArcF2D = UTIL_GetVectorsAngle(_currDir, _cummForce);
		//(2.)
		sign = UTIL_GetSignForDirChange(_currDir,_cummForce);
	
		CalQuaternion _currArcF2DQ = QuatToCalQuat( Quat(sign*_currArcF2D, Vec(0,1,0)) );

		double _cmFA = _currLenForce*0.1f;

		// set new value for direction angle according to forceAngle
		_dirAngleQ.blend(_cmFA, _currArcF2DQ);

		// apply changes to global direciton
		av->setGlobalRotationOffset(_dirAngleQ);
	}


    if(DRAW_MAGNET_CURVE)
    {
        curve_cummForce_angle->AddValue(RadToDeg(_currArcF2D));
        curve_curr_angle->AddValue(RadToDeg(_dirAngleValue));
		std::ostringstream st;
		st.precision(0);
		CalVector _o =  av->getPosition();
		CalVector _direction = av->getDirection(); 
		CalVector _force =_cummForce*100;
		_direction*=50; // scale
		_direction+=_o; // offset according to start point
		_force+= _o;
		_o.y = _direction.y =  _force.y = 1;


		st << std::fixed << "Av (" << av->getPosition().x <<", " << av->getPosition().z <<")";
		st << " F["<< _force.length() <<"], sign[" << _currLenForce << "]"<<std::endl;
		//av->setAnnotation(st.str());
		if (sign==-1) {
			m_boundaryParameters->setEnd(av->getDirection());
			m_boundaryParameters->setStart(_cummForce);
		}
		else
		{
			m_boundaryParameters->setStart(av->getDirection());
			m_boundaryParameters->setEnd(_cummForce);

		}
		m_boundaryParameters->setPosition(av->getPosition());
		m_boundaryScope->setPosition(av->getPosition());

		m_directionVector->setPosition(_o);
		m_directionVector->setEnd(_direction);

		m_forceVector->setPosition(_o);
		m_forceVector->setEnd(_force);
	}
}


/// \brief Resets parameters of this modifier
void MagnetController::Reset(TimeLineContext * timeLineContext)
{
//    TimeLineObject::Reset(timeLineContext);

}

/**
 * \brief Returns string representation of this MagnetController
 *
 * \return std::string - string representation
 **/
std::string MagnetController::toString()
{
    std::string result = Controller::toString() + "[MagnetController]";
    return result;
}
