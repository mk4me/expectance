/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CAL3D_ANIMEXECUTION_H
#define _FT_CAL3D_ANIMEXECUTION_H

#include "gil/gil_animexecution.h"
#include "cal3d/cal3d.h"

namespace ft
{
	/**
	 * An a class that implemets Evolution for Cal3d
	 **/
	class Cal3dAnimExecution : public gil::GIL_AnimExecution
    {
	public:
		Cal3dAnimExecution(CalAnimation* anim) { m_anim = anim; }

		CalAnimation* getAnimation() { return m_anim; }

	private:
		CalAnimation* m_anim;
    };

}
#endif //_FT_CAL3D_ANIMEXECUTION_H
