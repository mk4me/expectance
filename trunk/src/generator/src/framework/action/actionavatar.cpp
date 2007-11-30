/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "actionavatar.h"
#include "../app/gendebug.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
ActionAvatar::ActionAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
:PhysicsAvatar(calModel, calCoreModel, modelName)
{
    m_action = NULL;
    ACTION_IDLE = NULL;
    ACTION_WALK = NULL;
    ACTION_RUN = NULL;
	m_turnLeft = m_turnRight = true;
	FOOT_DETECTOR = ((Config::getInstance()->IsKey("foot_detector")) 
							&& (Config::getInstance()->GetIntVal("foot_detector")==1)) ? true: false;
}

// \brief Destructor
ActionAvatar::~ActionAvatar()
{
    //empty
}

/// \brief Releases all resources and objects related to this Avatar
void ActionAvatar::Destroy(void)
{
    PhysicsAvatar::Destroy();
    DestroyActions();
}

/// \brief Initiates avatar layer for Avatar
void ActionAvatar::Init()
{
    PhysicsAvatar::Init();
    CreateActions(); //create timelines for actions

    SetActionById(ACTION_IDLE_ID);
}

/// \brief Sets another action in Avatar
/// \param Action *newAction - new action
void ActionAvatar::SetCurrAction(Action *newAction)
{
    if (GenDebug::ACTION)
        _dbg << toString() << ": SetCurrAction( " << newAction->toString() << ")" << endl;

    if (IsTransitionAllowed(m_action, newAction))
    {
        if (m_action != NULL)
            m_action->Exit(this, newAction);
        
        Action *oldAction = m_action;

        m_action = newAction;

        newAction->Entry(this, oldAction);
    }
    else
    {
        if (GenDebug::ACTION)
            _dbg << toString() << ":SetAction: not allowed transition from " + m_action->toString() + " to " + newAction->toString() << endl;
    }
}

/// \brief Sets another action in Avatar using id of action
/// \param int id - id of target action
/// \return Action* - pointer to set action or NULL if the re is no action for given id
Action* ActionAvatar::SetActionById(int id)
{
    Action* action = getActionById(id);

    if (action != NULL)
    {
        SetCurrAction(action);
    }

    return action;
}

/// \brief Returns action for given action id 
/// \param int id - id of action 
/// \return Action* - pointer to set action or NULL if the re is no action for given id
Action* ActionAvatar::getActionById(int id)
{
    Action* result = NULL;
    switch(id)
    {
    case ACTION_IDLE_ID:
        result = ACTION_IDLE;
        break;
    case ACTION_WALK_ID:
        result = ACTION_WALK;
        break;
    case ACTION_RUN_ID:
        result = ACTION_RUN;
        break;
    default:
        break;
    }

    return result;
}

/// \brief Creates set of possible actions
void ActionAvatar::CreateActions()
{
    if (ACTION_IDLE == NULL)
    {
        ACTION_IDLE = new IdleAction();
        ACTION_IDLE->Init(this);
    }
    if (ACTION_WALK == NULL)
    {
        ACTION_WALK = new WalkAction();
        ACTION_WALK->Init(this);
    }
    if (ACTION_RUN == NULL)
    {
        ACTION_RUN = new RunAction();
        ACTION_RUN->Init(this);
    }
}

/// \brief Releases all resources related to actions for this Avatar
void ActionAvatar::DestroyActions()
{
    if (ACTION_IDLE != NULL)
    {
        delete ACTION_IDLE;
        ACTION_IDLE = NULL;
    }
    if (ACTION_WALK != NULL)
    {
        delete ACTION_WALK;
        ACTION_WALK = NULL;
    }
    if (ACTION_RUN != NULL)
    {
        delete ACTION_RUN;
        ACTION_RUN = NULL;
    }
    
}

/// \brief Checks if transition from given action to another specific action is allowed
/// \param ControlAction *oldAction - action from which the transition will be done
/// \param ControlAction *newAction - target action of transition 
/// \return bool - true if transition allowed, false otherwise
bool ActionAvatar::IsTransitionAllowed(Action* oldAction, Action* newAction)
{
    bool result = true;

    if (oldAction == newAction)
    {
        result = false;
    }

    return result;
}

/// \brief Resets all parameters related to action layer for this avatar
void ActionAvatar::Reset()
{
    PhysicsAvatar::Reset();

    if (GenDebug::ACTION)
        _dbg << toString() << " ActionAvatar::Reset() " << std::endl;

    ACTION_IDLE->Reset(this);
    ACTION_WALK->Reset(this);
    ACTION_RUN->Reset(this);
}

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void ActionAvatar::OnMessage(Message* msg)
{
    PhysicsAvatar::OnMessage(msg);

    if (msg->getType() == MSG_CONTROL_START)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
            if (getCurrAction() == ACTION_WALK)
                SetActionById(ACTION_RUN_ID);
            else if (getCurrAction() == ACTION_IDLE)
                SetActionById(ACTION_WALK_ID);
        }
    }
    else if (msg->getType() == MSG_CONTROL_STOP)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
            //_dbg << "PhysicsAvatar : will stop TimeLine "<< std::endl;
            //StopTimeLine();
           if (getCurrAction() == ACTION_WALK)
                SetActionById(ACTION_IDLE_ID);
           else if (getCurrAction() == ACTION_RUN)
                SetActionById(ACTION_WALK_ID);
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
	else if (msg->getType() == MSG_CONTROL_TURN_LEFT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            _dbg << "PhysicsAvatar : turn left " << std::endl;

            if (m_turnLeft)
			{
				Quat addRot = Quat(degToRad(-3.0f), Vec(0,1,0));
				changeGlobalRotationOffset( QuatToCalQuat(addRot) );
				//m_stepOrientation *= QuatToCalQuat(addRot);
			}
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_RIGHT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            _dbg << "PhysicsAvatar : turn left " << std::endl;
            if (m_turnRight)
			{
				Quat addRot = Quat(degToRad(3.0f), Vec(0,1,0));
				changeGlobalRotationOffset( QuatToCalQuat(addRot) );
				//m_stepOrientation *= QuatToCalQuat(addRot);
			}
        }
    }
    else if (msg->getType() == MSG_TEST)
    {
        if (this->getTLExecutor() != NULL)
        {
            if (this->getTLExecutor()->isTerminated()) 
            {
                if (GenDebug::ACTION>0)
                    _dbg << " setTerminated ---------------------- false" << endl;

                this->getTLExecutor()->setTerminated(false);
            }
            else
            {
                if (GenDebug::ACTION>0)
                    _dbg << " setTerminated ---------------------- true" << endl;

                this->getTLExecutor()->setTerminated(true);
            }
        }
    }

 }

void ActionAvatar::Dump()
{
    MovableAvatar::Dump();

    if (GenDebug::ACTION>0)
    {
        _dbg << "Current action: " << getCurrAction()->toString() << endl;
    }

}
