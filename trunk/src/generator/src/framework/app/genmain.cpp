/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "genmain.h"
#include "core/glutapp.h"
#include "generatorapp.h"

void RunGlutApp(int argc, char *argv[])
{
    ft::Application* app = new ft::GeneratorApp();

    if (argv == NULL)
    {
        int _argc = 2;
        char *_argv[2];
        //std::string myPath = "Empty";
        //std::string model = "cally.cfg";
        //argv[0] = myPath.cc_str();
        //argv[1] = model.c_str();

        _argv[0] = (char*)malloc(20);
        _argv[1] = (char*)malloc(20);
        strcpy(_argv[0],"");
        strcpy(_argv[1],"cally.cfg");
   
        InitGlutApplication(_argc, _argv, app);
    }
    else
    {
        InitGlutApplication(argc, argv, app);
    }

    delete app;
}



/**
 * \brief Main entry to application
 *
 * \param int argc - number of arguments
 * \param char *[] argv - list of arguments
 * \return int - result of application
 **/
int main(int argc, char *argv[])
{
    RunGlutApp(argc, argv);
    return 0;
}
