/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "transform.h"

#include "cal3d/coretrack.h"
#include "cal3d/corekeyframe.h"


using namespace ft;

void Transform::InitForAnim(CalCoreAnimation* coreAnim)
{
    CalVector pos;
    CalQuaternion rot;
    (coreAnim->getCoreTrack(0))->getState(0.0f,pos,rot);

	setOrigPosition(CalVector(pos.x, pos.y, pos.z));

	CalVector startPos;
	CalVector endPos;
	CalQuaternion tmpRot; 

	coreAnim->getCoreTrack(0)->getState(0, startPos, tmpRot);
	coreAnim->getCoreTrack(0)->getState(coreAnim->getDuration(), endPos, tmpRot);

	CalVector diff = endPos - startPos;
	setEntireTranslation(diff);
}

