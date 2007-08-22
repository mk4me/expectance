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

	/**
	 * Class TimeLineObject: base object for all objects that can be part of TimeLine
	 **/
    class TimeLineObject : public BaseObject
    {
    public:
        TimeLineObject();
        virtual ~TimeLineObject(void) { /* empty */ }
        virtual void Destroy(void);

        bool AddObject(TimeLineObject* object);
//        bool  RemoveObject(TimeLineObject* object);
//        TimeLineObject* GetObject(int);
        
        void setStartTime(float startTime) { m_startTime = startTime; }
        float getStartTime() { return m_startTime; }

        void setEndTime(float endTime) { m_endTime = endTime; }
        float getEndTime() { return m_endTime; }

        void setStarted(bool set) { m_started = set; }
        bool isStarted() { return m_started; }

        TimeLineObject* GetCurrentObject();

        virtual void Reset(TimeLineContext* timeLineContext); // resets current object and its children


        std::string getDepthStr(int depth);
        virtual void Dump(int depth);
 
    protected:
        float m_startTime;
        float m_endTime;

        bool m_started;

        std::vector<TimeLineObject*> m_vObjects;

        void PrintDebug(const std::string& text);
    };
};


#endif //_GEN_TIMELINE_OBJECT_H