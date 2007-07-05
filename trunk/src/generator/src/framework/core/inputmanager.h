/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_INPUT_MANAGER_H
#define _GEN_INPUT_MANAGER_H

#include <iostream>
#include "../utility/debug.h"
#include "../scene/camera.h"
#include "../ui/menumanager.h"
#include "../scene/scenemanager.h"
#include "../scene/oglcontext.h"
#include "controlmanager.h"
#include "message.h"

namespace ft
{
    class InputManager
    {
    public:
        InputManager(void) { /*empty*/}
	    virtual ~InputManager(void) { /*empty*/}
        
        static InputManager* getInstance();
        static void DestroyInstance();

        void OnKey(unsigned char key, int x, int y);
        void OnSpecial(int key, int x, int y);
        void OnMouseButtonDown(int button, int x, int y);
        void OnMouseButtonUp(int button, int x, int y);
        void OnMouseMove(int x, int y);

    private:
        static InputManager* m_instance;
    };
};

#endif //_GEN_INPUT_MANAGER_H