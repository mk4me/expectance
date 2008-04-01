/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "avatartype.h"
#include "../app/gendebug.h"
#include "cal3d/src/coretrack.h"
#include "scene/transformmanager.h"

using namespace ft;

/**
 * \brief Constructor for avatar type
 *
 * \param const std::string - typeName
 **/
AvatarType::AvatarType(const std::string &typeName):
Cal3dType(typeName)
{
}

/**
 * \brief Destructor
 *
 **/
AvatarType::~AvatarType()
{
    //TODO: delete all motions of this avatar type
}

/**
 * \brief Adds motion (animation) to avatar type
 *oi
 * \param ft::Motion * motion - motion that represents animation 
 * \return bool - true if operation succeed, false if motion with the same name is already added to avatar type
 **/
bool AvatarType::AddMotion(Motion* motion)
{
    std::string _id = motion->getAnimName();
	if (!_id.empty())
	{
        if (m_motions.size() > 0)
        {
	 	    std::map<std::string,Motion*>::iterator it = m_motions.find(_id);

		    if ( it!=m_motions.end())
            { 
                if (GenDebug::ERR)
                    _dbg << GenDebug::ERR_STR << "AvatarType::AddMotion motion " << _id << " already added to Avatar " << std::endl;

			    return false;
		    }
        }
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}

    if (GenDebug::MOTION>0)
        _dbg << " AvatarType::AddMotion motion " << _id << " added to avatar " << std::endl;

	return true;
}
/**
 * \brief Removes motion (animation) from avatar type
 *
 * \param ft::Motion * motion - moton to remove
 * \return bool - true if motion removed, false if given motion not found for this avatar type
 **/
bool  AvatarType::RemoveMotion(Motion* motion)
{
    std::string _id = motion->getAnimName();

    if (!_id.empty())
	{
	 	std::map<std::string,Motion*>::iterator it = m_motions.find(_id);
		if ( it!=m_motions.end()) { 
            m_motions.erase(it);

            if (GenDebug::MOTION>0)
                _dbg << "AvatarType::AddMotion motion " << _id << " removed form avatar " << std::endl;

			return true;
		}
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}

    if (GenDebug::ERR)
        _dbg << GenDebug::ERR_STR << "AvatarType::AddMotion motion " << _id << " not found in Avatar " << std::endl;
	return false;
}

/**
 * \brief Gets motion with given name
 *
 * \param std::string motionName - name of motion (animation)
 * \return ft::Motion * - requested motion or NULL if the is no motion with this name for this avatar type
 **/
Motion* AvatarType::GetMotion(std::string motionName)
{
    Motion * motion = NULL;
 	std::map<std::string,Motion*>::iterator it = m_motions.find(motionName);
	if ( it!=m_motions.end()) { 
        motion = dynamic_cast<Motion*>(it->second);
    }
    return motion;
}

/**
 * \brief Collects all motions (animations) for this CalCoreModel
 **/
void AvatarType::InitMotions()
{
//    FootDetector* _fd = getFootDetector();
    int animCount = getCoreAnimationCount();
    for (int i=0; i<animCount; i++)
    {
        CalCoreAnimation* anim = getCoreAnimation(i);
        std::string animName = anim->getFilename();

        Motion* mot = new Motion(animName, i);
//	_fd->AddLimits(animName, mot->footLimits);

        this->AddMotion(mot);
    }
}

void AvatarType::InitTransform(bool source_3dsmax)
{
    Cal3dType::InitTransform(source_3dsmax);

    //inits transform for motions
   	std::map<std::string,Motion*>::iterator it = m_motions.begin();

    for( ; it != m_motions.end(); ++it ) 
    {
        Motion* mot = it->second;
        CalCoreAnimation* coreAnim = getCoreAnimation(mot->getAnimID());
        Transform* transform = new Transform(TRANSFORM_FROM_3DSMAX);
        transform->InitForAnim(coreAnim, source_3dsmax);
        mot->setTransform(transform);
    }
}


/**
 * \brief Prints debug information describing this avatar on output console
 *
 **/
void AvatarType::Dump()
{
    if (GenDebug::MOTION>0)
    {
        _dbg << "Dump AvatarType content: " << std::endl;
        _dbg << "- motions: " << std::endl;

	    std::map<std::string,Motion*>::iterator it=m_motions.begin();
	    for( ; it != m_motions.end(); ++it ) 
        {
            _dbg << " - - id " << it->first << std::endl;
        }
    }
}   


