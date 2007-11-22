/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "action.h"
#include <iostream>

using namespace ft;

/// \brief Initializes this for particular Avatar
/// \param PhysicsAvatar* avatar - avatar to which this action will be connected
void Action::Init(PhysicsAvatar* avatar)
{
    if (Debug::ACTION)
        std::cout << toString() << ": Init " << std::endl;
}

/// \brief Callback method called when related Avatar gets to this action
/// \param PhysicsAvatar* avatar - avatar to which this action is connected
/// \param Action* oldAction - previous action in which was Avatar before it gets into this action
void Action::Entry(PhysicsAvatar* avatar, Action* oldAction)
{
    if (Debug::ACTION)
        std::cout << toString() << ": Entry " << std::endl;
}

/// \brief Callback method called when related Avatar exits from this action to another action
/// \param PhysicsAvatar* avatar - avatar to which this action is connected
/// \param Action* newAction - next action to which releted Avatar is going to get from this action
void Action::Exit(PhysicsAvatar* avatar, Action* newAction)
{
    if (Debug::ACTION)
        std::cout << toString() << ": Exit " << std::endl;
}

/// \brief Resets all paremeters releated to this action
/// \param PhysicsAvatar* avatar - avatar to which this action is connected
void Action::Reset(PhysicsAvatar* avatar)
{
    if (Debug::ACTION)
        std::cout << toString() << ": Reset " << std::endl;
}

/// \brief Returns string representation of this action
/// \return std::string - string representation 
std::string Action::toString()
{
    return "<Action>";
}

