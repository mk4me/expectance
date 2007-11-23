/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineobject.h"

using namespace ft;

/// \brief constructor
TimeLineObject::TimeLineObject()
{
    m_first = NULL;
    m_current = NULL;
    m_next = NULL;
    setParent(NULL);
}

/**
 * \brief Releases all resources and objects related to this TimeLineObject
 *
 **/
void TimeLineObject::Destroy(void)
{
    if (Debug::TIMELINE>0)
        _dbg << toString() << " Destroy() " << std::endl;
    
    TimeLineObject* obj = m_first;
    while(obj != NULL)
    {
        TimeLineObject* obj_to_delete = obj; 
        obj = obj->m_next;
        obj_to_delete->Destroy();
        delete obj_to_delete;
    }
    m_first = NULL;
    m_current = NULL;
    m_next=NULL;
}

/**
 * \brief Adds subobject to this TimeLineObject
 *
 * \param TimeLineObject* object - subobject to add
 * \param  int where_to_add - defines wher to add on the list (by default objects are added at the end of list)
 * \return bool - true if subject added successfuly, false otherwise
 **/
bool TimeLineObject::AddSubObject(TimeLineObject* object, int where_to_add)
{
    bool result = false;
    if (m_first== NULL)
    {
        m_first = object;
        m_first->m_next = NULL;
        result = true;
    }
    else
    {
        if (where_to_add == ADD_OBJECT_AS_LAST)
        {
            GetLastSubObject()->m_next = object;
            object->m_next = NULL;
            result = true;
        } 
        else if (where_to_add == ADD_OBJECT_AS_NEXT) 
        {
          if (m_current != NULL)
          {
              object->m_next = m_current->m_next;
              m_current->m_next = object;
              result = true;
          }
        }
    }

    if (result)
        object->setParent(this);

    return result;
}

/**
 * \brief Return last object on the subobjects list
 * \return TimeLineObject* - last object on the subobjects list
 **/
TimeLineObject* TimeLineObject::GetLastSubObject()
{
    TimeLineObject* obj = m_first;
    if (obj !=NULL)
    {
        while(obj->m_next != NULL)
            obj = obj->m_next;
    }
    return obj;
}

/**
 * \brief Removes object from subobjects list
 * \param TimeLineObject* obj_to_delete - object to delete
 * \param bool deleteFromMemory - indicates if object should be deleted from memory after removing from subobjects list
 **/
void TimeLineObject::RemoveSubObject(TimeLineObject* obj_to_delete, bool deleteFromMemory)
{
    if (obj_to_delete == m_first)
    {
        m_first = obj_to_delete->m_next;
    }
    else
    {
        TimeLineObject* prevObj = m_first;

        while(prevObj!=NULL && prevObj->m_next!=obj_to_delete )
        {
            prevObj = prevObj->m_next;
        }

        if (prevObj!=NULL)
        {
            prevObj->m_next = obj_to_delete->m_next;
        }
    }

    if (deleteFromMemory)
    {
        obj_to_delete->Destroy();
        delete obj_to_delete;
    }
}

/**
 * \brief Removes all objects from subobjects list
 **/
//void TimeLineObject::ClearSubObjects()
//{
//    TimeLineObject* obj = m_first;
//    TimeLineObject* obj_to_delete;
//    while(obj != NULL)
//    {
//        obj_to_delete = obj;
//        obj = obj->m_next;
//        obj_to_delete->Destroy();
//        delete obj_to_delete;
//    }
//    m_first = NULL;
//}

/// \brief Dumps subobjects list information
void TimeLineObject::DumpSubObjects(int depth)
{
    TimeLineObject* obj = m_first;
    while(obj != NULL)
    {
        obj->Dump(depth+1);
        obj = obj->m_next;
    }
}

/// \brief Resets all sub-objects 
void TimeLineObject::Reset(TimeLineContext* timeLineContext)
{
    TimeLineObject* obj = m_first;
    while(obj != NULL)
    {
        obj->Reset(timeLineContext);
        obj = obj->m_next;
    }
    //here is place to reset this object
}

/**
 * \brief Returns indent for dumping debug information on output console
 *
 * \param int depth - indent length
 * \return std::string - string that should be added before debug information for each line describing this TimeLineObject
 **/
std::string TimeLineObject::getDepthStr(int depth)
{
    std::string sep = "";
    for (int i=0; i<depth; i++)
        sep += "- ";
    return sep;
}

/**
 * \brief Prints debug information describing this TimeLineObject on output console
 *
 * \param int depth - indent on output console
 **/
void TimeLineObject::Dump(int depth)
{

    _dbg << getDepthStr(depth) << toString() << std::endl;
    DumpSubObjects(depth);
}

