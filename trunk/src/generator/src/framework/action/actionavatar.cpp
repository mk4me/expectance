/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "actionavatar.h"

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

    SetAction(ACTION_IDLE);
}

/// \brief Sets another action in Avatar
/// \param Action *newAction - new action
void ActionAvatar::SetAction(Action *newAction)
{
    cout << toString() << ": SetAction( " << newAction->toString() << ")" << endl;

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
        cout << toString() << ":SetAction: not allowed transition from " + m_action->toString() + " to " + newAction->toString() << endl;
    }
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
    cout << toString() << " ActionAvatar::Reset() " << std::endl;
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
            if (getAction() == ACTION_WALK)
                SetAction(ACTION_RUN);
            else if (getAction() == ACTION_IDLE)
                SetAction(ACTION_WALK);
        }
    }
    else if (msg->getType() == MSG_CONTROL_STOP)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
            //cout << "PhysicsAvatar : will stop TimeLine "<< std::endl;
            //StopTimeLine();
           if (getAction() == ACTION_WALK)
                SetAction(ACTION_IDLE);
           else if (getAction() == ACTION_RUN)
                SetAction(ACTION_WALK);
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_LEFT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            cout << "PhysicsAvatar : turn left " << std::endl;
            
            Quat addRot = Quat(degToRad(-3.0f), Vec(0,1,0));
            changeGlobalRotationOffset( QuatToCalQuat(addRot) );
            //m_stepOrientation *= QuatToCalQuat(addRot);
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_RIGHT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            cout << "PhysicsAvatar : turn left " << std::endl;

            Quat addRot = Quat(degToRad(3.0f), Vec(0,1,0));
			changeGlobalRotationOffset( QuatToCalQuat(addRot) );
            //m_stepOrientation *= QuatToCalQuat(addRot);
        }
    }
    else if (msg->getType() == MSG_TEST)
    {
        if (this->getTLExecutor() != NULL)
        {
            if (this->getTLExecutor()->isTerminated()) 
            {
                cout << " setTerminated ---------------------- false" << endl;
                this->getTLExecutor()->setTerminated(false);
            }
            else
            {
                cout << " setTerminated ---------------------- true" << endl;
                this->getTLExecutor()->setTerminated(true);
            }
        }
    }

 }
