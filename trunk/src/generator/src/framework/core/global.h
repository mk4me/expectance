//----------------------------------------------------------------------------//
// global.h                                                                   //
// Copyright (C) 2007 Future Today                                            //
//                                                                            //
//----------------------------------------------------------------------------//


#ifndef GLOBAL_H
#define GLOBAL_H


#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <vector>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include "../scene/GL/glew.h"
#include <GL/glut.h>

#include "cal3d/cal3d.h"
#include "../scene/textureManager.h"

static const std::string FT_SHADERPATH = "data\\shaders\\";
static const std::string FT_TEXTUREPATH = "data\\textures\\";
static const std::string FT_MODELPATH = "data\\cally\\";

#endif



