/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_AVATAR_H
#define _GEN_CONTROL_AVATAR_H

#include "../motion/movableavatar.h"
#include "idlestate.h"
#include "walkstate.h"
#include "runstate.h"

namespace ft
{
	/**
	 * Class ControlAvatar: this class is instantiated by avatars that contain and perform motions (animations)
     * by avatar
     **/
    class ControlAvatar : public MovableAvatar                                 
    {
    public:
        ControlAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);
        virtual ~ControlAvatar(void);
        virtual void Destroy(void);

        void Init();

        void OnMessage(Message* msg);
    
        void SetState(ControlState *newState);
        ControlState* getState() { return m_state; }

        void CreateStates();
        void DestroyStates();
        bool IsTransitionAllowed(ControlState* oldState, ControlState* newState);

        virtual void Reset();

    protected:
        IdleState *IDLE_STATE;
        WalkState *WALK_STATE;
        RunState *RUN_STATE;

    private:
        ControlState* m_state;
        
    };
};

#endif //__GEN_CONTROL_AVATAR_H