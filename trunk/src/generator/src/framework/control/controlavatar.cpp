/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "controlavatar.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
ControlAvatar::ControlAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
:MovableAvatar(calModel, calCoreModel, modelName)
{
    m_state = NULL;
    IDLE_STATE = NULL;
    WALK_STATE = NULL;
    RUN_STATE = NULL;
}

// \brief Destructor
ControlAvatar::~ControlAvatar()
{
    //empty
}

/// \brief Releases all resources and objects related to this Avatar
void ControlAvatar::Destroy(void)
{
    MovableAvatar::Destroy();
    DestroyStates();
}

/// \brief Initiates control layer for Avatar
void ControlAvatar::Init()
{
    MovableAvatar::Init();
    CreateStates(); //create timelines for states

    SetState(IDLE_STATE);
}

/// \brief Sets another control state in Avatar
/// \param ControlState *newState - new state
void ControlAvatar::SetState(ControlState *newState)
{
    cout << toString() << ": SetState( " << newState->toString() << ")" << endl;

    if (IsTransitionAllowed(m_state, newState))
    {
        if (m_state != NULL)
            m_state->Exit(this, newState);
        
        ControlState *oldState = m_state;

        m_state = newState;

        newState->Entry(this, oldState);
    }
    else
    {
        cout << toString() << ":SetState: not allowed transition from " + m_state->toString() + " to " + newState->toString() << endl;
    }
}

/// \brief Creates set of possible control states
void ControlAvatar::CreateStates()
{
    if (IDLE_STATE == NULL)
    {
        IDLE_STATE = new IdleState();
        IDLE_STATE->Init(this);
    }
    if (WALK_STATE == NULL)
    {
        WALK_STATE = new WalkState();
        WALK_STATE->Init(this);
    }
    if (RUN_STATE == NULL)
    {
        RUN_STATE = new RunState();
        RUN_STATE->Init(this);
    }
}

/// \brief Releases all resources related to control states for this Avatar
void ControlAvatar::DestroyStates()
{
    if (IDLE_STATE != NULL)
    {
        delete IDLE_STATE;
        IDLE_STATE = NULL;
    }
    if (WALK_STATE != NULL)
    {
        delete WALK_STATE;
        WALK_STATE = NULL;
    }
    if (RUN_STATE != NULL)
    {
        delete RUN_STATE;
        RUN_STATE = NULL;
    }
    
}

/// \brief Checks if transition from given state to another specific state is allowed
/// \param ControlState *oldState - state from which the transition will be done
/// \param ControlState *newState - target state of transition 
/// \return bool - true if transition allowed, false otherwise
bool ControlAvatar::IsTransitionAllowed(ControlState* oldState, ControlState* newState)
{
    bool result = true;

    if (oldState == newState)
    {
        result = false;
    }

    return result;
}

/// \brief Resets all parameters related to control layer for this avatar
void ControlAvatar::Reset()
{
    MovableAvatar::Reset();
    cout << toString() << " ControlAvatar::Reset() " << std::endl;
    IDLE_STATE->Reset(this);
    WALK_STATE->Reset(this);
    RUN_STATE->Reset(this);
}

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void ControlAvatar::OnMessage(Message* msg)
{
    MovableAvatar::OnMessage(msg);

    if (msg->getType() == MSG_CONTROL_START)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
            if (getState() == WALK_STATE)
                SetState(RUN_STATE);
            else if (getState() == IDLE_STATE)
                SetState(WALK_STATE);
        }
    }
    else if (msg->getType() == MSG_CONTROL_STOP)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
            //cout << "MovableAvatar : will stop TimeLine "<< std::endl;
            //StopTimeLine();
           if (getState() == WALK_STATE)
                SetState(IDLE_STATE);
           else if (getState() == RUN_STATE)
                SetState(WALK_STATE);
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_LEFT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            cout << "MovableAvatar : turn left " << std::endl;
            
            Quat addRot = Quat(degToRad(-3.0f), Vec(0,1,0));
            changeGlobalRotationOffset( QuatToCalQuat(addRot) );
            //m_stepOrientation *= QuatToCalQuat(addRot);
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_RIGHT)
    {
        if (msg->getParam() == NULL || (getName().compare(msg->getParam()->getStrValue()) == 0))
        {
//            cout << "MovableAvatar : turn left " << std::endl;

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

