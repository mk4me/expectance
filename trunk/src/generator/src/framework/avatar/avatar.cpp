/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "avatar.h"
#include "../utility/debug.h"

using namespace ft;


Avatar::Avatar(CalModel* calModel, CalCoreModel* calCoreModel)
{
    m_calModel = calModel;
    m_calCoreModel = calCoreModel;

}

void Avatar::SetCalModel(CalModel* calModel)
{
    m_calModel = calModel;
}

CalModel* Avatar::GetCalModel()
{
    return m_calModel;
}

void Avatar::SetCalCoreModel(CalCoreModel* calCoreModel)
{
    m_calCoreModel = calCoreModel;
}

CalCoreModel* Avatar::GetCalCoreModel()
{
    return m_calCoreModel;
}

void Avatar::OnMessage(Message& msg)
{
    DBG("Avatar::OnMessage: received");
}

void Avatar::OnUpdate(float elapsedTime)
{

}