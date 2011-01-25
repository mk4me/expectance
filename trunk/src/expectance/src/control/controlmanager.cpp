/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "controlmanager.h"

using namespace ft;
using namespace std;

ControlManager* ControlManager::m_instance = NULL;

/// \brief Constructor
ControlManager::ControlManager(void)
{
////    tracer_active_avatar = NULL; marker aktywnego avatara
}

/// \brief Destructor
ControlManager::~ControlManager(void) 
{ 
    ////if (tracer_active_avatar != NULL) usuwanie markera
    ////{
    ////    tracer_active_avatar->ClearTrace();
    ////    SceneManager::getInstance()->RemoveObject(tracer_active_avatar);
    ////    tracer_active_avatar = NULL;
    ////}
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
        // todo if (GenDebug::CONTROL>0)
		std::cout << "ControlManager::getInstace(): instance of ControlManager created " << std::endl;

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
    setActiveAvatar(-1);
    m_activeAvatarInd = -1;

    TRACE = true;

  ////  if (TRACE)
  ////  {
  ////      tracer_active_avatar = new TraceLine(toString() + "_active_avatar");
		////tracer_active_avatar->setMarkerShape(ft_Diamond);
  ////      SceneManager::getInstance()->AddObject(tracer_active_avatar);
  ////  }

  ////    InputManager::getInstance()->AddListener(this);

}


/// \brief Update callback called from ft::UpdateManager
/// \param double elapsedSeconds - time interval elapsed since last update
////void ControlManager::OnUpdate(const double elapsedSeconds)
////{
////    UpdateActiveAvatarMarker();
////}

/// \brief Updates marker of active avatar
////void ControlManager::UpdateActiveAvatarMarker()
////{
////    if (TRACE && tracer_active_avatar != NULL)
////    {
////        tracer_active_avatar->ClearTrace();
////        OsgAvatar* av = getActiveAvatarImpl();
////        if (av != NULL)
////        {
////            CalSkeleton *skel = av->GetCalModel()->getSkeleton();
////            CalBone *bone = skel->getBone(0);
////            CalVector currPos = bone->getTranslation();
////            currPos.y += 120;
////            tracer_active_avatar->AddPoint(currPos);
////            currPos.y += 10;
////            tracer_active_avatar->AddPoint(currPos);
////        }
////    }
////}

/**
 * \brief Adds avatar to list of controlled avatars
 *
 * \param ft::AIAvatar* av - avatar to add
 * \return bool - true if modifier added successfuly
 **/
bool ControlManager::AddAvatar(Avatar* av)
{
    //TODO if (GenDebug::CONTROL>0)
    //TODO    std::cout << " AddAvatar " << ((OsgAvatar*)av->getImplementation())->toString() << " to ControlManager " << std::endl;
	std::cout << "AddAvatar" << std::endl;
	m_vAvatars.push_back(av);
	return true;
}

////Avatar * ControlManager::getAvatar(const std::string& name)
////{
////	Avatar av* = null;
////	for (int i=0; i< m_vAvatars.size(); i++)
////	{
////		av = m_vAvatars[i];
////		if (av.getImplementation()->getn
////	}
////}

/// \brief Sets active avatar index in ControlManager
/// \param int ind - index of avatar on the list of control avatars
void ControlManager::setActiveAvatar(int ind)
{ 
    Avatar* currAvatar = getActiveAvatar();
    if (currAvatar != NULL)
    {
        currAvatar->SetThink(true);
    }

    m_activeAvatarInd  = ind; 

    // TODO if (GenDebug::CONTROL>0)
        std::cout << "ControlManager.setActiveAvatar(" << ind << ")" << std::endl;

    currAvatar = getActiveAvatar();
    if (currAvatar != NULL)
    {
        currAvatar->SetThink(false);
    }

}

/// \brief Gets the active avatar set in ControlManager
/// \return AIAvatar* - active avatar or NULL if active avatar is not set (or if current active avatar index in ControlManager 
///  is out of scope of control avatar list)
Avatar* ControlManager::getActiveAvatar()
{
    Avatar*  activeAv = NULL;

    int size = (int)m_vAvatars.size();
    if (m_activeAvatarInd >=0 && m_activeAvatarInd < size)
    {
        activeAv = m_vAvatars[m_activeAvatarInd];
    }
	return activeAv;
}

