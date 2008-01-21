/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "threads/threads.h"
#include <iostream>

class GenThread : public ActiveObject
{
public:
    GenThread()
    {
        _thread.Resume ();
    }
    ~GenThread()
    {
        Kill ();
    }

protected:
    void InitThread () {}
    virtual void Run () { std::cout << " Run " << std::endl; }
    void FlushThread () {}
};