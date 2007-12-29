/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_VIS_HELPER_H
#define _GEN_VIS_HELPER_H

#include "../scene/traceline.h"

namespace ft
{
	/**
	 * Class VisualizationHelper: useful methods to vizualization adorments
	 **/
    class VisualizationHelper
    {
    public:
        static CalVector COLOR_WHITE;
        static CalVector COLOR_BLACK;
        static CalVector COLOR_RED;
        static CalVector COLOR_GREEN;
        static CalVector COLOR_BLUE;
        static CalVector COLOR_SKYBLUE;
        static CalVector COLOR_PURPLE;
        static CalVector COLOR_ORANGE;
        static CalVector COLOR_YELLOW;

        static void TraceRotation(TraceLine *traceLine, CalVector vBaseDir, CalVector pos, CalQuaternion rot, float line_length, CalVector vColor);
        static void TraceVector(TraceLine *traceLine, CalVector vVector, CalVector pos, float line_length, CalVector vColor);
    };
}

#endif //_GEN_VIS_HELPER_H