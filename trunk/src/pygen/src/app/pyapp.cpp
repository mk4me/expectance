/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "app/genmain.h"
#include <iostream>
#include <boost/thread/thread.hpp>

void runLoop()
{
    RunGlutApp(0,NULL);
}

void runApp()
{
    std::cout << " New thread will be started for Generator ..." << std::endl;
    boost::thread thrd(&runLoop);
    
    //TODO:  add somewhere deletion of above thread object 
}

void printSomeText()
{
    std::cout << " Some text from pygen.cpp " << std::endl;
}


