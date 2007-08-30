/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

#include "controlavatar.h"

namespace ft
{
	/**
	 * Class ControlManager: responsible for control of avatars
	 **/
    class ControlManager
    {
    public:
        ControlManager(void) { /*empty*/}
	    virtual ~ControlManager(void) { /*empty*/}
        
        static ControlManager* getInstance();
        static void DestroyInstance();

        void Init();

        void setActiveAvatar(ControlAvatar* av) { m_activeAvatar = av; };
        ControlAvatar* getActiveAvatar() { return m_activeAvatar; }

        void OnSpecial(int key, int x, int y);

    private:
        static ControlManager* m_instance;

        ControlAvatar* m_activeAvatar;
    };
};

#endif //_GEN_CONTROL_MANAGER_H