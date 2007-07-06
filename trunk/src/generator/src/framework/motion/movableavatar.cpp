/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "movableavatar.h"
#include "../timeline/lcsmodifier.h"
#include "../utility/debug.h"
//#include "../utility/Cal3dMathsConversion.h"

using namespace ft;
using namespace std;

MovableAvatar::MovableAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
:Avatar(calModel, calCoreModel, modelName)
{
    TRACE = false;

    m_timeLine = NULL;
    setName(modelName);

    setStartPosition(CalVector(0,0,0));

    if (TRACE)
    {
        tracer_start_pos = new TraceLine("start_pos" + toString());
        SceneManager::getInstance()->AddObject(tracer_start_pos);

        tracer_start_pos->AddPoint(getStartPosition());
        tracer_start_pos->AddPoint(CalVector(0,70,0));

    }
 }

MovableAvatar::~MovableAvatar()
{
    if (m_timeLine != NULL)
        m_timeLine->Destroy();

    if (tracer_start_pos != NULL)
    {
        tracer_start_pos->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_start_pos);
    }
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

        Motion* mot = new Motion(animName, i);

        this->AddMotion(mot);
    }
}

void MovableAvatar::StartTimeLine(TimeLine *timeLine)
{
    cout << toString() << "StartTimeLine()" << endl;
    if (timeLine != NULL)
    {
        m_timeLine = timeLine;
        m_timeLine->Start();
    }
}

void MovableAvatar::StopTimeLine()
{
    if (m_timeLine != NULL)
    {
        m_timeLine->Reset();
    }

}

void MovableAvatar::UpdateTimeLine(float elapsedSeconds)
{

    bool timeLineStarted = (m_timeLine != NULL) ? m_timeLine->isStarted() : false;
    
    if (timeLineStarted)
        m_timeLine->Execute(elapsedSeconds, this);

    m_calModel->update(elapsedSeconds);

    if (timeLineStarted)
        m_timeLine->ExecuteModifiers(elapsedSeconds, this);


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

    cout << "- time line:" << std::endl;
    if (m_timeLine != NULL)
        m_timeLine->Dump(2);
    else
       cout << "- - NULL" << endl;
}   


void MovableAvatar::Init()
{
  InitMotions();

  m_bPaused = false;
  m_blendTime = 0.3f;

}

void MovableAvatar::OnUpdate(float elapsedSeconds)
{
     if(!m_bPaused)
    {
        UpdateTimeLine(elapsedSeconds);
    }
}

TimeLine* MovableAvatar::CreateTestTimeLine()
{
    TimeLine* timeLine = new TimeLine();

    Motion *mot = GetMotion(MOTION_WALK_START);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLine->AddObject(timeLineMotion);

    }
    
    mot = GetMotion(MOTION_WALK_LOOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setLoopNumber(3);
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setMotion(mot);
        timeLine->AddObject(timeLineMotion);
    }

    mot = GetMotion(MOTION_WALK_STOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.3f));
        timeLine->AddObject(timeLineMotion);
    }

    mot = GetMotion(MOTION_WALK_IDLE);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
		timeLineMotion->setAnimLoop(true);
        timeLine->AddObject(timeLineMotion);
    }


     //last motion to achive IDLE position
    //TimeLineMotion *idleMotion = new TimeLineMotion();
    //idleMotion->setAnimLoop(true);
    ////idleMotion->setInterupting(true);
    //idleMotion->setMotion(new Motion());
    //timeLine->AddObject(idleMotion);
       

    timeLine->AddModifier(new LCSModifier());
    
    return timeLine;
}

void MovableAvatar::OnMessage(Message* msg)
{
    Avatar::OnMessage(msg);
    if (msg->getType() == MSG_TEST) 
    {

            CalSkeleton *skel = m_calModel->getSkeleton();

            CalBone *bone = skel->getBone(0);

            CalQuaternion currRotation = bone->getRotation();
            Quat newRot(1.5f, Vec(0,0,1)); 
            currRotation *= QuatToCalQuat(newRot);
            bone->setRotation(currRotation);

            bone->calculateState();
    } 
    else if (msg->getType() == MSG_CONTROL_PAUSE)
    {
        m_bPaused = !m_bPaused;
    } 
    else if (msg->getType() == MSG_CONTROL_START)
    {
        std::string myName = getName();
        std::string requestedId = msg->getParam()->getStrValue();
        if (getName().compare(msg->getParam()->getStrValue()) == 0   )
        {
            StartTimeLine(CreateTestTimeLine());
//            cout << "MovableAvatar : blendCycle -> " << MOTION_WALK_LOOP << std::endl;
//            int animId = GetMotion(MOTION_WALK_LOOP)->getAnimID();
//            m_calModel->getMixer()->blendCycle(animId, 1.0f, 0.0f);
        }
    }
    else if (msg->getType() == MSG_CONTROL_STOP)
    {
        if (  getName().compare(msg->getParam()->getStrValue()) == 0   )
        {
            cout << "MovableAvatar : clearCycle -> " << MOTION_WALK_LOOP << std::endl;
            StopTimeLine();
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_LEFT)
    {
        if (  getName().compare(msg->getParam()->getStrValue()) == 0   )
        {
            cout << "MovableAvatar : turn left " << std::endl;
            Quat addRot = Quat(degToRad(-3.0f), Vec(0,1,0));
            m_vRotation *= QuatToCalQuat(addRot);
        }
    }
    else if (msg->getType() == MSG_CONTROL_TURN_RIGHT)
    {
        if (  getName().compare(msg->getParam()->getStrValue()) == 0   )
        {
            cout << "MovableAvatar : turn left " << std::endl;
            Quat addRot = Quat(degToRad(3.0f), Vec(0,1,0));
            m_vRotation *= QuatToCalQuat(addRot);
        }
    }
 }
