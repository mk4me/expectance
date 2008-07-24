/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "calavatar.h"
#include "../app/gendebug.h"
#include "calavatartype.h"
#include "../motion/animation.h"

using namespace ft;
using namespace ft::gil;

/**
 * \brief Constructor for avatar
 *
 * \param CalModel * calModel - model in Cal3d which is referenced by this avatar
 * \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
 * \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
 **/
CalAvatar::CalAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName):
Cal3DObject(calModel, calCoreModel, modelName)
{
    m_localMsgSender = new MsgSender();
	m_localMsgSender->AddMsgListener(this);
    InitSpeedFactor();
	setFootDetector(new FootDetector());
	InitFootDetector();

	m_turnLeft = m_turnRight = true;
	FOOT_DETECTOR = ((Config::getInstance()->IsKey("foot_detector")) 
							&& (Config::getInstance()->GetIntVal("foot_detector")==1)) ? true: false;
}

/**
 * \brief Destructor
 *
 **/
CalAvatar::~CalAvatar()
{
    m_localMsgSender->Destroy();
    delete m_localMsgSender;
}

/// \brief Releases all resources and objects related to this CalAvatar
void CalAvatar::Destroy(void)
{
    Cal3DObject::Destroy();
}

const std::string CalAvatar::getType()
{
	return getCalCoreModel()->getName();
}

GIL_Animation* CalAvatar::getAnimation(const std::string& animName)
{
    Animation * motion = NULL;
    motion = ((CalAvatarType*)this->GetCalCoreModel())->GetMotion(animName);
    return motion;
}

/**
 * \brief Init speed factor and all scopes for factor changing
 **/
void CalAvatar::InitSpeedFactor()
{
    setCurrSpeedFactor(1);
    setDestSpeedFactor(1);
    
    float minSpeed = 0.5f;
    if (Config::getInstance()->IsKey("default_min_speed_factor"))
    {
        minSpeed = Config::getInstance()->GetFloatVal("default_min_speed_factor");
    }
    setSpeedFactorMin(minSpeed);

    float maxSpeed = 1.5f;
    if (Config::getInstance()->IsKey("default_max_speed_factor"))
    {
        maxSpeed = Config::getInstance()->GetFloatVal("default_max_speed_factor");
    }
    setSpeedFactorMax(maxSpeed);
}

/**
 * \brief Collects all motions (animations) for ths avatar from CalCoreModel
 **/
void CalAvatar::InitFootDetector()
{
    FootDetector* _fd = getFootDetector();

    std::map<std::string,Animation*> motions = ((CalAvatarType*)GetCalCoreModel())->getMotionsMap();
  	std::map<std::string,Animation*>::iterator it = motions.begin();

    for( ; it != motions.end(); ++it ) 
    {
        _fd->AddLimits(it->second->getAnimName(), it->second->footLimits);
    }
}



void CalAvatar::Update(const double elapsedSeconds)
{
	GetCalModel()->update(elapsedSeconds);
}

GIL_AnimExecution* CalAvatar::PlayAnimation(int animID, float fade_in, float fade_out)
{
    GetCalModel()->getMixer()->executeAction(animID, fade_in, fade_out);

	GIL_AnimExecution* result = NULL;
	CalAnimation* anim = FindAddedAnimInCal3d(CalAnimation::TYPE_ACTION);
	if (anim != NULL)
	{
		result = new Cal3dAnimExecution(anim);
	}
	return result;
}

void CalAvatar::StopAnimation(int animID, float fade_out)
{
	GetCalModel()->getMixer()->removeAction(animID);
}

GIL_AnimExecution* CalAvatar::PlayCyclicAnimation(int animID, float fade_in)
{
    GetCalModel()->getMixer()->blendCycle(animID,  1.0f, fade_in);

	GIL_AnimExecution* result = NULL;
	CalAnimation* anim = FindAddedAnimInCal3d(CalAnimation::TYPE_CYCLE);
	if (anim != NULL)
	{
		result = new Cal3dAnimExecution(anim);
	}
	return result;
}

void CalAvatar::StopCyclicAnimation(int animID, float fade_out)
{
	GetCalModel()->getMixer()->clearCycle(animID, fade_out);
}

bool CalAvatar::isAnimValid(GIL_AnimExecution* animExec)
{
    bool result = false;

	CalAnimation* anim = ((Cal3dAnimExecution*)animExec)->getAnimation();

    CalMixer* mixer = GetCalModel()->getMixer();

    if (anim->getType() == CalAnimation::TYPE_ACTION)
    {
        if (mixer->getAnimationActionList().size() > 0)
        {
            std::list<CalAnimationAction *>::iterator iterator = mixer->getAnimationActionList().begin();
            while(iterator != mixer->getAnimationActionList().end())
            {
                if((*iterator) == anim )
                    result = true;
                iterator++;
            }
        }
    }
    else if (anim->getType() == CalAnimation::TYPE_CYCLE)
    {
        if (mixer->getAnimationCycle().size() > 0)
        {
            std::list<CalAnimationCycle *>::iterator iterator = mixer->getAnimationCycle().begin();
            while(iterator != mixer->getAnimationCycle().end())
            {
                if((*iterator) == anim )
                    result = true;
                iterator++;
            }
        }
    }

	if (!result)
	{
		delete animExec;  // TODO: consider all cases 
	}

    return result;
}

