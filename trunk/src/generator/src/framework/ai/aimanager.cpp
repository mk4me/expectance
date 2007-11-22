/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "aimanager.h"
#include "goalmanager.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;

AIManager* AIManager::m_instance = NULL;

/// \brief Constructor
AIManager::AIManager(void)
{
    m_bThink = false;
}

/// \brief Destructor
AIManager::~AIManager(void) 
{ 
}

/**
 * \brief Returns the only instance of ft::AIManager (creates it at first call to this method)
 *
 * \return ft::AIManager * - the only instance of AIManager
 **/
AIManager* AIManager::getInstance()
{
    if (m_instance == NULL)
    {
        if (Debug::AI>0)
            _dbg << "AIManager::getInstace(): instance of AIManager created " << endl;;

        m_instance = new AIManager();
    }

    return m_instance;
}

/**
 \brief Releases all resources related to AIManager
 *
 **/
void AIManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * \brief Initializes AIManager
 *
 **/
void AIManager::Init()
{
    GoalManager::getInstance()->Init();
}

/// \brief Update callback called from ft::UpdateManager
/// \param double elapsedSeconds - time interval elapsed since last update
void AIManager::OnUpdate(const double elapsedSeconds)
{
    if (m_bThink)
    {
        UpadateAvatarGoals();
    }
}

/**
 * \brief Update goals for all registered avatars
 *
 **/
void AIManager::UpadateAvatarGoals()
{
    if (m_vAvatars.size() > 0)
    {
        for (int m=0; m<(int)m_vAvatars.size(); m++)
        {
            GoalManager::getInstance()->UpdateAvatarGoal((AIAvatar*)m_vAvatars[m]); 
        }
    }
}


/**
 * \brief Adds avatar to list of controlled avatars
 *
 * \param ft::AIAvatar* av - avatar to add
 * \return bool - true if avatar added successfuly
 **/
bool AIManager::AddAvatar(AIAvatar* av)
{
    if (Debug::AI>0)
        _dbg << " AddAvatar " << av->toString() << " to AIManager " << std::endl;

    m_vAvatars.push_back(av);
    av->SetThink(true);
	return true;
}


//TODO: implement RemoveAvatar
//bool  AIManager::RemoveAvatar(AIAvatar* av);

