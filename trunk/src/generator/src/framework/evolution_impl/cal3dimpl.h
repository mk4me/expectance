/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CAL3D_IMPL_H
#define _FT_CAL3D_IMPL_H

#include "gil/gil_factory.h"

namespace ft
{
	/**
	 * An a class that implemets Evolution for Cal3d
	 **/
	class Cal3dImpl : public gil::GIL_Factory
    {
	public:
		void Init();
		gil::GIL_Avatar* CreateAvatar(const std::string& id, const std::string& type);
    };

}
#endif //_FT_CAL3D_IMPL_H
