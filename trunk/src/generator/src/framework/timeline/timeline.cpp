/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timeline.h"

using namespace ft;

/**
 * \brief OVERRIDEN from ft::TimeLineObject
 **/
bool TimeLine::AddSubObject(TimeLineObject* object, int where_to_add)
{
    if (Debug::TIMELINE>0)
        _dbg << "TimeLine::AddSubObject " << object->toString() << endl;

    return TimeLineObject::AddSubObject(object, where_to_add);
}

/**
 * \brief OVERRIDEN from ft::TimeLineObject
 **/
void TimeLine::RemoveSubObject(TimeLineObject* obj_to_delete, bool deleteFromMemory)
{
    if (Debug::TIMELINE>0)
        _dbg << "TimeLine::RemoveSubObject " << obj_to_delete->toString() << endl;

    TimeLineObject::RemoveSubObject(obj_to_delete, deleteFromMemory);
}

