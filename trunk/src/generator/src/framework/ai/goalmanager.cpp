/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "goalmanager.h"
#include "../utility/debug.h"
#include "goals/randommovegoal.h"
#include "goals/randommovecontroller.h"

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
    Goal* goal = new RandomMoveGoal();
    AddGoal(goal);
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
 * \brief Update goal for a given avatar
 *
 * \param ft::AIAvatar* av - avatar for which a goal is to update
 *
 **/
void GoalManager::UpdateAvatarGoal(AIAvatar *av)
{
    CheckCurrGoalReached(av);

    std::vector<Goal*> vPossibleGoals;
    FindPossibleGoals(vPossibleGoals, av);
    EsimateGoalsValue(vPossibleGoals, av);

    Goal* selectedGoal = SelectGoal(vPossibleGoals, av);

    if (selectedGoal != NULL)
    {
        //Goal* currGoal = av->getCurrGoal();
        //std::string id = av->getCurrGoal()->getID();
        //cout << " Selected goal is " << selectedGoal->getID() << endl;
        if (av->getCurrGoal() == NULL || (av->getCurrGoal()->getID().compare(selectedGoal->getID()) != 0))
        {
            ExecuteGoal(selectedGoal, av);
        }
    }

    //cout << " size " << vPossibleGoals.size() << endl; 
    //for (int m=0; m<(int)vPossibleGoals.size(); m++)
    //{
    //    cout << (Goal*)vPossibleGoals[m] << endl; 
    //}

    vPossibleGoals.clear();
}

void GoalManager::CheckCurrGoalReached(AIAvatar *av)
{
    AIController* currGoalController = av->getGoalController();
    if (currGoalController != NULL && currGoalController->isGoalReached())
    {
        if (currGoalController->getControlledGoal() == av->getCurrGoal())
        {
            av->setCurrGoal(NULL); 
            av->setGoalController(NULL);
            bool deleted = av->getTimeLine()->RemoveModifier(currGoalController);
            if (deleted)
            {
                delete currGoalController;
            }
            else
            {
                cout << "ERR: GoalManager::CheckCurrGoalReached: removing aicontroller " << currGoalController 
                    << " from timeline of " << av->toString() << " failed !!!! " << endl;
            }
        }
        else
        {
            cout << "ERR: GoalManager::CheckCurrGoalReached:  goal referenced by controller " << currGoalController->getControlledGoal()
                << " differs from goals referenced by " << av->toString() << " (" << av->getCurrGoal() << ")" << endl;
        }

    }

}

void GoalManager::FindPossibleGoals(std::vector<Goal*> &vPossibleGoals, AIAvatar *av)
{
   	std::map<std::string,Goal*>::iterator it=m_goals.begin();
	for( ; it != m_goals.end(); ++it )
    {
        vPossibleGoals.push_back(it->second);
    }
    //only for test
//    if (vPossibleGoals.size() == 0)
//        vPossibleGoals.push_back(new Goal());
}

void GoalManager::EsimateGoalsValue(std::vector<Goal*> &vPossibleGoals, AIAvatar *av)
{
    //empty yet
}

Goal* GoalManager::SelectGoal(std::vector<Goal*> &vPossibleGoals, AIAvatar *av)
{
    Goal* selectedGoal = NULL;

    if (m_goals.size()>0)
    {
        selectedGoal = (Goal*)(m_goals.begin()->second);
    }
    
    return selectedGoal;
}

void GoalManager::ExecuteGoal(Goal* goal, AIAvatar *av)
{
    av->setCurrGoal(goal);
    av->setGoalController(new RandomMoveController(3));
    av->getGoalController()->setControlledGoal(goal);
    av->getTimeLine()->AddModifier(av->getGoalController());

    int actionId = goal->getActionToPerform();
    if (actionId != -1)
    {
        Action* action = av->SetActionById(actionId);
        if (action == NULL)
        {
            cout << " ERR: GoalManager::ExecuteGoal - execution of action needed to Goal realization failed. " << endl;
        }
    }
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
