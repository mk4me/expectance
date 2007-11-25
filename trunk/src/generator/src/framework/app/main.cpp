/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "core/mainapp.h"
#include "generatorapp.h"

/**
 * \brief Main entry to application
 *
 * \param int argc - number of arguments
 * \param char *[] argv - list of arguments
 * \return int - result of application
 **/
int main(int argc, char *argv[])
{
    ft::Application* app = new ft::GeneratorApp();
    InitGlutApplication(argc, argv, app);

    delete app;

    return 0;
}
