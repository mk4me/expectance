/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */
#include "footdetector.h"
#include "cal3d/coretrack.h"
#include "../app/gendebug.h"

using namespace ft;

FootDetector::FootDetector(void)
{
    if ((Config::getInstance()->IsKey("track_on")) && (Config::getInstance()->GetIntVal("track_on")==1))
    {
		FOOTPLANT_TRACE = ((Config::getInstance()->IsKey("track_footplant")) 
							&& (Config::getInstance()->GetIntVal("track_footplant")==1)) ? true: false;
	} 
	else 
		FOOTPLANT_TRACE = false;

	if (FOOTPLANT_TRACE)
	{
		m_tracerL  = new TraceLine("leftFootMarker");
		m_tracerL->setMarkerShape(ft_Cross);
		m_tracerL->setBufferSize(20);
		m_tracerL->setMarkerColor(CalVector(1,1,0));
		VisualizationManager::getInstance()->AddObject(m_tracerL);
		m_tracerR  = new TraceLine("rightFootMarker");
		m_tracerR->setMarkerShape(ft_Cross);
		m_tracerR->setBufferSize(20);
		m_tracerR->setColor(CalVector(1,0,0));
		m_tracerR->setMarkerColor(CalVector(1,1,0));
		VisualizationManager::getInstance()->AddObject(m_tracerR);
	}
	
	m_leftFootPlant = m_rightFootPlant = false;   
}


FootDetector::~FootDetector()
{
	if (FOOTPLANT_TRACE)
	{
	    m_tracerL->ClearTrace();
		m_tracerR->ClearTrace();
		VisualizationManager::getInstance()->RemoveObject(m_tracerL);
		VisualizationManager::getInstance()->RemoveObject(m_tracerR);
	}

}

/**
 * \brief Applies this detector at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this detector is assigned
 **/
void FootDetector::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);
	
	// Here apply this detector
	CalVector _RHPos, _LHPos;
	CalVector _RTPos, _LTPos;

	CalSkeleton *_skel = timeLineContext->getAvatar()->GetCalModel()->getSkeleton();
	CalBone *_boneLFoot = _skel->getBone(38);
	CalBone *_boneLToe = _skel->getBone(39);
	CalBone *_boneRFoot = _skel->getBone(43);
	CalBone *_boneRToe = _skel->getBone(44);
	
	//init
	_LHPos = _boneLFoot->getTranslationAbsolute();
	_LTPos = _boneLToe->getTranslationAbsolute();
	_RHPos = _boneRFoot->getTranslationAbsolute();
	_RTPos = _boneRToe->getTranslationAbsolute();
	
	// for debug setting 
	// cout.precision(5);
	// std::cout<< "====>>> " << fixed << "dTCPy ="<<_LTPos.y<< " dFCPy ="<<_LHPos.y<<" dFCPx ="<<_LHPos.x<< " dFCPz ="<<_LHPos.z<<endl;
	
	if (timeLineContext->anim_changed) //change limits as motion has changed
	{
		std::string _animName = timeLineContext->currAnim->getCoreAnimation()->getFilename();
		const float *_fl = getLimits(_animName);
		if (_fl!=NULL)
		{
			//  HeelminY				   HeelmaxY				  ToeminY					ToemaxY
			m_footLimits[0] = _fl[0]; m_footLimits[1] = _fl[1]; m_footLimits[2] = _fl[2]; m_footLimits[3] = _fl[3]; 
		}
		else
			m_footLimits[0] = m_footLimits[1] = m_footLimits[2] = m_footLimits[3] = 1000; //when limit doesn't exist
	}
	
	//LEFT FOOT
	if( ((m_footLimits[0] < _LHPos.y) && (_LHPos.y < m_footLimits[1])) && ((m_footLimits[2] < _LTPos.y) && (_LTPos.y < m_footLimits[3])))
	{
		if (!m_leftFootPlant)
		{
			m_leftFootPlant = true;
			timeLineContext->getAvatar()->getLocalMsgSender()->SendMsg(new Message(MSG_DETECTOR_LEFT_FOOT_ON_THE_FLOOR, 
																	   new MessageParam(true)), true);

            if (GenDebug::FOOTSTEP>0)
            {
			    _dbg << "====>>> Left foot on the ground <<<=====" << endl;
            }
		}

		if (FOOTPLANT_TRACE)
		{
			_LTPos.y = 0;
			m_tracerL->AddPoint(_LTPos); 
		}
	}
	else
		if (m_leftFootPlant) //left foot siwng phase
		{
			m_leftFootPlant = false;
			timeLineContext->getAvatar()->getLocalMsgSender()->SendMsg(new Message(MSG_DETECTOR_LEFT_FOOT_ON_THE_FLOOR, 
																	   new MessageParam(false)), true);
            if (GenDebug::FOOTSTEP>0)
            {
			    _dbg << "====>>> Left foot swinging <<<=====" << endl;
            }
		}

	// RIGHT FOOT
	if( ((m_footLimits[0] < _RHPos.y) && (_RHPos.y < m_footLimits[1])) && ((m_footLimits[2] < _RTPos.y) && (_RTPos.y < m_footLimits[3])))
	{
		if (!m_rightFootPlant)
		{
			m_rightFootPlant = true;
			timeLineContext->getAvatar()->getLocalMsgSender()->SendMsg(new Message(MSG_DETECTOR_RIGHT_FOOT_ON_THE_FLOOR,
																	   new MessageParam(true)), true);
			// std::cout<< "====>>> Right foot on the ground <<<=====" << endl;
		}
			
		if (FOOTPLANT_TRACE)
		{
			_RTPos.y = 0;
			m_tracerR->AddPoint(_RTPos);
		}
	}
	else
		if (m_rightFootPlant)  // right foot swing phase
		{
			m_rightFootPlant = false;
			timeLineContext->getAvatar()->getLocalMsgSender()->SendMsg(new Message(MSG_DETECTOR_RIGHT_FOOT_ON_THE_FLOOR,
																	   new MessageParam(false)), true);

		}
}

bool FootDetector::AddLimits(const std::string& motionName, const float* limits)
{
    //float *_tmp = new float[4];
	std::string _id = motionName;
	if (!_id.empty())
	{
	 	std::map<std::string,const float*>::iterator it = m_limits.find(_id);

		if ( it!=m_limits.end())
        { 
            if (Debug::ERR)
                _dbg << Debug::ERR_STR << "FootDetector::AddLimits limits for motion " << _id << " already added to it's FootDetector " << std::endl;
			return false;
		}
		else
		{
			//_tmp[0] = limits[0]; _tmp[1] = limits[1]; _tmp[2] = limits[2]; _tmp[3] = limits[3];
			m_limits.insert( std::make_pair( std::string(_id), limits) );
		}
	}

//    if (Debug::MOTION>0)
//        _dbg << " FootDetector::AddLimits limits for motion " << _id << " added to it's FootDetector " << std::endl;

	return true;
}

const float* FootDetector::getLimits(const std::string &motionName)
{
	const float* _limits = NULL;
	std::map<std::string,const float*>::iterator it = m_limits.find(motionName);
	if ( it!=m_limits.end()) { 
        _limits = it->second;
    }
    return _limits;
}