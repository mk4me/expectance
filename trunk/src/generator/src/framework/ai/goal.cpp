/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "goal.h"

using namespace ft;

/// \brief Constructor
Goal::Goal()
{
    //empty
}

// \brief Destructor
Goal::~Goal()
{
    //empty
}

/// \brief Releases all resources and objects related to this object
void Goal::Destroy(void)
{
}

bool Goal::ExecuteAction(ActionAvatar *av)
{
    bool result = false;
    int actionId = getActionToPerform();
    if (actionId != -1)
    {
        Action* action = av->SetActionById(actionId);
        if (action == NULL)
        {
            cout << " ERR: Goal::ExecuteAction - action id not correct:" << actionId << endl;
        }
        else
        {
            result = true;
        }
    }
    return result;
}

int Goal::getActionToPerform()
{
    return -1;
}

AIController* Goal::CreateController(ActionAvatar *av)
{
    return new AIController();
}

