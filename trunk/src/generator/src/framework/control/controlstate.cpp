/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "controlstate.h"
#include <iostream>

using namespace ft;

void ControlState::Init(MovableAvatar* avatar)
{
    std::cout << toString() << ": Init " << std::endl;
}

void ControlState::Entry(MovableAvatar* avatar, ControlState* oldState)
{
    std::cout << toString() << ": Entry " << std::endl;
}

void ControlState::Exit(MovableAvatar* avatar, ControlState* newState)
{
    std::cout << toString() << ": Exit " << std::endl;
}

void ControlState::Reset(MovableAvatar* avatar)
{
    std::cout << toString() << ": Reset " << std::endl;
}

std::string ControlState::toString()
{
    return "<ControlState>";
}

