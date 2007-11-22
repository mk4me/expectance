/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_OBJECT_H
#define _GEN_TIMELINE_OBJECT_H

#include "../core/baseobject.h"
#include "timelinecontext.h"
#include <string>
#include <vector>

namespace ft
{
    static const int TIME_UNDEFINED = -1;

    static const int ADD_OBJECT_AS_NEXT = 0;
    static const int ADD_OBJECT_AS_LAST = 1;

	/**
	 * Class TimeLineObject: base object for all objects that can be part of TimeLine
	 **/
    class TimeLineObject : public BaseObject
    {
    public:
        TimeLineObject();
        virtual ~TimeLineObject(void) { /* empty */ }
        virtual void Destroy(void);

        bool AddSubObject(TimeLineObject* object, int where_to_add = ADD_OBJECT_AS_LAST);
        void RemoveSubObject(TimeLineObject* obj_to_delete, bool deleteFromMemory = false);
        TimeLineObject* GetCurrSubObject();
        TimeLineObject* GetLastSubObject();
        bool isEmpty() { return (m_first == NULL); }
        void ClearSubObjects();
        void DumpSubObjects(int depth);

        void setStarted(bool set) { m_started = set; }
        bool isStarted() { return m_started; }

        TimeLineObject* getCurrentObject() { return m_current; }
        void setCurrentObject(TimeLineObject* obj) { m_current = obj; }

        TimeLineObject* getNextObject() { return m_next; }
        void setNextObject(TimeLineObject* obj) { m_next = obj; }

        TimeLineObject* getFirstObject() { return m_first; }
        void setFirstObject(TimeLineObject* obj) { m_first = obj; }

        TimeLineObject* getParent() { return m_parent; }
        void setParent(TimeLineObject* obj) { m_parent = obj; }

        virtual void Reset(TimeLineContext* timeLineContext); // resets current object and its children


        std::string getDepthStr(int depth);
        virtual void Dump(int depth);

    protected:
        bool m_started;

        //list
        TimeLineObject* m_first;
        TimeLineObject* m_current;

        TimeLineObject* m_next;

        TimeLineObject* m_parent;
    };
};


#endif //_GEN_TIMELINE_OBJECT_H