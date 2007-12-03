/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "aiavatar.h"
#include "../app/gendebug.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param Cal3dType * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
AIAvatar::AIAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
:ActionAvatar(calModel, calCoreModel, modelName)
{
    m_currGoal = NULL;
    m_currGoalController = NULL;
    SetThink(false);
}

// \brief Destructor
AIAvatar::~AIAvatar()
{
    //empty
}

/// \brief Releases all resources and objects related to this Avatar
void AIAvatar::Destroy(void)
{
    ActionAvatar::Destroy();
    //implement additional stuff
}

/// \brief Initiates AI part for Avatar
void AIAvatar::Init()
{
    ActionAvatar::Init();
}


/// \brief Resets all parameters related to AI part of this avatar
void AIAvatar::Reset()
{
    ActionAvatar::Reset();

    if (GenDebug::AI>0)
        _dbg << toString() << " AIAvatar::Reset() " << std::endl;
}

void AIAvatar::SetThink(boolean set)
{
    if (set)
    {
        if (!m_bThink)
        {
            m_bThink = true;
        }
    }
    else
    {
        if (m_bThink)
        {
            if (getGoalController() != NULL)
            {
                getGoalController()->setGoalReached(true);
            }
            if (getCurrGoal() != NULL)
            {
                setCurrGoal(NULL);
            }
            m_bThink=false;
        }
    }

}
