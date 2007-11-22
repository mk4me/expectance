/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "goalmanager.h"
#include "../utility/debug.h"
#include "goals/randommovegoal.h"
#include "goals/changedirgoal.h"
#include "goals/limitedareagoal.h"
#include "constraints/limitedareaconstraint.h"
#include "../utility/randomgen.h"
#include "rule.h"


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
        if (Debug::AI>0)
            _dbg << "GoalManager::getInstace(): instance of GoalManager created " << endl;

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

    goal = new ChangeDirGoal();
    AddGoal(goal);

    Rule* rule = new Rule(new LimitedAreaConstraint(), new LimitedAreaGoal());
    AddRule(rule);
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

            if (Debug::ERR)
                _dbg << Debug::ERR_STR << "GoalManager::AddGoal goal " << _id << " already added to GoalManager " << std::endl;

			return false;
		}
	    m_goals.insert( std::make_pair( std::string(_id), pGoal) );
	}

    if (Debug::AI>0)
        _dbg << " GoalManager::AddGoal goal " << _id << " added to GoalManager " << std::endl;

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
            
            if (Debug::AI>0)
                _dbg << "GoalManager::RemoveGoal object " << _id << " removed form GoalManager " << std::endl;

			return true;
		}
	    m_goals.insert( std::make_pair( std::string(_id), pGoal) );
	}

    if (Debug::ERR)
        _dbg << Debug::ERR_STR <<"GoalManager::RemoveGoal object " << _id << " not found in CGoalManager " << std::endl;
	return false;
}

/**
 * \brief Adds rule to list of rules
 *
 * \param ft::Rule* rule - rule to add
 * \return bool - true if rule added successfuly
 **/
bool GoalManager::AddRule(Rule* rule)
{
    if (Debug::AI>0)
        _dbg << " AddRule " << rule->toString() << " to AIManager " << std::endl;

    m_vRules.push_back(rule);
	return true;
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

    if (av->isThink())
    {
        std::vector<Goal*> vPossibleGoals;
        int heighest_priority = FindPossibleGoals(vPossibleGoals, av);

        if (av->getCurrGoal() == NULL || heighest_priority > av->getCurrGoal()->getPriority())
        {
            EsimateGoalsValue(vPossibleGoals, av);

            Goal* selectedGoal = SelectGoal(vPossibleGoals, av);

            if (selectedGoal != NULL)
            {
                if (av->getCurrGoal() == NULL || (av->getCurrGoal()->getID().compare(selectedGoal->getID()) != 0))
                {
                    ExecuteGoal(selectedGoal, av);
                }
            }
            vPossibleGoals.clear();
        }
    }
}

void GoalManager::CheckCurrGoalReached(AIAvatar *av)
{
    AIController* currGoalController = av->getGoalController();
    if (currGoalController != NULL && currGoalController->isGoalReached())
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
            if (Debug::ERR)
                _dbg << Debug::ERR_STR << "GoalManager::CheckCurrGoalReached: removing aicontroller " << currGoalController 
                    << " from timeline of " << av->toString() << " failed !!!! " << endl;
        }
    }

}

/**
 * \brief Fills given vector with possible goals for particular avatar
 *
 * \return int - the highest priority among possible goals
 **/
int GoalManager::FindPossibleGoals(std::vector<Goal*> &vPossibleGoals, AIAvatar *av)
{
    int heighest_priority = PRIORITY_LOW;

    int size = (int)m_vRules.size();
    for (int i=0; i<size; i++)
    {
        Rule* rule = m_vRules[i];
        if (av->getCurrGoal() == NULL || av->getCurrGoal() != rule->getGoal())
        {
            if (! rule->getConstraint()->Check(av))
            {
                vPossibleGoals.push_back(rule->getGoal());
                if (rule->getGoal()->getPriority() > heighest_priority)
                {
                    heighest_priority = rule->getGoal()->getPriority();
                }

            }
        }
    }

    // find the heighest priority -----------
    
  	std::map<std::string,Goal*>::iterator it=m_goals.begin();
	for( ; it != m_goals.end(); ++it )
    {
        Goal* goal = (Goal*)it->second;
        if (goal->getPriority() > heighest_priority)
        {
            heighest_priority = goal->getPriority();
        }
    }

    //add possible goals with priority at least equal to heighest_priority
   	it=m_goals.begin();
	for( ; it != m_goals.end(); ++it )
    {
        Goal* goal = (Goal*)it->second;
        if (goal->getPriority() >= heighest_priority)
        {
            vPossibleGoals.push_back(it->second);
        }
    }

    //_dbg << " Highest priority ------------------- " << heighest_priority << endl;

    return heighest_priority;
}

void GoalManager::EsimateGoalsValue(std::vector<Goal*> &vPossibleGoals, AIAvatar *av)
{
    //empty yet
}

Goal* GoalManager::SelectGoal(std::vector<Goal*> &vPossibleGoals, AIAvatar *av)
{
    Goal* selectedGoal = NULL;

    int sumCounter = vPossibleGoals.size();
    int selectedGoalNo = RandomGenerator::RanIntValue(sumCounter);

    //_dbg << " ---------------- selectedGoalNo -- " << selectedGoalNo << " from " << sumCounter << endl;

    int size = (int)vPossibleGoals.size();
    for (int i=0; i<size; i++)
    {
        if (i==selectedGoalNo)
        {
            selectedGoal = vPossibleGoals[i];
            break;
        }
    }

    return selectedGoal;
}

void GoalManager::ExecuteGoal(Goal* goal, AIAvatar *av)
{
//    _dbg << av->toString() << " ----------- GoalManager::ExecuteGoal: " << goal->toString() << endl;

    av->setCurrGoal(goal);
    av->setGoalController(goal->CreateController(av));
//    av->getGoalController()->setControlledGoal(goal);
    av->getTimeLine()->AddModifier(av->getGoalController());

    bool actionExecuted = goal->ExecuteAction(av);

    if (!actionExecuted)
    {
        if (Debug::ERR)
            _dbg << Debug::ERR_STR << "GoalManager::ExecuteGoal - execution of action needed to Goal realization failed. " << endl;
    }
}

/**
 * \brief Prints debug information describing GoalManager on output console
 *
 **/
void GoalManager::Dump()
{
    _dbg << "Dump GoalManager content: " << std::endl;
    _dbg << "- objects: " << std::endl;

	std::map<std::string,Goal*>::iterator it=m_goals.begin();
	for( ; it != m_goals.end(); ++it ) 
    {
        _dbg << " - - id " << it->first << std::endl;
    }
}

