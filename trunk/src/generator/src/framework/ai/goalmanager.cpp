/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "goalmanager.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;

GoalManager* GoalManager::m_instance = NULL;

/// \brief Constructor
GoalManager::GoalManager()
{
    //empty
}

// \brief Destructor
GoalManager::~GoalManager()
{
    //empty
}

/**
 * \brief Returns the only instance of ft::GoalManager (creates it at first call to this method)
 *
 * \return ft::GoalManager * - the only instance of GoalManager
 **/
GoalManager* GoalManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("GoalManager::getInstace(): instance of GoalManager created ");
        m_instance = new GoalManager();
    }

    return m_instance;
}

/**
 \brief Releases all resources related to GoalManager
 *
 **/
void GoalManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * \brief Initializes GoalManager
 *
 **/
void GoalManager::Init()
{
}


/**
 * \brief Register Goal in GoalManager
 *
 * \param ft::Goal * pGoal - object that will be registered in GoalManager
 * \return bool - true if registration succeed, false otherwise
 **/
bool GoalManager::AddGoal(Goal* pGoal)
{
	std::string _id = pGoal->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,Goal*>::iterator it = m_goals.find(_id);

		if ( it!=m_goals.end()) { 
            cout << "ERR: GoalManager::AddGoal goal " << _id << " already added to GoalManager " << std::endl;
			return false;
		}
	    m_goals.insert( std::make_pair( std::string(_id), pGoal) );
	}
    cout << " GoalManager::AddGoal goal " << _id << " added to GoalManager " << std::endl;
	return true;
}

/**
 * \brief Unregisters Goal from GoalManager
 *
 * \param ft::Goal * pGoal - object that should be unregistered in GoalManager
 * \return bool - true if object has been unregistered, false otherwise
 **/
bool GoalManager::RemoveGoal(Goal* pGoal)
{
	std::string _id = pGoal->getID();

    if (!_id.empty())
	{
	 	std::map<std::string,Goal*>::iterator it = m_goals.find(_id);
		if ( it!=m_goals.end()) { 
            m_goals.erase(it);
            cout << "GoalManager::RemoveGoal object " << _id << " removed form GoalManager " << std::endl;
			return true;
		}
	    m_goals.insert( std::make_pair( std::string(_id), pGoal) );
	}
    cout << "ERR: GoalManager::RemoveGoal object " << _id << " not found in CGoalManager " << std::endl;
	return false;
}

/**
 * \brief Prints debug information describing GoalManager on output console
 *
 **/
void GoalManager::Dump()
{
    cout << "Dump GoalManager content: " << std::endl;
    cout << "- objects: " << std::endl;

	std::map<std::string,Goal*>::iterator it=m_goals.begin();
	for( ; it != m_goals.end(); ++it ) 
    {
        cout << " - - id " << it->first << std::endl;
    }
}

/**
 * \brief ??
 *
 **/
//void GoalManager::UpdateObjects(const double elapsedSeconds)
//{
//    //TODO: abak:  this update should be synchronized with adding and removing goals
//
//   	std::map<std::string,Goal*>::iterator it=m_goals.begin();
//	for( ; it != m_objects.end(); ++it )
//    {
//        it->second->OnUpdate(elapsedSeconds);
//    }
//}