OsgAvatar* ControlManager::getActiveAvatarImpl()
{
	Avatar* activeAv = getActiveAvatar();
	OsgAvatar* result = NULL;
	if (activeAv != NULL)
		result = static_cast<OsgAvatar*>(activeAv->getImplementation());
    return result;

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
//void ControlManager::OnSpecial(int key, int x, int y)
//{
//  OsgAvatar* av = NULL;
//
//  int controlMessage = -1;
//  int msgEvent = -1;
//  switch(key) 
//  {
//    case GLUT_KEY_UP:
//		msgEvent = 0;
//        break;
//    case GLUT_KEY_DOWN:
//        msgEvent = 1;
//        break;
//    case GLUT_KEY_RIGHT:
//        controlMessage = MSG_CONTROL_TURN_RIGHT;
//        break;
//    case GLUT_KEY_LEFT:
//        controlMessage = MSG_CONTROL_TURN_LEFT;
//        break;
//    case GLUT_KEY_F1:
//        if (m_activeAvatarInd >=0)
//        {
//            int newAvtiveInd = (m_activeAvatarInd+1) % (int)m_vAvatars.size();
//            setActiveAvatar(newAvtiveInd);
//        }
//        else
//        {
//            setActiveAvatar(0);
//        }
//
//		CameraManager::getInstance()->setCurrentSceneObjectID(getActiveAvatarImpl()->getID());
//        UpdateActiveAvatarMarker();
//        break;
//    case GLUT_KEY_F2:
//        setActiveAvatar(-1);
//        UpdateActiveAvatarMarker();
//        break;
//    case GLUT_KEY_F3:
//        av = getActiveAvatarImpl();
//        if (av!=NULL)
//        {
//            float newSpeedFactor = av->getDestSpeedFactor() * 1.1f;
//            newSpeedFactor = newSpeedFactor > av->getSpeedFactorMax() ? av->getSpeedFactorMax() :  newSpeedFactor;
//            av->setDestSpeedFactor(newSpeedFactor);
//        }
//        break;
//    case GLUT_KEY_F4:
//        av = getActiveAvatarImpl();
//        if (av!=NULL)
//        {
//            float newSpeedFactor = av->getDestSpeedFactor() * 0.9f;
//            newSpeedFactor = newSpeedFactor < av->getSpeedFactorMin() ? av->getSpeedFactorMin() :  newSpeedFactor;
//            av->setDestSpeedFactor(newSpeedFactor);
//        }
//        break;
//
//    default:
//        break;	
//  }
//
//  if (msgEvent != -1)
//  {
//	  if (m_activeAvatarInd >= 0 && getActiveAvatar() != NULL)
//	  {
//		SendControlEvent(getActiveAvatar(), msgEvent);
//	  }
//	  else
//	  {
//			if (m_vAvatars.size() > 0)
//			{
//				for (int m=0; m<(int)m_vAvatars.size(); m++)
//				{
//					SendControlEvent(m_vAvatars[m], msgEvent);
//				}
//			}
//	  }
//  }
//
//
//  if (controlMessage != -1)
//  {
//      if (m_activeAvatarInd >= 0 && getActiveAvatar() != NULL)
//      {
//        GlobalMsgSender::getInstance()->SendMsg(new Message(controlMessage, new MessageParam(getActiveAvatarImpl()->getName())), true);
//      }
//      else
//      {
//        GlobalMsgSender::getInstance()->SendMsg(new Message(controlMessage, NULL), true);
//      }
//  }
//}
//
//void ControlManager::SendControlEvent(Avatar* av, int event_id)
//{
//
//    if (event_id == 0)
//    {
//		if (av->getCurrTopMotion() != NULL)
//		{
//			
//			if (av->getCurrTopMotion()->getName().compare("walk") == 0)
//				av->ExecuteAction("run");
//			else if (av->getCurrTopMotion()->getName().compare("idle") == 0)
//				av->ExecuteAction("walk");
//		}
//		else
//		{
//			av->ExecuteAction("idle");
//		}
//    }
//    else if (event_id == 1)
//    {
//		if (av->getCurrTopMotion() != NULL)
//		{
//			if (av->getCurrTopMotion()->getName().compare("walk") == 0)
//				///av->ExecuteAction("idle");
//			{
//				//mka 2008.08.30 send signal to get stop controller possibility to go on
//				OsgAvatar* avImpl = static_cast<OsgAvatar*>(av->getImplementation());
//				avImpl->getStopController()->setStop(true);
//			}
//			else if (av->getCurrTopMotion()->getName().compare("run") == 0)
//				av->ExecuteAction("walk");
//		}
//		else
//		{
//			av->ExecuteAction("idle");
//		}
//
//    }
//}
