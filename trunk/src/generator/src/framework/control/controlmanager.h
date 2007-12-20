/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

#include "../ai/aiavatar.h"
#include "scene/cameramanager.h"
#include "core/inputlistener.h"

namespace ft
{
	/**
	 * Class ControlManager: responsible for control layer and manages list of control avatars
	 **/
    class ControlManager : public UpdateObject, public InputListener
    {
    public:

        bool TRACE;

        ControlManager(void);
	    virtual ~ControlManager(void);
        
        static ControlManager* getInstance();
        static void DestroyInstance();

        void Init();

        void setActiveAvatar(int ind);
        AIAvatar* getActiveAvatar();

        bool AddAvatar(AIAvatar* av);

        //TODO: uncommnet it when RemoveAvatar is implemented
        //bool  RemoveAvatar(AIAvatar* av);

        void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 

        void OnSpecial(int key, int x, int y);

    private:
        static ControlManager* m_instance;

        std::vector<AIAvatar*> m_vAvatars;

        int m_activeAvatarInd;

        TraceLine *tracer_active_avatar;

        void UpdateActiveAvatarMarker();
    };
}

#endif //_GEN_CONTROL_MANAGER_H