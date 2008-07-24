/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "player.h"

using namespace ft;

Player::Player(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
:CalAvatar(calModel, calCoreModel, modelName)
{
	m_restPosition = CalVector();
}

Player::~Player()
{
}