float CalAvatar::getAnimTime(GIL_AnimExecution* anim_exec)
{
   float animTime = 0;
   Cal3dAnimExecution* cal3d_anim_exec = (Cal3dAnimExecution*)anim_exec;
   if (cal3d_anim_exec->getAnimation()->getType() == CalAnimation::TYPE_CYCLE 
            && cal3d_anim_exec->getAnimation()->getState() == CalAnimation::STATE_SYNC)
    {
        animTime = GetCalModel()->getMixer()->getAnimationTime();
    }
    else  //actions and cycles in ASYNC state
    {
        animTime = cal3d_anim_exec->getAnimation()->getTime();
    }
   return animTime;
}


float CalAvatar::getAnimDuration(GIL_AnimExecution* anim_exec)
{
   float animDuration = 0;
   Cal3dAnimExecution* cal3d_anim_exec = (Cal3dAnimExecution*)anim_exec;
   if (cal3d_anim_exec->getAnimation()->getType() == CalAnimation::TYPE_CYCLE 
            && cal3d_anim_exec->getAnimation()->getState() == CalAnimation::STATE_SYNC)
    {
        animDuration = GetCalModel()->getMixer()->getAnimationDuration();
    }
    else  //actions and cycles in ASYNC state
    {
        animDuration = cal3d_anim_exec->getAnimation()->getCoreAnimation()->getDuration();
    }
   return animDuration;
}

bool CalAvatar::isEmptyFrame()
{
    CalMixer* mixer = GetCalModel()->getMixer();
    return (mixer->getAnimationActionList().size() == 0 && mixer->getAnimationCycle().size() == 0) ;
}

/**
 * \brief Extracts last animation object added to Cal3d
 * \param int animType - action or cycle
 * \return CalAnimation* - animation object extracted from Cal3d or NULL if extraction failed
 **/
CalAnimation* CalAvatar::FindAddedAnimInCal3d(int animType)
{
    CalAnimation* anim = NULL;
    CalMixer* mixer = GetCalModel()->getMixer();
    
    if (animType == CalAnimation::TYPE_ACTION)
    {
        if (mixer->getAnimationActionList().size() > 0)
        {
            anim = mixer->getAnimationActionList().front();
        }
    }
    else if (animType == CalAnimation::TYPE_CYCLE)
    {
        if (mixer->getAnimationCycle().size() > 0)
        {
            anim = mixer->getAnimationCycle().front();
            
        }
    }

    return anim;
}

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void CalAvatar::OnMessage(Message* msg)
{
	//TODO: uncomment
    //PhysicsAvatar::OnMessage(msg);

    if (msg->getType() == MSG_CONTROL_TURN_LEFT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            _dbg << "PhysicsAvatar : turn left " << std::endl;

//            if (m_turnLeft) //TODO: uncomment it
			{
				Quat addRot = Quat(degToRad(-3.0f), Vec(0,1,0));
				
				//TODO: uncomment it
				changeGlobalRotationOffset( QuatToCalQuat(addRot) );

			}
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_RIGHT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            _dbg << "PhysicsAvatar : turn left " << std::endl;
  //          if (m_turnRight)//TODO: uncomment it
			{
				Quat addRot = Quat(degToRad(3.0f), Vec(0,1,0));

				changeGlobalRotationOffset( QuatToCalQuat(addRot) );
				
			}
        }
    }
	else if (msg->getType() == MSG_DETECTOR_LEFT_FOOT_ON_THE_FLOOR)
    {
        if ((msg->getParam() != NULL) && (FOOT_DETECTOR) )
		{
			m_turnRight = msg->getParam()->getBoolValue();
		}
	}
	else if (msg->getType() == MSG_DETECTOR_RIGHT_FOOT_ON_THE_FLOOR)
    {
        if ((msg->getParam() != NULL) && (FOOT_DETECTOR))
		{
			m_turnLeft = msg->getParam()->getBoolValue();
		}
	}
	else if (msg->getType() == MSG_TEST)
    {
/*        if (this->getTLExecutor() != NULL)
        {
            if (this->getTLExecutor()->isTerminated()) 
            {
                if (EvoDBG::ACTION>0)
                    _dbg << " setTerminated ---------------------- false" << endl;

                this->getTLExecutor()->setTerminated(false);
            }
            else
            {
                if (EvoDBG::ACTION>0)
                    _dbg << " setTerminated ---------------------- true" << endl;

                this->getTLExecutor()->setTerminated(true);
            }
        }
		*/
    }
 }


/**
 * \brief Prints debug information describing this avatar on output console
 *
 **/
void CalAvatar::Dump()
{
    ((CalAvatarType*)GetCalCoreModel())->Dump();
}

