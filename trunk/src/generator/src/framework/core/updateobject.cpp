/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "UpdateObject.h"

using namespace ft;

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void UpdateObject::OnMessage(Message* msg)
{
}

/**
 * \brief This method is called when new frame is updated from ft::UpdateManager
 *
 * \param float elapsedSeconds - time elapsed sice last update
 **/
void UpdateObject::OnUpdate(const double elapsedTime)
{
}