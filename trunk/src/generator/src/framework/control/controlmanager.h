/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

#include "controlavatar.h"
#include "../scene/cameramanager.h"

namespace ft
{
	/**
	 * Class ControlManager: responsible for control of avatars
	 **/
    class ControlManager : public UpdateObject
    {
    public:

        bool TRACE;

        ControlManager(void);
	    virtual ~ControlManager(void);
        
        static ControlManager* getInstance();
        static void DestroyInstance();

        void Init();

        void setActiveAvatar(int ind);
        ControlAvatar* getActiveAvatar();

        bool AddControlAvatar(ControlAvatar* av);

        //TODO: uncommnet it when RemoveControlAvatar is implemented
        //bool  RemoveControlAvatar(ControlAvatar* av);

        void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 

        void OnSpecial(int key, int x, int y);

    private:
        static ControlManager* m_instance;

        std::vector<ControlAvatar*> m_vControlAvatars;

        int m_activeAvatarInd;

        TraceLine *tracer_active_avatar;

        void UpdateActiveAvatarMarker();
    };
};

#endif //_GEN_CONTROL_MANAGER_H