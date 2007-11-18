/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "randommovegoal.h"

using namespace ft;

/// \brief Constructor
RandomMoveGoal::RandomMoveGoal()
{
    counter = 0;
}

// \brief Destructor
RandomMoveGoal::~RandomMoveGoal()
{
    //empty
}

int RandomMoveGoal::getActionToPerform()
{
    int action_id = 0;
    if (counter ==0)
    {
        action_id = ACTION_WALK_ID;
    }

    if (counter == 1)
    {
        action_id = ACTION_RUN_ID;
    }

    //if (counter == 2)
    //{
    //    action_id = ACTION_IDLE_ID;
    //}

    counter = (counter+1)%2;

    return action_id;
}
