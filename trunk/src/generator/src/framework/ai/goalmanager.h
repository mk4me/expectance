/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_GOALMANAGER_H
#define _GEN_AI_GOALMANAGER_H

#include "goal.h"
#include <map>

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

        void Dump();

    private:
        static GoalManager* m_instance;

        std::map<std::string,Goal*> m_goals;
    };
};

#endif //_GEN_AI_GOALMANAGER_H