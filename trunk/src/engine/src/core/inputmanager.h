/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_INPUT_MANAGER_H
#define _GEN_INPUT_MANAGER_H

#include "inputlistener.h"
#include "../utility/stlhelper.h"

namespace ft
{
	/**
	 * Class InputManager: handles inputs from keyboard and mouse and passes them to appropriate modules
	 **/
    class ENGINE_API InputManager
    {
    public:
        InputManager(void) { /*empty*/}
	    virtual ~InputManager(void) { /*empty*/}
        
        static InputManager* getInstance();
        static void DestroyInstance();

        void AddListener(InputListener* listener);
        bool RemoveListener(InputListener* listener);

        void OnKey(unsigned char key, int x, int y);
        void OnSpecial(int key, int x, int y);
        void OnMouseButtonDown(int button, int x, int y);
        void OnMouseButtonUp(int button, int x, int y);
        void OnMouseMove(int x, int y);

    private:
        static InputManager* m_instance;

        STL_WrappedVector<InputListener*> m_vListeners;

    };
}

#endif //_GEN_INPUT_MANAGER_H