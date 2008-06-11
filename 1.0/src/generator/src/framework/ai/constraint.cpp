/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "constraint.h"

using namespace ft;

/// \brief Constructor
Constraint::Constraint()
{
    //empty
}

// \brief Destructor
Constraint::~Constraint()
{
    //empty
}

/// \brief Releases all resources and objects related to this object
void Constraint::Destroy(void)
{
}

/**
 * \brief Checks if this Constraint is satisfied
 *
 * \return bool - true if the constraint is satisfied, false otherwise (in this case the proper action should be taken)
 **/
bool Constraint::Check(ActionAvatar *av)
{
    return true;
}
