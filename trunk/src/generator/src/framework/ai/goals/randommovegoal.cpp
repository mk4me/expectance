/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "randommovegoal.h"
#include "utility/randomgen.h"
#include "../../app/gendebug.h"

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
    AIController::Apply(elapsedSeconds, timeLineContext);

    if (!isGoalReached())
    {
        m_sumTime += elapsedSeconds;

        if (m_sumTime > m_desiredGoalTime)
        {
            setGoalReached(true);

            if (GenDebug::AI>0)
                _dbg << " RandomMoveController::Apply .. FINISH reached " << endl; 
        }
    }
}


/// \brief Resets parameters of this modifier
void RandomMoveController::Reset(TimeLineContext * timeLineContext)
{
    AIController::Reset(timeLineContext);
    m_sumTime = 0.0f;
    setGoalReached(false);
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

int RandomMoveGoal::getActionToPerform()
{
    int action_id = RandomGenerator::RanIntValue(2);
    action_id++;
    return action_id;
}

AIController* RandomMoveGoal::CreateController(ActionAvatar *av)
{
    return new RandomMoveController(3);
}
