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
#include "scene/transform.h"


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
	 * Class Motion: is main object represnting motion (animation) of avatar. 
     * It is constructed when avatar is created and 
     * is not changed during lifecycle of avatar.
     * 
	 **/
    class Motion
    {
    public:
        
        Motion();
        Motion(const std::string animName, int anim_id);
        virtual ~Motion(void) { /* empty */}

        void setAnimName(const std::string animName) { m_animName = animName; }
        std::string getAnimName() { return m_animName; }

        void setAnimID(int anim_id) { m_anim_id = anim_id; }
        int getAnimID() { return m_anim_id; }

        void setTransform(Transform* transform) { m_transform = transform; }
        Transform* getTransform() { return m_transform; }

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
        std::string m_animName;
        int m_anim_id;

        Transform* m_transform;

        float m_minSpeedFactor;
        float m_maxSpeedFactor;
    };
}

#endif //_GEN_MOTION_H
