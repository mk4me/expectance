/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _GEN_DEBUG_H
#define _GEN_DEBUG_H


#include <iostream>

#define DEBUG_MESSAGES false

namespace ft
{
	//! A Debug class
	/*!
	 *	Provides a set of methods and properties that help debug your code. 
	 */
    class Debug
    {
    public:
        static std::string ERR_STR;
        static std::string WARN_STR;

        static bool ON;
        static bool WARN;
        static bool ERR;

        static void LoadLevelsFromConfig();

        //////// ADD  BELOW YOUR DEBUG FLAG //////////////

        static int APP;
        static int MODEL_LOADING;
        static int TIMELINE;
        static int ACTION;
        static int AI;
        static int UPDATE;
        static int MOTION;
        static int CONTROL;
        static int RENDER;
        static int FPS;
        static int CAMERA;

        //////////////////////////////////////
    };
};

#endif //_GEN_DEBUG_H
