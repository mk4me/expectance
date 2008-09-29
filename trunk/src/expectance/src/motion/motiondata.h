/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_MOTION_DATA_H
#define _FT_MOTION_DATA_H

#include "gil/gil_animdata.h"

#include "transform.h"

namespace ft
{
	/** 
	 * An wrapper for ft::AnimData from evolution packet
	 */
	class MotionData : public gil::GIL_AnimData
    {
	public:
		MotionData(Transform* trans) { m_transform = trans; }
		void setTransform(Transform* trans) { m_transform = trans; }
		Transform* getTransform() { return m_transform; }

	private:
		Transform* m_transform;

    };
}
#endif //_FT_MOTION_DATA_H

