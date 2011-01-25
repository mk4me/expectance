/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: abak
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
#include "../timeline/footdetector.h"
#include "../timeline/stopcontroller.h"

#include "osg/PositionAttitudeTransform"
#include "osg/Vec3d"
#include "osg/Quat"
#include "osgavatartype.h"

namespace ft
{
	/* An OsgAvatar class 
	 * Base class for all humanoid characters
     */
	class OsgAvatar : public gil::GIL_Avatar
    {
    public:
	OsgAvatar(osgCal::Model* osgModel, OsgAvatarType* type,  const std::string& name);
	virtual ~OsgAvatar(void);
        virtual void Destroy(void);

	virtual const std::string getType(); 
	OsgAvatarType* getAvatarType() { return m_avatarType; }
	gil::GIL_Animation* getAnimation(const std::string& animName);

        //MsgSender* getLocalMsgSender() { return m_localMsgSender; }
		void setName(const std::string& avatarName) {m_name = avatarName; m_offsetTransform->setName(avatarName);}
		const std::string& getName() { return m_name; }

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

		osg::PositionAttitudeTransform* getOffsetTransform() { return m_offsetTransform; }
		void setPosition(const osg::Vec3d &pos) { m_offsetTransform->setPosition(pos);  }
		const osg::Vec3d& getPosition() { return m_offsetTransform->getPosition();  }

		void setGlobalRotation(const osg::Quat &rotation) { m_offsetTransform->setAttitude(rotation);  }
		const osg::Quat& getGlobalRotation() { return m_offsetTransform->getAttitude();  }


		//void OnMessage(Message* msg);

		void setFootDetector(FootDetector* detector) { m_footDetector = detector; }
        FootDetector* getFootDetector() { return m_footDetector; }
		void setStopController(StopController* cntrl) {m_stopController = cntrl; }
		StopController* getStopController() { return m_stopController; }


    private:
		osg::PositionAttitudeTransform* m_offsetTransform;
		osgCal::Model* m_osgModel;
		OsgAvatarType* m_avatarType;
        //MsgSender* m_localMsgSender;

        float m_currSpeedFactor;
        float m_destSpeedFactor;
        float m_speedfactorMin;
        float m_speedfactorMax;

		std::string m_name;

		void InitSpeedFactor();
		void InitFootDetector();

        FootDetector* m_footDetector; //TODO: only temporarily here
		StopController* m_stopController; //TODO: only temporarily here 28-08-200
		//bool m_turnLeft, m_turnRight, FOOT_DETECTOR;
    };
}

#endif //_OSGAVATAR_H
