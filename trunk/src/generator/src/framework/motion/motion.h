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
    static const std::string MOTION_WALK_LOOP = "cally_walkloop.CAF";  

    class Motion
    {
    public:
        Motion(const std::string animName, int anim_id) { setAnimName(animName); setAnimID(anim_id); } 
        virtual ~Motion(void) { /* empty */}

        void setAnimName(const std::string animName) { m_animName = animName; }
        std::string getAnimName() { return m_animName; }

        void setAnimID(int anim_id) { m_anim_id = anim_id; }
        int getAnimID() { return m_anim_id; }

    private:
        std::string m_animName;
        int m_anim_id;
    };
};

#endif //_GEN_MOTION_H