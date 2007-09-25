/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MOTION_H
#define _GEN_MOTION_H

#include <string>


namespace ft
{
    //NAMES of MOTIONS
    static const std::string MOTION_WALK_START = "data\\cally\\cally_walkstart.CAF";  
    static const std::string MOTION_WALK_LOOP = "data\\cally\\cally_walkloop.CAF";  
    static const std::string MOTION_WALK_STOP = "data\\cally\\cally_walkstop.CAF";  
    static const std::string MOTION_WALK_IDLE = "data\\cally\\cally_walkidle.CAF";  
    static const std::string MOTION_RUN_START = "data\\cally\\cally_runstart.CAF";  
    static const std::string MOTION_RUN_LOOP = "data\\cally\\cally_runloop.CAF";  
    static const std::string MOTION_RUN_STOP = "data\\cally\\cally_runstop.CAF";  
    static const std::string MOTION_DANCE = "data\\cally\\cally_ballerina.CAF";  

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

       
        bool isNullAnim() { return (m_anim_id == -1);}

    private:
        std::string m_animName;
        int m_anim_id;
    };
};

#endif //_GEN_MOTION_H