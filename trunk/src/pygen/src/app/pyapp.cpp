/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "app/genmain.h"

#include <iostream>

#include "genthread.h"
#include <boost/python.hpp>
using namespace boost::python;


class GlutThread : public GenThread
{
    virtual void Run () 
    { 
        RunGlutApp(0,NULL);
    }
};

void runApp()
{
    GlutThread *thread = new GlutThread();
    //TODO:  add somewhere deletion of above thread object 
}

void printSomeText()
{
    std::cout << " Some text from generator " << std::endl;
}

BOOST_PYTHON_MODULE(pygen)
{
    def("runApp", runApp);
    def("printSomeText", printSomeText);
}
