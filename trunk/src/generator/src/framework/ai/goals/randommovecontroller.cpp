/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "randommovecontroller.h"


using namespace ft;

/// \brief constructor
RandomMoveController::RandomMoveController(float desiredGoalTime)
{
    m_sumTime = 0.0f;
    m_desiredGoalTime = desiredGoalTime;
    setGoalReached(false);
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
            cout << " RandomMoveController::Apply .. FINISH reached " << endl; 
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

