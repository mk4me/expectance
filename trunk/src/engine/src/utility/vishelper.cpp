/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "vishelper.h"

using namespace ft;

CalVector VisualizationHelper::COLOR_WHITE = CalVector(255,255,255);
CalVector VisualizationHelper::COLOR_BLACK = CalVector(0,0,0);
CalVector VisualizationHelper::COLOR_RED = CalVector(255,0,0);
CalVector VisualizationHelper::COLOR_GREEN = CalVector(0,255,0);
CalVector VisualizationHelper::COLOR_BLUE = CalVector(0,0,255);
CalVector VisualizationHelper::COLOR_SKYBLUE = CalVector(0,100,100);
CalVector VisualizationHelper::COLOR_PINK = CalVector(128, 0, 128);
CalVector VisualizationHelper::COLOR_YELLOW = CalVector(255,255,0);

/**
 * \brief Adds helper trace line for rendering
 * \param TraceLine *traceLine - traceline object (should be created and added to ft::VisualizationManager before)
 * \param  CalVector vBaseDir - base vector from which given rotation is defined
 * \param CalVector pos - current position of origin of traceline
 * \param CalQuaternion rot - rotation to visualize
 * \param float line_length - lenght of traceline to draw
 * \param CalVector vColor - collor of traceline
 */
void VisualizationHelper::TraceRotation(TraceLine *traceLine, CalVector vBaseDir, CalVector pos, CalQuaternion rot, float line_length, CalVector vColor)
{
    traceLine->ClearTrace();

    traceLine->setColor(vColor);
    traceLine->AddPoint(pos);

    vBaseDir *= line_length;
    vBaseDir *= rot;

    traceLine->AddPoint(pos + vBaseDir);
}

/**
 * \brief Adds helper trace line for rendering
 * \param TraceLine *traceLine - traceline object (should be created and added to ft::VisualizationManager before)
 * \param  CalVector vVector - vector to visualize
 * \param CalVector pos - current position of origin of traceline
 * \param float line_length - lenght of traceline to draw
 * \param CalVector vColor - collor of traceline
 */
void VisualizationHelper::TraceVector(TraceLine *traceLine, CalVector vVector, CalVector pos, float line_length, CalVector vColor)
{
    traceLine->ClearTrace();

    if (vVector.length() >0)
    {
        traceLine->setColor(vColor);
        traceLine->AddPoint(pos);

        vVector.normalize();
        vVector*= line_length;

        traceLine->AddPoint(pos + vVector);
    }
}