/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "msgsender.h"
#include "../utility/debug.h"

using namespace ft;

/**
 * \brief Adds listener  to MsgSender
 * \param ft::MsgListener* listener - listener to add
 **/
void MsgSender::AddMsgListener(MsgListener* listener)
{
    if (Debug::MESSAGE>0)
        _dbg << " MsgSender::AddListener " << listener << std::endl;

    m_vListeners.push_back(listener);
}

bool  MsgSender::RemoveMsgListener(MsgListener* listener)
{
    bool result = false;

    std::vector<MsgListener*>::iterator iteratorListener;
    iteratorListener = m_vListeners.begin();

    while(iteratorListener != m_vListeners.end())
    {
        // find the specified action and remove it
        if((*iteratorListener) == listener)
        {
            // found, so remove
            m_vListeners.erase(iteratorListener);
            result = true;
            break;
        }
        iteratorListener++;
    }

        if (Debug::MESSAGE>0)
        {
            if (result)
                _dbg << " MsgSender::RemoveListener " << listener << " removed " << std::endl;
             else
                _dbg << " MsgSender::RemoveListener " << listener << " - removing FAILED " << std::endl;
        }

    return result;
}

void MsgSender::Destroy()
{
    if (m_vListeners.size() > 0)
        m_vListeners.clear();
}

/**
 * \brief This method is used to send new message to objectes registered in MsgSender
 *
 * \param ft::Message * msg - message to send
 * \param bool deleteAfterSent - defines if message object msg should be deleted after sending
 **/
void MsgSender::SendMsg(Message* msg, bool deleteAfterSent)
{
    if (Debug::MESSAGE>0)
        _dbg << "MsgSender " << this << " SendMsg: "<< Message::_GET_MSG_NAME(msg->getType())  << std::endl;


    if (m_vListeners.size() > 0)
    {
        for (int m=0; m<(int)m_vListeners.size(); m++)
        {
            m_vListeners[m]->OnMessage(msg);
        }
    }
    if (deleteAfterSent && msg != NULL)
    {
        if (msg->getParam() != NULL)
            delete msg->getParam();
        delete msg;
    }

}

