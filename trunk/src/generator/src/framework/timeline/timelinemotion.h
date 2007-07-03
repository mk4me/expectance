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
        void ExecuteTracks(float elapsedSeconds, Avatar* avatar);
        bool ExecuteSubMotions(float elapsedSeconds, Avatar* avatar);
        void ExecuteAnim(float elapsedSeconds,Avatar* avatar);
        void ExecuteModifiers(float elapsedSeconds, Avatar* avatar);
     

        void setLoopNumber(int number) { m_loopNumber = number; }
        int getLoopNumber() { return m_loopNumber; } 

        void setInterupting(bool set) { m_interrupting = set; }
        bool isInterupting() { return m_interrupting; }

        void setToFinish(bool set) { m_isToFinish = set; }
        bool isToFinish() { return m_isToFinish; }

        void setAnimStarted(bool set) { m_animStarted = set; }
        bool isAnimStarted() { return m_animStarted; }


        virtual void Reset(); // resets current object and its children

        void Start();
        void Stop();


        void Dump(int depth);   //OVERRIDEN
        std::string toString(); //OVERRIDEN

    private:
        Motion* m_motionRef; //reference to motion which is represented by this object
        TimeLineBlender* m_blender;

        bool m_animLoop;  // if the motion is looped
        int m_loopNumber; // defines length of looped motion by number of cycles
        int m_currLoop;

        bool m_animStarted;  // indicates if anim for this motion is already perfomed
        float m_animTime;

        bool m_interrupting;  // indicates if this motion should immediately interuppt the previous motion
        bool m_isToFinish;  //marker if this motion should be finished as soobn as possible

        int m_currSubMotion; // current submotion to execute - if not any submotion to execute it shoild be -1


        std::vector<TimeLineMotion*> m_vTracks;
        std::vector<TimeLineModifier*> m_vModifiers;

        void ResetParams();
        
    };
};

#endif //_GEN_TIMELINE_MOTION_H