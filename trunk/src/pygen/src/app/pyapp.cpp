/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "app/genmain.h"
#include <iostream>
#include <boost/thread/thread.hpp>

#include "core/propmanager.h"

using namespace ft;

void runLoop()
{
    RunGlutApp(0,NULL);
}


///////////////// Interface for PYTHON //////////////////
void runApp()
{
    std::cout << " New thread will be started for Generator ..." << std::endl;
    boost::thread thrd(&runLoop);
    
    //TODO:  add somewhere deletion of above thread object 
}


////////// Properties

void setPropStr(const std::string& key, std::string val)
{
    PropManager::getInstance()->setPropertyStr(key, val);
}

void setPropInt(const std::string& key, int val)
{
    PropManager::getInstance()->setPropertyInt(key, val);
}

void setPropFloat(const std::string& key, float val)
{
    PropManager::getInstance()->setPropertyFloat(key, val);
}

std::string getPropStr(const std::string& key)
{
    return PropManager::getInstance()->getPropertyStr(key);
}

int getPropInt(const std::string& key)
{
    return PropManager::getInstance()->getPropertyInt(key);
}

float getPropFloat(const std::string& key)
{
    return PropManager::getInstance()->getPropertyFloat(key);
}

void dumpProps()
{
    PropManager::getInstance()->DumpProperties();
}

void test()
{
    std::cout << " Some text from pygen.cpp " << std::endl;
}




