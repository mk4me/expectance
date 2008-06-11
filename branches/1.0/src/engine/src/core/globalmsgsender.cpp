/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "globalmsgsender.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;


GlobalMsgSender* GlobalMsgSender::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::GlobalMsgSender (creates it at first call to this method)
 *
 * \return ft::GlobalMsgSender * - the only instance of GlobalMsgSender
 **/
GlobalMsgSender* GlobalMsgSender::getInstance()
{
    if (m_instance == NULL)
    {
        if (Debug::MESSAGE>0)
            _dbg << "GlobalMsgSender::getInstace(): instance of GlobalMsgSender created " << endl;

        m_instance = new GlobalMsgSender();
    }

    return m_instance;
}

/**
 \brief Releases all resources related to GlobalMsgSender
 *
 **/
void GlobalMsgSender::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}
