/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MOTION_H
#define _GEN_TIMELINE_MOTION_H

#include "timelinemodifier.h"
#include "timelineblender.h"
#include "../motion/motion.h"
#include "../avatar/avatar.h"
#include <vector>

namespace ft
{
    class TimeLineMotion : public TimeLineObject
    {
    public:
        TimeLineMotion(void);
        virtual ~TimeLineMotion(void) { /* empty */ }
        virtual void Destroy(void);

        void setMotion(Motion *motion) { m_motionRef = motion; }
        Motion * getMotion() { return m_motionRef; } 

        void setBlender(TimeLineBlender* blender) { m_blender = blender; } 
        TimeLineBlender* getBlender() { return m_blender; }

        bool AddModifier(TimeLineModifier* modifier);
//        bool  RemoveMdofier(TimeLineModifier* modfier);
//        TimeLineModifier* GetModifier(int);

        bool AddTrack(TimeLineMotion* trackMotion);
//        bool  RemoveTrack(TimeLineMotion* trackMotion);
//        TimeLineMotion* GetTrack(int);

        void setAnimLoop(bool set) { m_animLoop = set; }
        bool isAnimLoop() { return m_animLoop; }

        virtual void Execute(float elapsedSeconds, Avatar* avatar);
        void ExecuteAnim(float elapsedSeconds,Avatar* avatar);
        

        void Dump(int depth);   //OVERRIDEN
        std::string toString(); //OVERRIDEN

    private:
        Motion* m_motionRef; //reference to motion which is represented by this object
        TimeLineBlender* m_blender;
        bool m_animLoop;

        std::vector<TimeLineMotion*> m_vTracks;
        std::vector<TimeLineModifier*> m_vModifiers;
        
    };
};

#endif //_GEN_TIMELINE_MOTION_H