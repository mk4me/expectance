/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_ACTION_AVATAR_H
#define _GEN_ACTION_AVATAR_H

#include "../physics/physicsavatar.h"
#include "idleaction.h"
#include "walkaction.h"
#include "runaction.h"

namespace ft
{
	/**
	 * Class ActionAvatar: this class is instantiated by avatar to define and performs actions (motions)
     * 
     **/
    class ActionAvatar : public PhysicsAvatar                                 
    {
    public:
        ActionAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);
        virtual ~ActionAvatar(void);
        virtual void Destroy(void);

        void Init();

        void OnMessage(Message* msg);
    
        Action* getCurrAction() { return m_action; }

        Action* SetActionById(int id);
        Action* getActionById(int id);

        void CreateActions();
        void DestroyActions();
        bool IsTransitionAllowed(Action* oldAction, Action* newAction);

        virtual void Reset();

        void ActionAvatar::Dump();

    protected:
        IdleAction *ACTION_IDLE;
        WalkAction *ACTION_WALK;
        RunAction *ACTION_RUN;

    private:
        void SetCurrAction(Action *newAction);

        Action* m_action;
        
    };
};

#endif //_GEN_ACTION_AVATAR_H