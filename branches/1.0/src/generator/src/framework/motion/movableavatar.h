/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MOVABLE_AVATAR_H
#define _GEN_MOVABLE_AVATAR_H

//#include "cal3d/src/cal3d.h"
#include "../avatar/avatar.h"
#include "utility/quat.h"
#include "../timeline/timelineexecutor.h"
#include "../timeline/lcsmodifier.h"
#include "../timeline/footdetector.h"


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

		MovableAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName); 
	    virtual ~MovableAvatar(void);
        virtual void Destroy(void);

        virtual void Init();

        virtual void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 
        void OnMessage(Message* msg);  // OVERRIDEN, receives a message from UpdateManager

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

        virtual void Dump();

        void setLCSModifier(LCSModifier* modifier) { m_lcsModifier = modifier; }
        LCSModifier* getLCSModifier() { return m_lcsModifier; }

		void setFootDetector(FootDetector* detector) { m_footDetector = detector; } //TMMP
        FootDetector* getFootDetector() { return m_footDetector; }

        void setTLExecutor(TimeLineExecutor* executor) { m_tlExecutor = executor; }
        TimeLineExecutor* getTLExecutor() { return m_tlExecutor; }

        void setCurrSpeedFactor(float factor) { m_currSpeedFactor = factor; }
        float getCurrSpeedFactor() { return m_currSpeedFactor; }

        void setDestSpeedFactor(float factor) { m_destSpeedFactor = factor; }
        float getDestSpeedFactor() { return m_destSpeedFactor; }

        void setSpeedFactorMin(float factorMin) { m_speedfactorMin = factorMin; }
        float getSpeedFactorMin() { return m_speedfactorMin; }
        void setSpeedFactorMax(float factorMax) { m_speedfactorMax = factorMax; }
        float getSpeedFactorMax() { return m_speedfactorMax; }


    protected:
        virtual TimeLine* InitTimeLine();

    private:
        //// ANIMATIONS
        int m_currentAnimationId;
        float m_leftAnimationTime;
        float m_blendTime;
        bool m_bPaused;

        float m_currSpeedFactor;
        float m_destSpeedFactor;
        float m_speedfactorMin;
        float m_speedfactorMax;

        TimeLine* m_timeLine;
        TimeLineContext* m_timeLineContext;
        TimeLineExecutor* m_tlExecutor;
        
        void InitFootDetector();

        void UpdateTimeLine(float elapsedSeconds);

        TraceLine *tracer_start_pos;

        LCSModifier* m_lcsModifier; //TODO: only temporarily here
        FootDetector* m_footDetector; //TODO: only temporarily here
    };
}

#endif //_GEN_MOVABLE_AVATAR_H
