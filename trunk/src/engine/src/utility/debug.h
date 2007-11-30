/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _GEN_DEBUG_H
#define _GEN_DEBUG_H

#include <fstream>
#include <iostream>

//#define _DEBUGS_TO_FILE   //uncomment it to write logs into a file

#ifdef _DEBUGS_TO_FILE
    #define _dbg Debug::FILE_OUT
#else
    #define _dbg std::cout
#endif

namespace ft
{
	//! A Debug class
	/*!
	 *	Provides a set of methods and properties that help debug your code. 
	 */
    class Debug
    {
    public:

        //////// ADD  BELOW YOUR DEBUG FLAG //////////////

        static int APP;
        static int MODEL_LOADING;
        static int UPDATE;
        static int RENDER;
        static int FPS;
        static int CAMERA;
        static int INPUT;
        static int MESSAGE;
        static int SCENE;

        //////////////////////////////////////

        static bool ON;
        static bool WARN;
        static bool ERR;


        static std::ofstream FILE_OUT;

        static void InitDebug();
        static void Destroy();

        static std::string ERR_STR;
        static std::string WARN_STR;

        static int ReadLevelFromConfig(const char * debugName);

    private:
        static void LoadLevelsFromConfig();
    };
};

#endif //_GEN_DEBUG_H
