/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "limitedareaconstraint.h"
#include "../../avatar/calavatar.h"

using namespace ft;
using namespace std;

/// \brief Constructor
LimitedAreaConstraint::LimitedAreaConstraint()
{
    //empty
}

// \brief Destructor
LimitedAreaConstraint::~LimitedAreaConstraint()
{
    //empty
}

/**
 * \brief Checks if this Constraint is satisfied
 *
 * \return bool - true if the constraint is satisfied, false otherwise (in this case the proper action should be taken)
 **/
bool LimitedAreaConstraint::Check(Avatar *av)
{
	CalAvatar* avImpl = (CalAvatar*)av->getImplementation();
    bool result = true;
    float dist = avImpl ->getPosition().length();
    if (dist > 1200)
        result = false;
    else
        result = true;

    return result;
}

