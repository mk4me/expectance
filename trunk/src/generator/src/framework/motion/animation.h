/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MOTION_H
#define _GEN_MOTION_H

#include <string>
#include "core/global.h"
#include "core/config.h"
#include "utility/stringhelper.h"

#include "gil/gil_animation.h"
#include "motiondata.h"


namespace ft
{
    //NAMES of MOTIONS
    static const std::string MOTION_WALK_START = "walkstart.caf";  
    static const std::string MOTION_WALK_LOOP = "walkloop.caf";  
    static const std::string MOTION_WALK_STOP = "walkstop.caf";  
    static const std::string MOTION_WALK_IDLE = "idle.caf";  
    static const std::string MOTION_WALK_TURN_LEFT = "walturn_left.caf";
    static const std::string MOTION_WALK_TURN_RIGHT = "walturn_right.caf";
    static const std::string MOTION_RUN_START = "runstart.caf";  
    static const std::string MOTION_RUN_LOOP  = "runloop.caf";  
    static const std::string MOTION_RUN_STOP  = "runstop.caf";  
    static const std::string MOTION_DANCE     = "ballerina.caf";  

    /**
	 * Class Animation: is main object represnting motion (animation) of avatar. 
     * It is constructed when avatar is created and 
     * is not changed during lifecycle of avatar.
     * 
	 **/
	class Animation: public gil::GIL_Animation
    {
    public:
        
        Animation(CalCoreAnimation* anim);
        Animation(CalCoreAnimation* anim, const std::string animName, int anim_id);
        virtual ~Animation(void) { /* empty */}

		float getDuration(); 
		float getTime() { return 0; }

		bool play(bool cyclic) { return false; }
		bool stop() { return false; }

		bool isFeature(const std::string&) { return false; }

		virtual gil::GIL_AnimData* getAnimData() { return getMotionData(); }


        void setAnimName(const std::string animName) { m_animName = animName; }
        std::string getAnimName() { return m_animName; }

        void setAnimID(int anim_id) { m_anim_id = anim_id; }
        int getAnimID() { return m_anim_id; }

        void setMotionData(MotionData* data) { m_motionData = data; }
        MotionData* getMotionData() { return m_motionData; }

        void setMinSpeedfactor(float minFactor) { m_minSpeedFactor = minFactor; }
        float getMinSpeedfactor() { return m_minSpeedFactor; } 

        void setMaxSpeedfactor(float maxFactor) { m_maxSpeedFactor = maxFactor; }
        float getMaxSpeedfactor() { return m_maxSpeedFactor; } 

	    //! init foot limits from configuration for given animName
	    bool initFootLimits(const std::string animName); 
        void InitSpeedFactorLimits(const std::string animName);

        bool isNullAnim() { return (m_anim_id == -1);}

	float footLimits[4];

    private:
		CalCoreAnimation* m_coreAnim;
        std::string m_animName;
        int m_anim_id;

		MotionData* m_motionData;

        float m_minSpeedFactor;
        float m_maxSpeedFactor;
    };
}

#endif //_GEN_MOTION_H
