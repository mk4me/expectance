/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */
#include "footdetector.h"
#include "cal3d/coretrack.h"

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
		tracer_L  = new TraceLine("leftFootMarker");
		tracer_L->setMarkerShape(ft_Cross);
		tracer_L->setBufferSize(20);
		tracer_L->setMarkerColor(CalVector(1,1,0));
		VisualizationManager::getInstance()->AddObject(tracer_L);
		tracer_R  = new TraceLine("rightFootMarker");
		tracer_R->setMarkerShape(ft_Cross);
		tracer_R->setBufferSize(20);
		tracer_R->setColor(CalVector(1,0,0));
		tracer_R->setMarkerColor(CalVector(1,1,0));
		VisualizationManager::getInstance()->AddObject(tracer_R);
	}
}


FootDetector::~FootDetector()
{
	if (FOOTPLANT_TRACE)
	{
	    tracer_L->ClearTrace();
		tracer_R->ClearTrace();
		VisualizationManager::getInstance()->RemoveObject(tracer_L);
		VisualizationManager::getInstance()->RemoveObject(tracer_R);
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
	CalVector _RFPos, _LFPos;
	CalVector _RTPos, _LTPos;

	CalSkeleton *_skel = timeLineContext->getAvatar()->GetCalModel()->getSkeleton();
	CalBone *_boneLFoot = _skel->getBone(38);
	CalBone *_boneLToe = _skel->getBone(39);
	CalBone *_boneRFoot = _skel->getBone(43);
	CalBone *_boneRToe = _skel->getBone(44);
	
	//init
	_LFPos = _boneLFoot->getTranslationAbsolute();
	_LTPos = _boneLToe->getTranslationAbsolute();
	_RFPos = _boneRFoot->getTranslationAbsolute();
	_RTPos = _boneRToe->getTranslationAbsolute();
	
	// for debug setting 
	// cout.precision(5);
	// std::cout<< "====>>> " << fixed << "dTCPy ="<<_LTPos.y<< " dFCPy ="<<_LFPos.y<<" dFCPx ="<<_LFPos.x<< " dFCPz ="<<_LFPos.z<<endl;
	
	if (timeLineContext->anim_changed) //change limits as motion has changed
	{
		std::string _animName = timeLineContext->currAnim->getCoreAnimation()->getFilename();
		const float *_fl = getLimits(_animName);
		if (_fl!=NULL)
		{
			footLimits[0] = _fl[0]; footLimits[1] = _fl[1]; footLimits[2] = _fl[2]; footLimits[3] = _fl[3]; 
		}
		else
			footLimits[0] = footLimits[1] = footLimits[2] = footLimits[3] = 1000; //when limit doesn't exist
	}
	
	if( ((footLimits[0] < _LFPos.y) && (_LFPos.y < footLimits[1])) && ((footLimits[2] < _LTPos.y) && (_LTPos.y < footLimits[3])))
	{
		_LTPos.y = 0;
	if (FOOTPLANT_TRACE)
			tracer_L->AddPoint(_LTPos); 
	}

	if( ((footLimits[0] < _RFPos.y) && (_RFPos.y < footLimits[1])) && ((footLimits[2] < _RTPos.y) && (_RTPos.y < footLimits[3])))
	{
		_RTPos.y = 0;
	if (FOOTPLANT_TRACE)
		tracer_R->AddPoint(_RTPos);
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