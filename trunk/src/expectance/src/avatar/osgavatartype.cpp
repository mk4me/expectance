/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: abak
 */

#include "osgavatartype.h"
//#include "../app/gendebug.h"
#include "../motion/transform.h"
#include "../utility/stringhelper.h"

using namespace ft;

/**
 * \brief Constructor for avatar type
 *
 * \param const std::string - typeName
 **/
OsgAvatarType::OsgAvatarType(const std::string &typeName, osgCal::CoreModel* coreModel)
{
	m_osgCoreModel = coreModel;
	InitMotions();
	InitTransforms();
}

/**
 * \brief Destructor
 *
 **/
OsgAvatarType::~OsgAvatarType()
{
    //TODO: delete all motions of this avatar type

	//TODO:  delete m_osgCoreModel when it is possible - right now destructor is provate !!
	//delete m_osgCoreModel;
}

/**
 * \brief Adds motion (animation) to avatar type
 *oi
 * \param ft::Animation * motion - motion that represents animation 
 * \return bool - true if operation succeed, false if motion with the same name is already added to avatar type
 **/
bool OsgAvatarType::AddMotion(Animation* motion)
{
    std::string _id = motion->getAnimName();
	if (!_id.empty())
	{
        if (m_motions.size() > 0)
        {
	 	    std::map<std::string,Animation*>::iterator it = m_motions.find(_id);

		    if ( it!=m_motions.end())
            { 
                //if (GenDebug::ERR)
                //    _dbg << GenDebug::ERR_STR << "CalAvatarType::AddMotion motion " << _id << " already added to Avatar " << std::endl;

			    return false;
		    }
        }
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}

    //if (GenDebug::MOTION>0)
    //    _dbg << " CalAvatarType::AddMotion motion " << _id << " added to avatar " << std::endl;

	return true;
}
/**
 * \brief Removes motion (animation) from avatar type
 *
 * \param ft::Animation * motion - moton to remove
 * \return bool - true if motion removed, false if given motion not found for this avatar type
 **/
bool  OsgAvatarType::RemoveMotion(Animation* motion)
{
    std::string _id = motion->getAnimName();

    if (!_id.empty())
	{
	 	std::map<std::string,Animation*>::iterator it = m_motions.find(_id);
		if ( it!=m_motions.end()) { 
            m_motions.erase(it);

            //if (GenDebug::MOTION>0)
            //    _dbg << "CalAvatarType::AddMotion motion " << _id << " removed form avatar " << std::endl;

			return true;
		}
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}

    //if (GenDebug::ERR)
    //    _dbg << GenDebug::ERR_STR << "CalAvatarType::AddMotion motion " << _id << " not found in Avatar " << std::endl;
	return false;
}

/**
 * \brief Gets motion with given name
 *
 * \param std::string motionName - name of motion (animation)
 * \return ft::Animation * - requested motion or NULL if the is no motion with this name for this avatar type
 **/
Animation* OsgAvatarType::GetMotion(std::string motionName)
{
    Animation * motion = NULL;
 	std::map<std::string,Animation*>::iterator it = m_motions.find(motionName);
	if ( it!=m_motions.end()) { 
        motion = dynamic_cast<Animation*>(it->second);
    }
    return motion;
}

/**
 * \brief Collects all motions (animations) for this CalCoreModel
 **/
void OsgAvatarType::InitMotions()
{
//    FootDetector* _fd = getFootDetector();
	CalCoreModel* calCoreModel = m_osgCoreModel->getCalCoreModel();

	if (calCoreModel != NULL)
	{
		int animCount = calCoreModel->getCoreAnimationCount();
		for (int i=0; i<animCount; i++)
		{
			CalCoreAnimation* anim = calCoreModel->getCoreAnimation(i);
			std::string animFileName = anim->getFilename();

			std::string animName = StringHelper::GetFileNameForPath(animFileName);
			Animation* mot = new Animation(anim, animName, i);
	//	_fd->AddLimits(animName, mot->footLimits);

			this->AddMotion(mot);
		}
	}
	else
	{
		//TODO: print ERROR
	}
}

void OsgAvatarType::InitTransforms()
{
    //inits transform for motions
   	std::map<std::string,Animation*>::iterator it = m_motions.begin();

    for( ; it != m_motions.end(); ++it ) 
    {
        Animation* mot = it->second;
		CalCoreAnimation* coreAnim = getCoreModel()->getCalCoreModel()->getCoreAnimation(mot->getAnimID());
        Transform* transform = new Transform();
        transform->InitForAnim(coreAnim);
        mot->setMotionData(new MotionData(transform));
    }
}



/**
 * \brief Prints debug information describing this avatar on output console
 *
 **/
/*void CalAvatarType::Dump()
{
    if (GenDebug::MOTION>0)
    {
        _dbg << "Dump CalAvatarType content: " << std::endl;
        _dbg << "- motions: " << std::endl;

	    std::map<std::string,Animation*>::iterator it=m_motions.begin();
	    for( ; it != m_motions.end(); ++it ) 
        {
            _dbg << " - - id " << it->first << std::endl;
        }
    }
}   
*/

