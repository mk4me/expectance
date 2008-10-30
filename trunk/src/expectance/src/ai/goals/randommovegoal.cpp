/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "randommovegoal.h"
#include "../../utility/randomgen.h"
//#include "../../app/gendebug.h"
#include <iostream>

using namespace ft;
using namespace std;

/////////////////////  RandomMoveController  /////////////////////

/// \brief constructor
RandomMoveController::RandomMoveController(float desiredGoalTime)
{
    m_sumTime = 0.0f;
    m_desiredGoalTime = desiredGoalTime;
}

/// \brief destructor
RandomMoveController::~RandomMoveController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void RandomMoveController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    GoalController::Apply(elapsedSeconds, timeLineContext);

    if (!isGoalReached())
    {
        m_sumTime += elapsedSeconds;

		if (m_sumTime > m_desiredGoalTime)
        {
            setGoalReached(true);


//            if (GenDebug::AI>0)
//               _dbg << " RandomMoveController::Apply .. FINISH reached " << endl; 
        }
    }
}


/// \brief Resets parameters of this modifier
void RandomMoveController::Reset(TimeLineContext * timeLineContext)
{
    GoalController::Reset(timeLineContext);
    m_sumTime = 0.0f;
    setGoalReached(false);
}

std::string RandomMoveController::toString()
{
    std::string result = TimeLineObject::toString() + "[RandomMoveController]";
    return result;
}


////////////////////////  RandomMoveGoal /////////////////////////

/// \brief Constructor
RandomMoveGoal::RandomMoveGoal()
{
}

// \brief Destructor
RandomMoveGoal::~RandomMoveGoal()
{
    //empty
}

void RandomMoveGoal::OnEnter(Avatar* av)
{
	
//	cout << " RandomMove::OnEnter " << endl;

//	int action_id = RandomGenerator::RanIntValue(2);
//	if (action_id ==0)
	{
//		cout << " RandomMove::OnEnter execute walk " << endl;
		if (av->getCurrTopMotion()!=NULL)
		{
			if (av->getCurrTopMotion()->getName().compare("idle") == 0)
				av->ExecuteAction("walk");

			if (av->getCurrTopMotion()->getName().compare("walk") == 0)
				av->ExecuteAction("run");
			

			if (av->getCurrTopMotion()->getName().compare("run") == 0)
				av->ExecuteAction("walk");
		}

    }
//	else
	{
//		cout << " RandomMove::OnEnter execute run " << endl;
//		av->ExecuteAction("run");
	}
}

void RandomMoveGoal::OnExit(Avatar* av)
{
	//cout << " RandomMove::OnExit " << endl;
}

int RandomMoveGoal::getActionToPerform()
{
    int action_id = RandomGenerator::RanIntValue(2);
    action_id++;
    return action_id;
}

GoalController* RandomMoveGoal::CreateController(Avatar *av)
{
    return new RandomMoveController(3);
}

