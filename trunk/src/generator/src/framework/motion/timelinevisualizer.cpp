/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinevisualizer.h"
#include "../utility/vishelper.h"

using namespace ft;

bool TimeLineVisualizer::TRACK_ON = true;

/// \brief constructor
TimeLineVisualizer::TimeLineVisualizer()
{
    TRACK_TIMELINE_STATES = false;

    if (TRACK_TIMELINE_STATES)
    {
        tracer_timeline_states = new TraceLine(toString() + "TimeLineVisualizer_tracer_timeline_states");
        VisualizationManager::getInstance()->AddObject(tracer_timeline_states);
        tracer_timeline_states->HideMarker();
    }	
}

/// \brief destructor
TimeLineVisualizer::~TimeLineVisualizer(void)
{
    if (tracer_timeline_states != NULL)
    {
        tracer_timeline_states->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_timeline_states);
    }
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void TimeLineVisualizer::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);

    CalVector vAvatarPos = timeLineContext->getAvatar()->getPosition();

    if (TRACK_TIMELINE_STATES)
    {
        int state = timeLineContext->exec_state;

        if (timeLineContext->exec_event == EXEC_EVENT_STATE_CHANGED)
        {
            tracer_timeline_states->ClearTrace();
        }

        if (state == EXEC_STATE_SINGLE || state == EXEC_STATE_FADE_IN 
            || state == EXEC_STATE_FADE_OUT || state == EXEC_STATE_OVERLAP)
        {
            tracer_timeline_states->AddPoint(vAvatarPos);

            switch(state)
            {
            case EXEC_STATE_SINGLE:
                tracer_timeline_states->setColor(VisualizationHelper::COLOR_WHITE);
                break;
            case EXEC_STATE_FADE_IN:
                tracer_timeline_states->setColor(VisualizationHelper::COLOR_YELLOW);
                break;
            case EXEC_STATE_FADE_OUT:
                tracer_timeline_states->setColor(VisualizationHelper::COLOR_GREEN);
                break;
            case EXEC_STATE_OVERLAP:
                tracer_timeline_states->setColor(VisualizationHelper::COLOR_RED);
                break;
            default:
                break;
            }
        }
        else
        {
            tracer_timeline_states->ClearTrace();
        }
    }
}


/// \brief Resets parameters of this modifier
void TimeLineVisualizer::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    if (TRACK_TIMELINE_STATES)
    {
        tracer_timeline_states->ClearTrace();
    }
}

