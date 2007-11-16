/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "movableavatar.h"
#include "../timeline/lcsmodifier.h"
#include "timelinevisualizer.h"
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
    m_tlExecutor = NULL;
    setName(modelName);

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
        m_timeLine = NULL;
    }

    if (m_tlExecutor != NULL)
    {
        m_tlExecutor->Destroy();
        delete m_tlExecutor;
        m_tlExecutor = NULL;
    }

    if (m_timeLineContext != NULL)
    {
        m_timeLineContext->Destroy();
        delete m_timeLineContext;
        m_timeLineContext = NULL;
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
        if (m_tlExecutor != NULL)
        {
            m_tlExecutor->Initiate(m_timeLine, m_timeLineContext);
            m_tlExecutor->Start(); 
        }
    }
}

/**
 * \brief Stops currently assigned timeline (if any)
 **/
void MovableAvatar::StopTimeLine()
{
    if (m_tlExecutor != NULL)
        m_tlExecutor->StopRequest();

    if (m_timeLine != NULL)
    {
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
    
    m_calModel->update(elapsedSeconds);
    
    if (m_tlExecutor != NULL)
        m_tlExecutor->UpdateMotions(elapsedSeconds);

    if (m_tlExecutor != NULL)
        m_tlExecutor->UpdateModifiers(elapsedSeconds);

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
  ctx->stop_immediate = true;

  ctx->remove_after_execution = true;

  ctx->setAvatar(this);
  setTimeLineContext(ctx);

  m_tlExecutor = new TimeLineExecutor();
  setTLExecutor(m_tlExecutor);

  CreateTestTimeLine();

  m_bPaused = false;
  m_blendTime = 0.3f;

}

/**
 * \brief This method is called when new frame is updated from ft::UpdateManager
 *
 * \param float elapsedSeconds - time elapsed sice last update
 **/
void MovableAvatar::OnUpdate(const double elapsedSeconds)
{
    if(!m_bPaused)
    {
        UpdateTimeLine(elapsedSeconds);
    }
}

/**
 * \brief Resets all parameters of avatar with regard to motion layer
*/
void MovableAvatar::Reset()
{
     cout << toString() << " MovableAvatar::Reset() " << std::endl;
    if (getTimeLine() != NULL)
    {
        getTimeLine()->Reset(getTimeLineContext());
    }

    if (getTLExecutor() != NULL)
        getTLExecutor()->Reset();
}

/**
 * \brief Creates TimeLine object for testing purpose
 *
 * \return ft::TimeLine * - TimeLine for testing framework
 **/
TimeLine* MovableAvatar::CreateTestTimeLine()
{
//    if (getName().compare("FirstAvatar") == 0   )
    {
        setLCSModifier(new LCSModifier());
        getTimeLine()->AddModifier(getLCSModifier());
		
        if ((Config::getInstance()->IsKey("track_on")) && (Config::getInstance()->GetIntVal("track_on")==1))
        {
            getTimeLine()->AddModifier(new TimeLineVisualizer());
        }
    
    }

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
    if (msg->getType() == MSG_DUMP_STATE) 
    {

        this->Dump();
        if (getTLExecutor()!=NULL)
        {
            getTLExecutor()->Dump();
        }
    } 
    else if (msg->getType() == MSG_START_SIMULATION)
    {
        cout << toString() << " start simulation .. " << std::endl;
        if (!getTimeLine()->isStarted())
        {
            StartTimeLine();
        }
    }  
    else if (msg->getType() == MSG_RESTART_SIMULATION)
    {
        Reset();
    }  
    else if (msg->getType() == MSG_CONTROL_PAUSE)
    {
        m_bPaused = !m_bPaused;
    } 
 }
