/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _OSGAVATAR_H
#define _OSGAVATAR_H

//#include "scene/cal3dobject.h"
//#include "core/msgsender.h"
#include "osgCal/Model"
#include "gil/gil_avatar.h"
#include "gil/gil_animation.h"
#include "cal3d/cal3d.h"
#include "../evolution_impl/cal3danimexecution.h"
//#include "../timeline/footdetector.h"

namespace ft
{
	/* An OsgAvatar class 
	 * Base class for all humanoid characters
     */
	class OsgAvatar : public gil::GIL_Avatar
    {
    public:
		OsgAvatar(osgCal::Model* osgModel, const std::string name);
	    virtual ~OsgAvatar(void);
        virtual void Destroy(void);

		virtual const std::string getType(); 
		gil::GIL_Animation* getAnimation(const std::string& animName);

        //MsgSender* getLocalMsgSender() { return m_localMsgSender; }

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

        //virtual void Dump();

		virtual void Update(const double elapsedSeconds);
		
		CalAnimation* FindAddedAnimInCal3d(int animType);

		virtual osgCal::Model* getOsgModel() { return m_osgModel; }

		//void OnMessage(Message* msg);

		//void setFootDetector(FootDetector* detector) { m_footDetector = detector; }
        //FootDetector* getFootDetector() { return m_footDetector; }


    private:
		osgCal::Model* m_osgModel;
        //MsgSender* m_localMsgSender;

        float m_currSpeedFactor;
        float m_destSpeedFactor;
        float m_speedfactorMin;
        float m_speedfactorMax;

		//void InitSpeedFactor();
		//void InitFootDetector();

        //FootDetector* m_footDetector; //TODO: only temporarily here
		//bool m_turnLeft, m_turnRight, FOOT_DETECTOR;
    };
}

#endif //_OSGAVATAR_H
