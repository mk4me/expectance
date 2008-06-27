/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CAL3DANIMPROVIDER_H
#define _FT_CAL3DANIMPROVIDER_H

#include "gil/gil_dataprovider.h"

namespace ft
{
	/**
	 * An a class that implemets Evolution animation provider for Cal3d
	 **/
	class Cal3dAnimProvider: public gil::GIL_DataProvider
    {
	public:
		void Init();
		gil::GIL_Animation* getAnimation(const std::string& avatarType, const std::string& animName);
    };

}
#endif //_FT_CAL3DANIMPROVIDER_H
