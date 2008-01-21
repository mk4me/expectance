// Sources from http://www.relisoft.com/win32/active.html

// The constructor of the derived class
// should call
//    _thread.Resume ();
// at the end of construction
#include "threads.h"

ActiveObject::ActiveObject ()
: _isDying (0),
#pragma warning(disable: 4355) // 'this' used before initialized
  _thread (ThreadEntry, this)
#pragma warning(default: 4355)
{
}

void ActiveObject::Kill ()
{
    _isDying++;
    FlushThread ();
    // Let's make sure it's gone
    _thread.WaitForDeath ();
}

DWORD WINAPI ActiveObject::ThreadEntry (void* pArg)
{
    ActiveObject * pActive = (ActiveObject *) pArg;
    pActive->InitThread ();
    pActive->Run ();
    return 0;
}

