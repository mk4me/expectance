/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinecontext.h"

using namespace ft;

TimeLineContext::TimeLineContext(void)
{
    SetDefaultFlags();
}

TimeLineContext::~TimeLineContext(void)
{
}

/// \brief Releases all resources and objects related to this TimeLineContext
void TimeLineContext::Destroy(void)
{
    
}

void TimeLineContext::SetValuesByFlag(int positive_flag, int negative_flag)
{
    //add_copy = true;
    //where_to_add = ADD_AS_LAST;
    //remove_after_execution = false;
    //delete_objects_after_remove_from_list = false;
    //stop_immediate = false;
}

void TimeLineContext::SetDefaultFlags()
{
    add_copy = false;
    where_to_add = ADD_AS_LAST;
    remove_after_execution = false;
    delete_objects_after_remove_from_list = false;
    stop_immediate = false;
}
