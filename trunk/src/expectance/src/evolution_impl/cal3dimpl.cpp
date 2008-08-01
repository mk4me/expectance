/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "cal3dimpl.h"
#include "gil/gil_manager.h"
#include "../avatar/osgavatarfactory.h"


using namespace ft;

void Cal3dImpl::Init()
{
	gil::GIL_Manager::getInstance()->RegisterHandler(GIL_Factory::iface_name,  this);
}

gil::GIL_Avatar* Cal3dImpl::CreateAvatar(const std::string& id, const std::string& type)
{
	OsgAvatar* avatar = OsgAvatarFactory::getOsgAvatarFactoryInstance()->CreateAvatar(type, id);
	return avatar;
}

