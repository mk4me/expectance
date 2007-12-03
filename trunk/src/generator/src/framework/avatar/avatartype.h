/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AVATARTYPE_H
#define _GEN_AVATARTYPE_H

#include "scene/cal3dType.h"
#include "../motion/motion.h"


namespace ft
{
	/* 
     * An AvatarType class is extension of Cal3dType for Avatars
	 */
    class AvatarType : public Cal3dType
    {
    public:
		AvatarType(const std::string &typeName);
	    virtual ~AvatarType(void);

        bool AddMotion(Motion* motion);
        bool  RemoveMotion(Motion* motion);
        Motion* GetMotion(std::string motionName);
        std::map<std::string,Motion*>& getMotionsMap() { return m_motions; }
        void InitMotions();

        virtual void InitTransform(bool source_3dsmax);

        virtual void Dump();
    
    private:
        std::map<std::string,Motion*> m_motions;
       
    };
};

#endif //_GEN_AVATARTYPE_H
