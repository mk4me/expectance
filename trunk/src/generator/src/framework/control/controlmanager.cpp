/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "controlmanager.h"
#include "../app/gendebug.h"
#include "scene/cameramanager.h"
#include "scene/oglcontext.h"
#include "core/inputmanager.h"
#include "core/globalmsgsender.h"


using namespace ft;
using namespace std;

ControlManager* ControlManager::m_instance = NULL;

/// \brief Constructor
ControlManager::ControlManager(void)
{
    tracer_active_avatar = NULL;
}

/// \brief Destructor
ControlManager::~ControlManager(void) 
{ 
    if (tracer_active_avatar != NULL)
    {
        tracer_active_avatar->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_active_avatar);
        tracer_active_avatar = NULL;
    }
}

/**
 * \brief Returns the only instance of ft::ControlManager (creates it at first call to this method)
 *
 * \return ft::ControlManager * - the only instance of ControlManager
 **/
ControlManager* ControlManager::getInstance()
{
    if (m_instance == NULL)
    {
        if (GenDebug::CONTROL>0)
            _dbg << "ControlManager::getInstace(): instance of ControlManager created " << endl;

        m_instance = new ControlManager();
    }

    return m_instance;
}

/**
 \brief Releases all resources related to ControlManager
 *
 **/
void ControlManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * \brief Initializes ControlManager
 *
 **/
void ControlManager::Init()
{
    setActiveAvatar(NULL);
    m_activeAvatarInd = -1;

    TRACE = true;

    if (TRACE)
    {
        tracer_active_avatar = new TraceLine(toString() + "_active_avatar");
		tracer_active_avatar->setMarkerShape(ft_Diamond);
        SceneManager::getInstance()->AddObject(tracer_active_avatar);
    }

      InputManager::getInstance()->AddListener(this);

}

/// \brief Update callback called from ft::UpdateManager
/// \param double elapsedSeconds - time interval elapsed since last update
void ControlManager::OnUpdate(const double elapsedSeconds)
{
    UpdateActiveAvatarMarker();
}

/// \brief Updates marker of active avatar
void ControlManager::UpdateActiveAvatarMarker()
{
    if (TRACE && tracer_active_avatar != NULL)
    {
        tracer_active_avatar->ClearTrace();
        AIAvatar* av = getActiveAvatar();
        if (av != NULL)
        {
            CalSkeleton *skel = av->GetCalModel()->getSkeleton();
            CalBone *bone = skel->getBone(0);
            CalVector currPos = bone->getTranslation();
            currPos.y += 120;
            tracer_active_avatar->AddPoint(currPos);
            currPos.y += 10;
            tracer_active_avatar->AddPoint(currPos);
        }
    }
}

/**
 * \brief Adds avatar to list of controlled avatars
 *
 * \param ft::AIAvatar* av - avatar to add
 * \return bool - true if modifier added successfuly
 **/
bool ControlManager::AddAvatar(AIAvatar* av)
{
    if (GenDebug::CONTROL>0)
        _dbg << " AddAvatar " << av->toString() << " to ControlManager " << std::endl;

    m_vAvatars.push_back(av);
	return true;
}

/// \brief Sets active avatar index in ControlManager
/// \param int ind - index of avatar on the list of control avatars
void ControlManager::setActiveAvatar(int ind)
{ 
    AIAvatar* currAvatar = getActiveAvatar();
    if (currAvatar != NULL)
    {
        currAvatar->SetThink(true);
    }

    m_activeAvatarInd  = ind; 

    if (GenDebug::CONTROL>0)
        _dbg << "ControlManager.setActiveAvatar(" << ind << ")" << std::endl;

    currAvatar = getActiveAvatar();
    if (currAvatar != NULL)
    {
        currAvatar->SetThink(false);
    }

}

/// \brief Gets the active avatar set in ControlManager
/// \return AIAvatar* - active avatar or NULL if active avatar is not set (or if current active avatar index in ControlManager 
///                           is out of scope of control avatar list)
AIAvatar* ControlManager::getActiveAvatar()
{
    AIAvatar*  activeAv = NULL;

    int size = (int)m_vAvatars.size();
    if (m_activeAvatarInd >=0 && m_activeAvatarInd < size)
    {
        activeAv = m_vAvatars[m_activeAvatarInd];
    }
    return activeAv;
}


  
//TODO: implement RemoveAvatar
//bool  ControlManager::RemoveAvatar(Avatar* av);


/**
 * \brief Called from InputManager when special keys pressed (F1, F2, UP, DOWN, etc.)
 *
 * \param int key - pressed key
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void ControlManager::OnSpecial(int key, int x, int y)
{
  MovableAvatar* av = NULL;

  int controlMessage = -1;
  switch(key) 
  {
    case GLUT_KEY_UP:
        controlMessage = MSG_CONTROL_START;
        break;
    case GLUT_KEY_DOWN:
        controlMessage = MSG_CONTROL_STOP;
        break;
    case GLUT_KEY_RIGHT:
        controlMessage = MSG_CONTROL_TURN_RIGHT;
        break;
    case GLUT_KEY_LEFT:
        controlMessage = MSG_CONTROL_TURN_LEFT;
        break;
    case GLUT_KEY_F1:
        if (m_activeAvatarInd >=0)
        {
            int newAvtiveInd = (m_activeAvatarInd+1) % (int)m_vAvatars.size();
            setActiveAvatar(newAvtiveInd);
        }
        else
        {
            setActiveAvatar(0);
        }

		CameraManager::getInstance()->setCurrentSceneObjectID(getActiveAvatar()->getID());
        UpdateActiveAvatarMarker();
        break;
    case GLUT_KEY_F2:
        setActiveAvatar(-1);
        UpdateActiveAvatarMarker();
        break;
    case GLUT_KEY_F3:
        av = getActiveAvatar();
        if (av!=NULL)
        {
            float newSpeedFactor = av->getDestSpeedFactor() * 1.1f;
            newSpeedFactor = newSpeedFactor > av->getSpeedFactorMax() ? av->getSpeedFactorMax() :  newSpeedFactor;
            av->setDestSpeedFactor(newSpeedFactor);
            cout << "set new speed " << newSpeedFactor << endl;
        }
        break;
    case GLUT_KEY_F4:
        av = getActiveAvatar();
        if (av!=NULL)
        {
            float newSpeedFactor = av->getDestSpeedFactor() * 0.9f;
            newSpeedFactor = newSpeedFactor < av->getSpeedFactorMin() ? av->getSpeedFactorMin() :  newSpeedFactor;
            av->setDestSpeedFactor(newSpeedFactor);
            cout << "set new speed " << newSpeedFactor << endl;
        }
        break;

    default:
        break;	
  }

  if (controlMessage != -1)
  {
      if (m_activeAvatarInd >= 0 && getActiveAvatar() != NULL)
      {
        GlobalMsgSender::getInstance()->SendMsg(new Message(controlMessage, new MessageParam(getActiveAvatar()->getName())), true);
      }
      else
      {
        GlobalMsgSender::getInstance()->SendMsg(new Message(controlMessage, NULL), true);
      }
  }
}


