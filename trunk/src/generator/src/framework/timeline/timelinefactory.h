/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_FACTORY_H
#define _GEN_TIMELINE_FACTORY_H

#include "timeline.h"
#include "timelinemotion.h"
#include "../motion/motion.h"

namespace ft
{
	/**
	 * Class TimeLineFactory: responsible for creating objetcs on TimeLine
	 **/
    class TimeLineFactory
    {
    public:
        TimeLineFactory() { /* empty */ } 
        virtual ~TimeLineFactory(void) { /* empty */ }
        static TimeLineFactory* getInstance();
        static void DestroyInstance();

        TimeLine* CreateTimeLine(Motion *motion);
    private:
        static TimeLineFactory* m_instance;
    };
};


#endif //_GEN_TIMELINE_FACTORY_H