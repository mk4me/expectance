/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "restposconstraint.h"
#include "player.h"

using namespace ft;
using namespace std;

/// \brief Constructor
RestPosConstraint::RestPosConstraint()
{
    //empty
}

// \brief Destructor
RestPosConstraint::~RestPosConstraint()
{
    //empty
}

/**
 * \brief Checks if this Constraint is satisfied
 *
 * \return bool - true if the constraint is satisfied, false otherwise (in this case the proper action should be taken)
 **/
bool RestPosConstraint::Check(Avatar *av)
{
	Player* avImpl = (Player*)av->getImplementation();

	bool result = true;
	CalVector dist = avImpl->getPosition() - avImpl->getRestPos();
	if (dist.length() > 300)
        result = false;
    else
        result = true;

    return result;
}

