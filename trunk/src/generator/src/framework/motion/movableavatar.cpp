/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "movableavatar.h"
#include "../timeline/lcsmodifier.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
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
        VisualizationManager::getInstance()->AddObject(tracer_start_pos);

        tracer_start_pos->AddPoint(getStartPosition());
        tracer_start_pos->AddPoint(CalVector(0,70,0));

    }
 }

// \brief Destructor
MovableAvatar::~MovableAvatar()
{
    if (m_timeLine != NULL)
        m_timeLine->Destroy();

    if (tracer_start_pos != NULL)
    {
        tracer_start_pos->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_start_pos);
    }
}

/// \brief Releases all resources and objects related to this Avatar
void MovableAvatar::Destroy(void)
{
    Avatar::Destroy();

    if (m_timeLine != NULL)
    {
        m_timeLine->Destroy();
        delete m_timeLine;
    }

    if (m_timeLineContext != NULL)
    {
        m_timeLineContext->Destroy();
        delete m_timeLineContext;
    }
}

/**
 * \brief Adds motion (animation) to avatar
 *
 * \param ft::Motion * motion - motion that represents animation 
 * \return bool - true if operation succeed, false if motion with the same name is already added to avatar
 **/
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
/**
 * \brief Removes motion (animation) from avatar
 *
 * \param ft::Motion * motion - moton to remove
 * \return bool - true if motion removed, false if given motion not found for this avatar
 **/
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

/**
 * \brief Gets motion with given name
 *
 * \param std::string motionName - name of motion (animation)
 * \return ft::Motion * - requested motion or NULL if the is no motion with this name for this avatar
 **/
Motion* MovableAvatar::GetMotion(std::string motionName)
{
    Motion * motion = NULL;
 	std::map<std::string,Motion*>::iterator it = m_motions.find(motionName);
	if ( it!=m_motions.end()) { 
        motion = dynamic_cast<Motion*>(it->second);
    }
    return motion;
}

/**
 * \brief Collects all motions (animations) for ths avatar from CalCoreModel
 **/
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

/**
 * \brief Sets main TimeLine for this avatar
 * * \param TimeLine* timeLine - TimeLine to set
 **/
bool MovableAvatar::setTimeLine(TimeLine* timeLine)
{
    bool result = false;
    if (m_timeLine == NULL)
    {
        m_timeLine = timeLine;
        result = true;
    }
    else
    {
        cout << "ERR: MovableAvatar::setTimeLine - timeline is already set for avatar " + toString();
    }

    return result;
}

/**
 * \brief Returns TimeLine for this avatar
 * \return ft::TimeLine * - main TimeLine for this avatar
 **/
TimeLine* MovableAvatar::getTimeLine()
{
    return m_timeLine;
}

/**
 * \brief Adds new TimeLineMotion to main TimeLine of this avatar
 * \param TimeLineMotion *timeLineMotion - TimeLineMotion to add
 * \return bool - true if added succesfully, false otherwise.
 **/

bool MovableAvatar::AddTimeLineMotion(TimeLineMotion *timeLineMotion)
{
    return false;
}


/**
 * \brief Starts performing TimeLine assigned currently to this avatar 
 *
 * \param ft::TimeLine * timeLine - TimeLine to start
 **/
void MovableAvatar::StartTimeLine()
{
    cout << toString() << "StartTimeLine()" << endl;
    if (m_timeLine != NULL)
    {
        m_timeLine->Start(m_timeLineContext);
    }
}

/**
 * \brief Stops currently assigned timeline (if any)
 **/
void MovableAvatar::StopTimeLine()
{
    if (m_timeLine != NULL)
    {
        //m_timeLine->Reset(m_timeLineContext);
//        m_timeLine->StopTimeLine(m_timeLineContext);
        m_timeLine->Reset(m_timeLineContext);
    }

}

/**
 * \brief Updates currently assigned TimeLine object (if any). 
 * It is called for each frame updated from UpdateManager.
 *
 * \param float elapsedSeconds - time elapsed sice last update
 **/
