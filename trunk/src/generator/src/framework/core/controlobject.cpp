/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "controlobject.h"

using namespace ft;

/**
 * \brief This method is called when message is sent to this object from ft::ControlManager
 *
 * \param ft::Message * msg - message from ft::ControlManager
 **/
void ControlObject::OnMessage(Message* msg)
{
}

/**
 * \brief This method is called when new frame is updated from ft::ControlManager
 *
 * \param float elapsedSeconds - time elapsed sice last update
 **/
void ControlObject::OnUpdate(float elapsedTime)
{
}