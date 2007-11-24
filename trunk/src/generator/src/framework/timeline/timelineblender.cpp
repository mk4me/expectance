/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineblender.h"

using namespace ft;

//to use it in cloning operation
TimeLineObject* TimeLineBlender::CreateInstance()
{
    return new TimeLineBlender(m_overlap);
}

//OVERRIDEN
TimeLineObject* TimeLineBlender::Clone()
{
    TimeLineObject* cloneOfThis = (TimeLineBlender*)TimeLineObject::Clone();
    return cloneOfThis;
}