void MovableAvatar::UpdateTimeLine(float elapsedSeconds)
{

    bool timeLineStarted = (m_timeLine != NULL) ? m_timeLine->isStarted() : false;
    
    if (timeLineStarted)
        m_timeLine->Execute(elapsedSeconds, m_timeLineContext);

    m_calModel->update(elapsedSeconds);

    if (timeLineStarted)
        m_timeLine->ExecuteModifiers(elapsedSeconds, m_timeLineContext);
}


/**
 * \brief Prints debug information describing this avatar on output console
 *
 **/
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



/// \brief Initializes this avatar with regard to motions
void MovableAvatar::Init()
{
  InitMotions();
  setTimeLine(new TimeLine());
  TimeLineContext* ctx = new TimeLineContext();
  //ctx->stop_immediate = true;

  //ctx->remove_after_execution = true;

  ctx->setAvatar(this);
  setTimeLineContext(ctx);
  CreateTestTimeLine();

  m_bPaused = false;
  m_blendTime = 0.3f;
}

/**
 * \brief This method is called when new frame is updated from ft::UpdateManager
 *
 * \param float elapsedSeconds - time elapsed sice last update
 **/
void MovableAvatar::OnUpdate(float elapsedSeconds)
{
     if(!m_bPaused)
    {
        UpdateTimeLine(elapsedSeconds);
    }
}

/**
 * \brief Creates TimeLine object for testing purpose
 *
 * \return ft::TimeLine * - TimeLine for testing framework
 **/
TimeLine* MovableAvatar::CreateTestTimeLine()
{
    Motion *mot = GetMotion(MOTION_WALK_START);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.1f));
        getTimeLine()->AddSubObject(timeLineMotion, ADD_OBJECT_AS_LAST);

    }
 
 
    mot = GetMotion(MOTION_WALK_LOOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setLoopNumber(2);
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setMotion(mot);
//        timeLineMotion->setBlender(new TimeLineBlender(1.0f));
        getTimeLine()->AddSubObject(timeLineMotion);
    }

    mot = GetMotion(MOTION_WALK_STOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(1.0f));
        getTimeLine()->AddSubObject(timeLineMotion);
    }

    mot = GetMotion(MOTION_WALK_IDLE);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
		timeLineMotion->setAnimLoop(true);
        getTimeLine()->AddSubObject(timeLineMotion);
    }


     //last motion to achive IDLE position
  //  TimeLineMotion *idleMotion = new TimeLineMotion();
//    idleMotion->setAnimLoop(true);
    //idleMotion->setInterupting(true);
//    idleMotion->setMotion(new Motion());
//    timeLine->AddObject(idleMotion);
       

    getTimeLine()->AddModifier(new LCSModifier());
    
    return m_timeLine;
}

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void MovableAvatar::OnMessage(Message* msg)
{
    Avatar::OnMessage(msg);
    if (msg->getType() == MSG_TEST) 
    {

        Dump();
/*            CalSkeleton *skel = m_calModel->getSkeleton();

            CalBone *bone = skel->getBone(0);

            CalQuaternion currRotation = bone->getRotation();
            Quat newRot(1.5f, Vec(0,0,1)); 
            currRotation *= QuatToCalQuat(newRot);
            bone->setRotation(currRotation);

            bone->calculateState();
*/
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
//            StartTimeLine(CreateTestTimeLine());
              StartTimeLine();
//            cout << "MovableAvatar : blendCycle -> " << MOTION_WALK_LOOP << std::endl;
//            int animId = GetMotion(MOTION_WALK_LOOP)->getAnimID();
//            m_calModel->getMixer()->blendCycle(animId, 1.0f, 0.0f);
        }
    }
    else if (msg->getType() == MSG_CONTROL_STOP)
    {
        if (  getName().compare(msg->getParam()->getStrValue()) == 0   )
        {
            cout << "MovableAvatar : will stop TimeLine "<< std::endl;
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
