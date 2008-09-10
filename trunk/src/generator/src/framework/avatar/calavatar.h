/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_AVATAR_H
#define _GEN_AVATAR_H

#include "scene/cal3dobject.h"
#include "core/msgsender.h"
#include "gil/gil_avatar.h"
#include "gil/gil_animation.h"
#include "../evolution_impl/cal3danimexecution.h"
#include "../timeline/footdetector.h"
#include "../timeline/stopcontroller.h"

namespace ft
{
	/* An CalAvatar class 
	 * Base class for all humanoid characters managed by Generator
     * \param CalModel * calModel -- model in Cal3d which is referenced by this avatar
	 */
	class CalAvatar : public Cal3DObject, public gil::GIL_Avatar
    {
    public:
		CalAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
	    virtual ~CalAvatar(void);
        virtual void Destroy(void);

		virtual const std::string getType(); 
		gil::GIL_Animation* getAnimation(const std::string& animName);

        MsgSender* getLocalMsgSender() { return m_localMsgSender; }

        void setCurrSpeedFactor(float factor) { m_currSpeedFactor = factor; }
        float getCurrSpeedFactor() { return m_currSpeedFactor; }

        void setDestSpeedFactor(float factor) { m_destSpeedFactor = factor; }
        float getDestSpeedFactor() { return m_destSpeedFactor; }

        void setSpeedFactorMin(float factorMin) { m_speedfactorMin = factorMin; }
        float getSpeedFactorMin() { return m_speedfactorMin; }
        void setSpeedFactorMax(float factorMax) { m_speedfactorMax = factorMax; }
        float getSpeedFactorMax() { return m_speedfactorMax; }

		virtual gil::GIL_AnimExecution* PlayAnimation(int animID, float fade_in, float fade_out);
		virtual void StopAnimation(int animID, float fade_out);
		
		virtual gil::GIL_AnimExecution* PlayCyclicAnimation(int animID, float fade_in);
		virtual void StopCyclicAnimation(int animID, float fade_out);

		virtual bool isAnimValid(gil::GIL_AnimExecution* animExec);
		float getAnimTime(gil::GIL_AnimExecution* anim_exec);
		float getAnimDuration(gil::GIL_AnimExecution* anim_exec);

		virtual bool isEmptyFrame();

        virtual void Dump();

		virtual void Update(const double elapsedSeconds);
		
		CalAnimation* FindAddedAnimInCal3d(int animType);

		virtual CalModel* getCalModel() { return GetCalModel(); }
		virtual CalCoreModel* getCalCoreModel() { return GetCalCoreModel(); }

		void OnMessage(Message* msg);

		void setFootDetector(FootDetector* detector) { m_footDetector = detector; }
        FootDetector* getFootDetector() { return m_footDetector; }

		void setStopController(StopController* cntrl) {m_stopController = cntrl; }
		StopController* getStopController() { return m_stopController; }

    private:
        MsgSender* m_localMsgSender;

        float m_currSpeedFactor;
        float m_destSpeedFactor;
        float m_speedfactorMin;
        float m_speedfactorMax;

		void InitSpeedFactor();
		void InitFootDetector();

        FootDetector* m_footDetector; //TODO: only temporarily here
		StopController* m_stopController; //TODO: only temporarily here 28-08-2008
		bool m_turnLeft, m_turnRight, FOOT_DETECTOR;
    };
}

#endif //_GEN_AVATAR_H
