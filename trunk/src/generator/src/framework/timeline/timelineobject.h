/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_OBJECT_H
#define _GEN_TIMELINE_OBJECT_H

#include "../core/baseobject.h"
#include <string>
#include <vector>

namespace ft
{
    class TimeLineObject : BaseObject
    {
    public:
        TimeLineObject() { CLASS_NAME = "TimeLineObject"; } 
        virtual ~TimeLineObject(void) { /* empty */ }
        void Destroy(void);

        bool AddObject(TimeLineObject* object);
//        bool  RemoveObject(TimeLineObject* object);
//        TimeLineObject* GetObject(int);
        std::string getDepthStr(int depth);
        virtual void Dump(int depth);
        virtual std::string toString(); 
 
    protected:
        std::string CLASS_NAME;

        std::vector<TimeLineObject*> m_vObjects;
    };
};


#endif //_GEN_TIMELINE_OBJECT_H