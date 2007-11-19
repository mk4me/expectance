/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "randommovegoal.h"
#include "../../utility/randomgen.h"

using namespace ft;
using namespace std;

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
    int action_id = RandomGenerator::RanIntValue(3);
    return action_id;
}
