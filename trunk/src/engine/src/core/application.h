/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_APPLICATION_H
#define _GEN_APPLICATION_H

#include "inputlistener.h"

namespace ft
{
	/**
	 * Class Application: is responsible for lifecycle of Generator application
	 **/
    class Application : public InputListener
    {
    public:
        virtual int Init();

        void InitConfig();

        virtual bool InitModules();
        virtual void InitObjects();

        void OnKey(unsigned char key, int x, int y);
    };
};

#endif //_GEN_APPLICATION_H

