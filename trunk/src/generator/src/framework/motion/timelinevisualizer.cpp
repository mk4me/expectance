/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinevisualizer.h"
#include "utility/vishelper.h"
#include "scene/scenemanager.h"
#include "../evolution_impl/cal3danimexecution.h"

using namespace ft;

bool TimeLineVisualizer::CURVE_ANIM_WEIGHT = false;

/// \brief constructor
TimeLineVisualizer::TimeLineVisualizer()
{
    CURVE_ANIM_WEIGHT = (Config::getInstance()->IsKey("anim_weight_curve")) && (Config::getInstance()->GetIntVal("anim_weight_curve")==1);

    if (CURVE_ANIM_WEIGHT)
    {
        curve_prev_anim_weight = new DataCollector(toString() + "curve_prev_anim_weight");
        SceneManager::getInstance()->AddDataObject(curve_prev_anim_weight);
        curve_prev_anim_weight->HidePoints();       curve_prev_anim_weight->setColor(VisualizationHelper::COLOR_GREEN);
        curve_prev_anim_weight->setDrawScale(40);   curve_prev_anim_weight->setLegendLabel("weight of prev anim");

        curve_curr_anim_weight = new DataCollector(toString() + "curve_curr_anim_weight");
        SceneManager::getInstance()->AddDataObject(curve_curr_anim_weight);
        curve_curr_anim_weight->HidePoints();       curve_curr_anim_weight->setColor(VisualizationHelper::COLOR_YELLOW);
        curve_curr_anim_weight->setDrawScale(40);   curve_curr_anim_weight->setLegendLabel("weight of curr anim");
    }
}

/// \brief destructor
TimeLineVisualizer::~TimeLineVisualizer(void)
{
    if (CURVE_ANIM_WEIGHT)
    {
        curve_prev_anim_weight->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_prev_anim_weight);

        curve_curr_anim_weight->Clear();
        SceneManager::getInstance()->RemoveDataObject(curve_curr_anim_weight);
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
    Controller::Apply(elapsedSeconds, timeLineContext);

    if (CURVE_ANIM_WEIGHT)
    {
        float prevAnimValue = 0;
        float currAnimValue = 0;

        if (timeLineContext->prevAnim != NULL)
        {
			CalAnimation* prevAnim = ((Cal3dAnimExecution*)timeLineContext->prevAnim)->getAnimation();
            prevAnimValue = prevAnim->getWeight();
        }

        if (timeLineContext->currAnim != NULL)
        {
			CalAnimation* currAnim = ((Cal3dAnimExecution*)timeLineContext->currAnim)->getAnimation();
            currAnimValue = currAnim->getWeight();
        }

        curve_prev_anim_weight->AddValue(prevAnimValue);
        curve_curr_anim_weight->AddValue(currAnimValue);
    }


/*
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
*/
}


/// \brief Resets parameters of this modifier
void TimeLineVisualizer::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);
    if (CURVE_ANIM_WEIGHT)
    {
        curve_prev_anim_weight->Clear();
        curve_curr_anim_weight->Clear();
    }
}

/**
 * \brief Returns string representation of this TimeLineVisualizer
 *
 * \return std::string - string representation
 **/
std::string TimeLineVisualizer::toString()
{
    std::string result = Controller::toString() + "[TimeLineVisualizer]";
    return result;
}


