/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_H
#define _GEN_TIMELINE_H


#include "timelinemotion.h"

#include "../motion/motion.h"

namespace ft
{
	/**
	 * Class TimeLine: represents task to execute by avatar in sense of motion or sequence of motions. 
     * It is specific time of TimeLineMotion and can be trated in the same way as TimeLineMotion.
	 **/
    class TimeLine : public TimeLineMotion
    {
    public:
        TimeLine(void) {     m_started = false; }
        virtual ~TimeLine(void) { /* empty */}

        void Destroy(void);

        void setStarted(bool set) { m_started = set; }
        bool isStarted() { return m_started; }

        bool AddModifier(TimeLineModifier* modifier);
        bool RemoveModifier(TimeLineModifier* modifier);
//        TimeLineModifier* GetModifier(int);

        virtual bool AddSubObject(TimeLineObject* object, int where_to_add = ADD_OBJECT_AS_LAST);
        virtual void RemoveSubObject(TimeLineObject* obj_to_delete, bool deleteFromMemory = false);

        virtual void Reset(TimeLineContext* timeLineContext); // resets current object and its children

        void Dump();

        std::vector<TimeLineModifier*> m_vModifiers;
    private:
        bool m_started;
    };
}


#endif //_GEN_TIMELINE_H
