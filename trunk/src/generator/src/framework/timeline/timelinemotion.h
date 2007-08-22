/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MOTION_H
#define _GEN_TIMELINE_MOTION_H

#include "timelinemodifier.h"
#include "timelineblender.h"
#include "../motion/motion.h"
#include <vector>

namespace ft
{
	/**
	 * Class TimeLineMotion: object that can be put on TimeLine that represents motion (or set of motions) to execute 
     * by avatar
     **/
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

        virtual void Execute(float elapsedSeconds, TimeLineContext* timeLineContext);
        void ExecuteTracks(float elapsedSeconds, TimeLineContext* timeLineContext);
        bool ExecuteSubMotions(float elapsedSeconds, TimeLineContext* timeLineContext);
        void ExecuteAnim(float elapsedSeconds, TimeLineContext* timeLineContext);
        void ExecuteModifiers(float elapsedSeconds, TimeLineContext* timeLineContext);
     

        void setLoopNumber(int number) { m_loopNumber = number; }
        int getLoopNumber() { return m_loopNumber; } 

        void setInterupting(bool set) { m_interrupting = set; }
        bool isInterupting() { return m_interrupting; }

        void setToFinish(bool set) { m_isToFinish = set; }
        bool isToFinish() { return m_isToFinish; }

        void setAnimStarted(bool set) { m_animStarted = set; }
        bool isAnimStarted() { return m_animStarted; }

        float getAnimTime() { return m_animTime; }

        float GetMotionDuration(TimeLineContext* timeLineContext);


        virtual void Reset(TimeLineContext* timeLineContext); // resets current object and its children

        void Start();
        void Stop();

        void Dump(int depth);   //OVERRIDEN
        std::string toString(); //OVERRIDEN

    private:
        Motion* m_motionRef; //reference to motion which is represented by this object
        TimeLineBlender* m_blender;

        bool m_animLoop;  // if the motion is looped
        int m_loopNumber; // defines length of looped motion by number of cycles (if value is -1 it is infinite)
        int m_currLoop;

        bool m_animStarted;  // indicates if anim for this motion is already perfomed
        float m_animTime;

        bool m_interrupting;  // indicates if this motion should immediately interuppt the previous motion
        bool m_isToFinish;  //marker if this motion should be finished as soobn as possible

        int m_currSubMotion; // current submotion to execute - if not any submotion to execute it shoild be -1


        std::vector<TimeLineMotion*> m_vTracks;
        std::vector<TimeLineModifier*> m_vModifiers;

        void ResetParams();
        bool IsBlendingToStart(TimeLineMotion* currMotion, TimeLineMotion* nextMotion, TimeLineContext* timeLineContext);
        
    };
};

#endif //_GEN_TIMELINE_MOTION_H