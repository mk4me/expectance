/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "cal3danimprovider.h"
#include "gil/gil_manager.h"
#include "../avatar/osgavatarfactory.h"
#include "../avatar/osgavatartype.h"

using namespace ft;

void Cal3dAnimProvider::Init()
{
	gil::GIL_Manager::getInstance()->RegisterHandler(GIL_DataProvider::iface_name,  this);
}


gil::GIL_Animation* Cal3dAnimProvider::getAnimation(const std::string& avatarType, const std::string& animName)
{
	gil::GIL_Animation* result = NULL;
	OsgAvatarType* avType = OsgAvatarFactory::getOsgAvatarFactoryInstance()->getAvatarType(avatarType);

	if (avType != NULL)
	{
		result = avType->GetMotion(animName);
	}

	return result;
}
