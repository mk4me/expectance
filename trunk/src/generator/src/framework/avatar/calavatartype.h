/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AVATARTYPE_H
#define _GEN_AVATARTYPE_H

#include "scene/cal3dtype.h"
#include "../motion/animation.h"


namespace ft
{
	/* 
     * An CalAvatarType class is extension of Cal3dType for CalAvatars
	 */
    class CalAvatarType : public Cal3dType
    {
    public:
		CalAvatarType(const std::string &typeName);
	    virtual ~CalAvatarType(void);

        bool AddMotion(Animation* motion);
        bool  RemoveMotion(Animation* motion);
        Animation* GetMotion(std::string motionName);
        std::map<std::string,Animation*>& getMotionsMap() { return m_motions; }
        void InitMotions();

        virtual void InitTransform(bool source_3dsmax);

        virtual void Dump();
    
    private:
        std::map<std::string,Animation*> m_motions;
       
    };
}

#endif //_GEN_AVATARTYPE_H
