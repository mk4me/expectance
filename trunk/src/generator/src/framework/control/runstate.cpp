/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "runstate.h"

using namespace ft;
using namespace std;

void RunState::Init(MovableAvatar* avatar)
{
    ControlState::Init(avatar);
}

void RunState::Entry(MovableAvatar* avatar)
{
    ControlState::Entry(avatar);
}

void RunState::Exit(MovableAvatar* avatar)
{
    ControlState::Exit(avatar);
}

void RunState::Reset(MovableAvatar* avatar)
{
    ControlState::Reset(avatar);
}

std::string RunState::toString()
{
    return "<RunState>";
}

