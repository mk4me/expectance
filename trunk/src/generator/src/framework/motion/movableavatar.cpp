/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "movableavatar.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;

MovableAvatar::MovableAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
:Avatar(calModel, calCoreModel, modelName)
{
}

MovableAvatar::~MovableAvatar()
{
}

bool MovableAvatar::AddMotion(Motion* motion)
{
    std::string _id = motion->getAnimName();
	if (!_id.empty())
	{
	 	std::map<std::string,Motion*>::iterator it = m_motions.find(_id);

		if ( it!=m_motions.end()) { 
            cout << "ERR: MovableAvatar::AddMotion motion " << _id << " already added to Avatar " << std::endl;
			return false;
		}
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}
    cout << " MovableAvatar::AddMotion motion " << _id << " added to avatar " << std::endl;
	return true;
}
bool  MovableAvatar::RemoveMotion(Motion* motion)
{
    std::string _id = motion->getAnimName();

    if (!_id.empty())
	{
	 	std::map<std::string,Motion*>::iterator it = m_motions.find(_id);
		if ( it!=m_motions.end()) { 
            m_motions.erase(it);
            cout << "MovableAvatar::AddMotion motion " << _id << " removed form avatar " << std::endl;
			return true;
		}
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}
    cout << "ERR: MovableAvatar::AddMotion motion " << _id << " not found in Avatar " << std::endl;
	return false;
}

Motion* MovableAvatar::GetMotion(std::string motionName)
{
    Motion * motion = NULL;
 	std::map<std::string,Motion*>::iterator it = m_motions.find(motionName);
	if ( it!=m_motions.end()) { 
        motion = dynamic_cast<Motion*>(it->second);
    }
    return motion;
}

void MovableAvatar::InitMotions()
{
    int animCount = m_calCoreModel->getCoreAnimationCount();
    for (int i=0; i<animCount; i++)
    {
        CalCoreAnimation* anim = m_calCoreModel->getCoreAnimation(i);
        std::string animName = anim->getFilename();
        this->AddMotion(new Motion(animName, i));
    }
}

void MovableAvatar::Dump()
{
    cout << "Dump MovableAvatar content: " << std::endl;
    cout << "- motions: " << std::endl;

	std::map<std::string,Motion*>::iterator it=m_motions.begin();
	for( ; it != m_motions.end(); ++it ) 
    {
        cout << " - - id " << it->first << std::endl;
    }
}


void MovableAvatar::Init()
{
  InitMotions();

  m_bPaused = false;
  m_blendTime = 0.3f;
  // set initial animation state
/*  if(m_calCoreModel->getCoreAnimationCount() > 0)
  {
    m_currentAnimationId = startAnim;
    m_leftAnimationTime = m_calCoreModel->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;
    if(m_calCoreModel->getCoreAnimationCount() > 1)
    {
      m_calModel->getMixer()->executeAction(m_currentAnimationId, 0.0f, m_blendTime);
    }
    else
    {
      m_calModel->getMixer()->blendCycle(m_currentAnimationId, 1.0f, 0.0f);
    }
  }
  else
  {
    m_currentAnimationId = -1;
    m_leftAnimationTime = -1.0f;
  }*/
}

void MovableAvatar::OnUpdate(float elapsedSeconds)
{
  // update the model if not paused
  /*
  if(!m_bPaused)
  {

    // check if the time has come to blend to the next animation
    if(m_calCoreModel->getCoreAnimationCount() > 1)
    {
      m_leftAnimationTime -= elapsedSeconds;
      if(m_leftAnimationTime <= m_blendTime)
      {
        // get the next animation
        m_currentAnimationId = (m_currentAnimationId + 1) % m_calCoreModel->getCoreAnimationCount();

        // fade in the new animation
        m_calModel->getMixer()->executeAction(m_currentAnimationId, m_leftAnimationTime, m_blendTime);

        // adjust the animation time left until next animation flip
        m_leftAnimationTime = m_calCoreModel->getCoreAnimation(m_currentAnimationId)->getDuration() - m_blendTime;
      }
    }
   
  */
    if(!m_bPaused)
    {
    //        m_calCoreModel->getCoreAnimation(0)->getCoreTrack(0)->
	    m_calModel->update(elapsedSeconds);
    }
}

void MovableAvatar::OnMessage(Message* msg)
{
    Avatar::OnMessage(msg);

    if (msg->getType() == MSG_CONTROL_PAUSE)
    {
        m_bPaused = !m_bPaused;
    } 
    else if (msg->getType() == MSG_CONTROL_START)
    {
        cout << "MovableAvatar : blendCycle -> " << MOTION_WALK_LOOP << std::endl;
        int animId = GetMotion(MOTION_WALK_LOOP)->getAnimID();
        m_calModel->getMixer()->blendCycle(animId, 1.0f, 0.0f);
    }
    else if (msg->getType() == MSG_CONTROL_STOP)
    {
        cout << "MovableAvatar : clearCycle -> " << MOTION_WALK_LOOP << std::endl;
        int animId = GetMotion(MOTION_WALK_LOOP)->getAnimID();
        m_calModel->getMixer()->clearCycle(animId, 0.0f);
    }
}

