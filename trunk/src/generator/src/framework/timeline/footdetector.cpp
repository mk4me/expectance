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
	
	//debug for setting 
	//cout.precision(5);
	//std::cout<< "====>>> " << fixed << "dTCPy ="<<_LTPos.y<< " dFCPy ="<<_LFPos.y<<" dFCPx ="<<_LFPos.x<< " dFCPz ="<<_LFPos.z<<endl;

	//chod  Toe < 0 && 5 < Foot < 6
	if((_LTPos.y < 0) && ((5<_LFPos.y)&&(_LFPos.y<6)))
	{
		_LTPos.y = 0;
	if (FOOTPLANT_TRACE)
			tracer_L->AddPoint(_LTPos); 
	}

	if((_RTPos.y < 0) && ((5<_RFPos.y)&&(_RFPos.y<6)))
	{
		_RTPos.y = 0;
	if (FOOTPLANT_TRACE)
		tracer_R->AddPoint(_RTPos);
	}

	//bieg  Toe < 2 && 11 < Foot < 13
	if((_LTPos.y < 3) && ((11<_LFPos.y)&&(_LFPos.y<14)))
	{
		_LTPos.y = 0;
	if (FOOTPLANT_TRACE)
		tracer_L->AddPoint(_LTPos);
	}

	if((_RTPos.y < 3) && ((11<_RFPos.y)&&(_RFPos.y<14)))
	{
		_RTPos.y = 0;
	if (FOOTPLANT_TRACE)
		tracer_R->AddPoint(_RTPos);
	}
}