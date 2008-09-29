/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "stopcontroller.h"
#include "../avatar/osgavatar.h"
#include "evolution/dataprovider.h"
#include "../utility/mathutil.h"

using namespace ft;

StopController::StopController(void)
{
	//STOPCONTROLLER_TRACE = ((Config::getInstance()->IsKey("stopcontroller_trace")) && (Config::getInstance()->GetIntVal("stopcontroller_trace")==1));
	//if (STOPCONTROLLER_TRACE)
	//{
	//	m_rootRFootVector = new Line("rightFootHelper");
	//	SceneManager::getInstance()->AddObject(m_rootRFootVector);
	//	m_rootRFootVector->setArrow(true).setColor(CalVector(1,0,0));

	//	m_directionVector = new Line("directionHelper");
	//	SceneManager::getInstance()->AddObject(m_directionVector);
	//	m_directionVector->setArrow(true);
	//}

	setStop(false);
}

StopController::~StopController(void)
{
	//if (STOPCONTROLLER_TRACE)
	//{
	//	SceneManager::getInstance()->RemoveObject(m_rootRFootVector);
	//	SceneManager::getInstance()->RemoveObject(m_directionVector);
	//}
}


/**
 * \brief Applies this controller at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this controller is assigned
 **/
void StopController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
	//TODO: uncomment it
//    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);


	//pierwsza wersja algorytm uproszczony.
	//1. w kazdym kroku sprawdzamy, czy noga jest z przodu jesli tak to czekamy (return) inaczej robimy interpolacje

	if (isStopped())
	{
		//OsgAvatar* av = (OsgAvatar*)timeLineContext->getAvatarImpl();
		Avatar* av = timeLineContext->getAvatar();
		//_dbg << "Stop signal was sent" << std::endl;
		
		if (isFrontLeg(av)) // while any foot is front of the root do nothing
			return;	
		else
		{   // reset states and interpolate
			setStop(false); // reset signal state after full usage
			// interpoluj ostatecznie (tutaj trzeba chyba dolozyc jeszcze parametr dotyczacy polozenia animacji IDLE)
			Motion* idle = DataProvider::getInstance()->getMotion(av->getType(), "idle"); // wez ruch docelowy
			av->AddMotion(idle->Clone());    // dodaj do timeline
			//Motion* mot=av->getCurrMotion(); // pobierz biezacy ruch
			//Blender* bl=mot->getBlender();   // pobierz blender biezacego ruchu
			//bl->setOverlap(0.5);			 // i ustaw go na 0.5 sec
			av->StopCurrentAnimation();		 // zatrzymaj biezaca animacje wedlug ustawionych wartosci
		}
	}
}

/**
 * \brief Returns string representation of this Controller
 *
 * \return std::string - string representation
 **/
std::string StopController::toString()
{
    std::string result = /*Controller::toString() + */ "[StopController]";
    return result;
}

/**
 * \brief Returns true if any leg is in front of avatar otherwise false
 *
 * \return std::boolean - 
 **/
bool StopController::isFrontLeg(Avatar* av)
{
	//algorithm
	//1. get current the smallest difference angle between root-foot vector (BC) and direction vector (AC)
	//  A
	// / \
	//  |
	//  B - > C
	// BA - current vector between arms of skeleton
	// BC - vector between heel (left or right) and root
    // check sign of mutual location both vectors

	// prepare for one leg at first
	OsgAvatar* cav = static_cast<OsgAvatar*>(av->getImplementation());
	
	CalSkeleton *_skel = cav->getOsgModel()->getCalModel()->getSkeleton();

	//init by defaults to freebie
	int _LFootIndex = 66;
	int	_RFootIndex = 73;
	int _LArmIndex = 14;
	int _RArmIndex = 39;
	int _RootIndex = 2 ;

	// TODO resolve indexing problem in more universal way
	if(av->getType().compare("freebie")==0)
	{
		_LFootIndex = 66;
		_RFootIndex = 73;
		_LArmIndex = 14;
		_RArmIndex = 39;
		_RootIndex = 2;
	}
	else if (av->getType().compare("cally")==0)
	{
		_LFootIndex = 38;
		_RFootIndex = 43;
		_LArmIndex = 12;
		_RArmIndex = 24;
		_RootIndex = 2;
	}
	CalBone *_boneLFoot = _skel->getBone(_LFootIndex); // freebie 66 cally 38
	CalBone *_boneRFoot = _skel->getBone(_RFootIndex); // freebie 73 cally 43
	CalBone *_boneLArm = _skel->getBone(_LArmIndex);   // freebie 14 cally 12
	CalBone *_boneRArm = _skel->getBone(_RArmIndex);   // freebie 39 cally 24
	CalBone *_boneRoot = _skel->getBone(_RootIndex);   // freebie 2 cally 2

	CalVector _RFootPos = _boneRFoot->getTranslationAbsolute(); 
	CalVector _LFootPos = _boneLFoot->getTranslationAbsolute(); 
	CalVector _RArmPos  = _boneRArm->getTranslationAbsolute();  
	CalVector _LArmPos  = _boneLArm->getTranslationAbsolute();  
	CalVector _RootPos  = _boneRoot->getTranslationAbsolute();

	// calculate vectors lrarms (from left ro right arm) and root2RightFoot and root2LeftFoot
	CalVector _LRArmsDir = _RArmPos - _LArmPos;      // base skeleton vector (between arms)
	CalVector _root2RFoot = _RFootPos - _RootPos;	 // root to right foot position vector
	CalVector _root2LFoot = _LFootPos - _RootPos;    // root to left foot position vector
	

//	double _currArcF2D;
	float _signL, _signR = 0;
	bool _isfront = false;
	CalVector normal2VectorSurface =  CalVector(0,0,1); //(coordinate system XZY)
	////if (STOPCONTROLLER_TRACE)
	////{
	////	// visualization
	////	_LArmPos.y=_RArmPos.y=_RootPos.y=_RFootPos.y=1; // on the common plane
	////	m_directionVector->setPosition(_LArmPos);
	////	m_directionVector->setEnd(_RArmPos);
	////	m_rootRFootVector->setPosition(_RootPos);
	////	m_rootRFootVector->setEnd(_LFootPos);
	////}
	
	_LRArmsDir.z = _root2RFoot.z = _root2LFoot.z = 0;
	_LRArmsDir.normalize();
	_root2RFoot.normalize();
	_root2RFoot.normalize();
	_signL = UTIL_GetSignForDirChange( _root2LFoot, _LRArmsDir, normal2VectorSurface );
	_signR = UTIL_GetSignForDirChange( _root2RFoot, _LRArmsDir, normal2VectorSurface );
	//if (_signL == _signR)
	//	std::cout << "ERROR wrong signs between wectors.."<< std::endl; // mijaja sie i moga wtedy miec wspolny znak
	if (_signL == 1) // left foot is in front of the body
	{ 
		_isfront = true;
		//return _isfront;
	}
	else if (_signL == -1) // left foot is in back of the body
		_isfront = false;

	if (_signR == 1) // right foot is in front of the body
	{
		_isfront = true;
		//return _isfront;
	}
	else if (_signR == -1) // right foot is in back of the body
		_isfront = false;

	///_currArcF2D = UTIL_GetVectorsAngle(_root2LFoot, _LRArmsDir);
	//init
	//_root2RHPos.normalize();
	////if (STOPCONTROLLER_TRACE)
	////{	
	////	if (_isfront) 
	////		m_rootRFootVector->setColor(CalVector(1,0,0));
	////	else if (!_isfront)
	////		m_rootRFootVector->setColor(CalVector(0,1,0));
	////}
	return _isfront;
}
