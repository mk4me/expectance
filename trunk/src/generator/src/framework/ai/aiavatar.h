/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_AVATAR_H
#define _GEN_AI_AVATAR_H

#include "../action/actionavatar.h"
#include "goal.h"
#include "aicontroller.h"

namespace ft
{
	/**
	 * Class AIAvatar: this class represents AI part in avatar stack 
     * 
     **/
    class AIAvatar : public ActionAvatar                                 
    {
    public:
        AIAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
        virtual ~AIAvatar(void);
        virtual void Destroy(void);
        void Init();
        virtual void Reset();

        void setCurrGoal(Goal* goal) { m_currGoal = goal; }
        Goal* getCurrGoal() { return m_currGoal; }

        void setGoalController(AIController* controller) { m_currGoalController = controller; }
        AIController* getGoalController() { return m_currGoalController; }

        void SetThink(boolean set);
        bool isThink() { return m_bThink; }

    private:
        Goal* m_currGoal;
        AIController* m_currGoalController;

        bool m_bThink;
    };
}

#endif //_GEN_AI_AVATAR_H