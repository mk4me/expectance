/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MOVABLE_AVATAR_H
#define _GEN_MOVABLE_AVATAR_H

#include "cal3d/cal3d.h"
#include "../avatar/avatar.h"
#include "../utility/Quat.h"
#include "../timeline/timelinefactory.h"
#include "../scene/visualizationmanager.h"

#include "motion.h"
#include <map>

namespace ft
{
    /**
	 * Class MovableAvatar: this class is instantiated by avatars that contain and perform motions (animations)
	 **/
    class MovableAvatar : public Avatar
    {
    public:
        bool TRACE;

		MovableAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName); 
	    virtual ~MovableAvatar(void);
        virtual void Destroy(void);

        virtual void Init();

        virtual void OnUpdate(float elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 
        void OnMessage(Message* msg);  // OVERRIDEN, receives a message from UpdateManager

        bool AddMotion(Motion* motion);
        bool  RemoveMotion(Motion* motion);
        Motion* GetMotion(std::string motionName);


        //////////////////
        // TimeLine

        bool setTimeLine(TimeLine* timeLine);
        TimeLine* getTimeLine();

        void setTimeLineContext(TimeLineContext* timeLineContext) { m_timeLineContext = timeLineContext; }
        TimeLineContext* getTimeLineContext() { return m_timeLineContext; }


        void StartTimeLine();
        void StopTimeLine();
        bool AddTimeLineMotion(TimeLineMotion *timeLineMotion);

        virtual void Reset();

        ////////////////////////////

        void Dump();


    private:
        //// ANIMATIONS
        int m_currentAnimationId;
        float m_leftAnimationTime;
        float m_blendTime;
        bool m_bPaused;

        std::map<std::string,Motion*> m_motions;

        TimeLine* m_timeLine;
        TimeLineContext* m_timeLineContext;
        
        void InitMotions();

        void UpdateTimeLine(float elapsedSeconds);

        TimeLine* CreateTestTimeLine();

        TraceLine *tracer_start_pos;

    };
};

#endif //_GEN_MOVABLE_AVATAR_H