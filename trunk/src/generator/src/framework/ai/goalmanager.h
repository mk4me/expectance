/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_GOALMANAGER_H
#define _GEN_AI_GOALMANAGER_H

#include "goal.h"
#include "aiavatar.h"
#include <map>
#include <vector>

namespace ft
{
	/**
	 * Class GoalManager: responsible for selecting and assigning goals for avatars
     * 
     **/
    class GoalManager
    {
    public:
        GoalManager();
        virtual ~GoalManager(void);
        static GoalManager* getInstance();
        static void DestroyInstance();

        void Init();

        bool AddGoal(Goal* pGoal);
        bool  RemoveGoal(Goal* pGoal);

        void UpdateAvatarGoal(AIAvatar *av);

        void Dump();

    private:
        void FindPossibleGoals(std::vector<Goal*> &vPossibleGoals, AIAvatar *av);
        void EsimateGoalsValue(std::vector<Goal*> &vPossibleGoals, AIAvatar *av);
        Goal* SelectGoal(std::vector<Goal*> &vPossibleGoals, AIAvatar *av);
        void ExecuteGoal(Goal* goal, AIAvatar *av);

        static GoalManager* m_instance;

        std::map<std::string,Goal*> m_goals;
    };
};

#endif //_GEN_AI_GOALMANAGER_H