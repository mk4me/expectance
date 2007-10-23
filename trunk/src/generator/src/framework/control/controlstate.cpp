/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "controlstate.h"
#include <iostream>

using namespace ft;

/// \brief Initializes this for particular Avatar
/// \param MovableAvatar* avatar - avatar to which this state will be connected
void ControlState::Init(MovableAvatar* avatar)
{
    std::cout << toString() << ": Init " << std::endl;
}

/// \brief Callback method called when related Avatar gets to ths state
/// \param MovableAvatar* avatar - avatar to which this state is connected
/// \param ControlState* oldState - previous state in which was Avatar before it gets into this state
void ControlState::Entry(MovableAvatar* avatar, ControlState* oldState)
{
    std::cout << toString() << ": Entry " << std::endl;
}

/// \brief Callback method called when related Avatar exits from this state to another state
/// \param MovableAvatar* avatar - avatar to which this state is connected
/// \param ControlState* newState - next state to which releted Avatar is going to get from this state
void ControlState::Exit(MovableAvatar* avatar, ControlState* newState)
{
    std::cout << toString() << ": Exit " << std::endl;
}

/// \brief Resets all paremeters releated to this state
/// \param MovableAvatar* avatar - avatar to which this state is connected
void ControlState::Reset(MovableAvatar* avatar)
{
    std::cout << toString() << ": Reset " << std::endl;
}

/// \brief Returns string representation of this state
/// \return std::string - string representation 
std::string ControlState::toString()
{
    return "<ControlState>";
}

