/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_AIMANAGER_H
#define _GEN_AI_AIMANAGER_H

#include "aiavatar.h"
#include "core/updatemanager.h"


namespace ft
{
	/**
	 * Class AIManager: responsible for management of AI part
	 **/
    class AIManager : public UpdateObject
    {
    public:

        bool TRACE;

        AIManager(void);
	    virtual ~AIManager(void);
        
        static AIManager* getInstance();
        static void DestroyInstance();

        void Init();

        void StartThinking() { m_bThink = true; }

        bool AddAvatar(AIAvatar* av);

        //TODO: uncommnet it when RemoveAvatar is implemented
        //bool  RemoveAvatar(AIAvatar* av);

        void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 

        void UpadateAvatarGoals();

    private:
        static AIManager* m_instance;

        std::vector<ActionAvatar*> m_vAvatars;

        bool m_bThink;

    };
}

#endif //_GEN_AI_AIMANAGER_H
