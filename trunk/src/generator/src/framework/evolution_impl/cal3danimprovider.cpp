/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "cal3danimprovider.h"
#include "gil/gil_manager.h"
#include "../avatar/avatarfactory.h"
#include "../avatar/calavatar.h"
#include "../avatar/calavatartype.h"

using namespace ft;

void Cal3dAnimProvider::Init()
{
	GIL_Manager::getInstance()->RegisterHandler(GIL_AnimProvider::iface_name,  this);
}


gil::GIL_Animation* Cal3dAnimProvider::getAnimation(const std::string& avatarType, const std::string& animName)
{
	gil::GIL_Animation* result = NULL;
	CalCoreModel* coreModel = AvatarFactory::getAvatarFactoryInstance()->getCoreModel(avatarType);
	if (coreModel != NULL)
	{
		CalAvatarType* type = (CalAvatarType* )coreModel;
		result = type->GetMotion(animName);
	}

	return result;
}