/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

#include <iostream>
#include <vector>
#include "../avatar/osgavatar.h"
#include "evolution/avatar.h"
//#include "scene/cameramanager.h"
//#include "core/inputlistener.h"

namespace ft
{
	/**
	 * Class ControlManager: responsible for control layer and manages list of control avatars
	 **/
    class ControlManager //: public UpdateObject, public InputListener
    {
    public:

        bool TRACE;

        ControlManager(void);
	    virtual ~ControlManager(void);
        
        static ControlManager* getInstance();
        static void DestroyInstance();

        void Init();

        void setActiveAvatar(int ind);
		int getActiveAvatarInd(){return m_activeAvatarInd;};
		int getAvatarsCount(){ return (int)m_vAvatars.size();};
        Avatar* getActiveAvatar();

        OsgAvatar* getActiveAvatarImpl();

        bool AddAvatar(Avatar* av);

        //TODO: uncommnet it when RemoveAvatar is implemented
        //bool  RemoveAvatar(AIAvatar* av);

        ////void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 
        ////void OnSpecial(int key, int x, int y);

    private:
		////void SendControlEvent(Avatar* av, int event_id);

        static ControlManager* m_instance;

        std::vector<Avatar*> m_vAvatars;

        int m_activeAvatarInd;

        ////TraceLine *tracer_active_avatar;
        ////void UpdateActiveAvatarMarker();
    };
}

#endif //_GEN_CONTROL_MANAGER_H
