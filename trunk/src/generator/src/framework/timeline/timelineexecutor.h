/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINEEXECUTOR_H
#define _GEN_TIMELINEEXECUTOR_H

#include "timeline.h"

namespace ft
{
    struct timeline_exec_item 
    {
        TimeLineMotion* motion;
        CalAnimation* anim;  // reference to animation in Cal3d
        float animTime;  // time of animation in Cal3d
        float animDuration; // duration of animation in Cal3d

        float lastTime; // for cycle countin for cyclic animation
        bool  lastStep;
        int   loopStep;
    };
    
    typedef timeline_exec_item timeline_exec_item_t;


	/**
	 * Class TimeLine: responsible for timeline execution 
	 **/
    class TimeLineExecutor
    {
    public:

        bool LOCAL_DEBUG;

        TimeLineExecutor(void);
        virtual ~TimeLineExecutor(void) { /* empty */}

        void Destroy(void);

        void setState(int state); 
        int getState(); 

        void setTimeLine(TimeLine* tl) { m_timeLine = tl; }
        TimeLine* getTimeLine() { return m_timeLine; }

        void setCtx(TimeLineContext* tlCtx) { m_timeLineContext = tlCtx; }
        TimeLineContext* getCtx() { return m_timeLineContext; }



        void Reset();

        void Initiate(TimeLine* timeLine, TimeLineContext* timeLineContext);

        void Start();
        void StopRequest();
        void RemoveUnexecutedMotions();

        void ChangeState(int newState);

        void UpdateMotions(const double elapsedSeconds);
        void UpdateModifiers(const double elapsedSeconds);

        void setTerminated(bool set) { m_isTerminated = set; }
        bool isTerminated() { return m_isTerminated; }


        void Dump();
        static std::string _GET_STATE_NAME(int state);

    private:
        int m_state;
        TimeLine* m_timeLine;
        TimeLineContext* m_timeLineContext;

        timeline_exec_item_t m_prevMotion;  //only informative
        timeline_exec_item_t m_currMotion;
        timeline_exec_item_t m_nextMotion;

//        TimeLineBlender * m_prevBlender;  //lastly used blender
//        TimeLineBlender * m_currBlender;  // blender that will be used afte current motion
//        TimeLineBlender * m_nextBlender;  // blender that will be used after next motion
        float m_prevBlender;  //lastly used blender
        float m_currBlender;  // blender that will be used afte current motion
        float m_nextBlender;  // blender that will be used after next motion


        int m_lastEvent;
        bool m_animChanged;
        bool m_animNewCycle;

        bool m_isTerminated;

        void RemoveExecutedMotions();
        void RemoveMotionsAfterCurrent();
        int UpdateAnimations(float elapsedSeconds);
        void UpdateExecItem(timeline_exec_item_t &item);
        void CheckInterrupting();
        void IdentifyNextMotion();
        void IdentifyBlenders();
        void LimitCurrentBlender();
        void UpdateContext();

        TimeLineMotion* GetSubMotionWithAnim(TimeLineMotion* motion);
        TimeLineMotion* GetParentWithNext(TimeLineMotion* motion);
        TimeLineMotion* GetParentWithBlender(TimeLineMotion* motion);
        TimeLineMotion* GetTopParentInHierarchy(TimeLineMotion* motion);
        bool IsAnimStillActive(CalAnimation* anim);

        void StartNextMotion();
        void StopLoopAnim();
        void StopActionAnim();
        void ExchangeExecItems();
        void CheckLoopAnimStop(timeline_exec_item_t &item);
        void MarkAnimStopped(timeline_exec_item_t &item);
        CalAnimation* FindAddedAnimInCal3d(int animType);

        void UpdateModifiersForMotion(TimeLineMotion* motion, float elapsedSeconds);
        bool IsParent(TimeLineObject* motion, TimeLineObject* parent);

        bool CheckEpmtyFrame();

        void EntryState(int state, int oldState);
        void ExitState(int state);
        void UpdateState(int state);

        void EntryWaitState(int oldState);
        void EntrySingleState(int oldState);
        void EntryOverlapState(int oldState);
        void EntryFadeInState(int oldState);
        void EntryFadeOutState(int oldState);

        void UpdateWaitState();
        void UpdateSingleState();
        void UpdateOverlapState();
        void UpdateFadeInState();
        void UpdateFadeOutState();
    };
};

#endif //_GEN_TIMELINEEXECUTOR_H