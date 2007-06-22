/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MOVABLE_AVATAR_H
#define _GEN_MOVABLE_AVATAR_H

#include "cal3d/cal3d.h"
#include "../avatar/avatar.h"
#include "../utility/Quat.h"

#include "motion.h"
#include <map>

namespace ft
{
    class MovableAvatar : public Avatar
    {
    public:
		MovableAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName); 
	    virtual ~MovableAvatar(void);

        void Init();

        virtual void OnUpdate(float elapsedSeconds);  // OVERRIDEN, updates by ControlManager 
        void OnMessage(Message* msg);  // OVERRIDEN, receives a message from ControlManager

        bool AddMotion(Motion* motion);
        bool  RemoveMotion(Motion* motion);
        Motion* GetMotion(std::string motionName);
        void Dump();


    private:
        //// ANIMATIONS
        int m_currentAnimationId;
        float m_leftAnimationTime;
        float m_blendTime;
        bool m_bPaused;

        CalVector m_vTranslation;
        CalVector m_vLastPos;

        Quat m_vRotation;

        std::map<std::string,Motion*> m_motions;

        void InitMotions();
    };
};

#endif //_GEN_MOVABLE_AVATAR_H