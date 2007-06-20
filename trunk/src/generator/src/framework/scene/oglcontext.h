/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef _GEN_OGL_CONTEXT_H
#define _GEN_OGL_CONTEXT_H

#include "../core/global.h"
#include "../core/config.h"
#include "texturemanager.h"
#include "camera.h"

namespace ft
{
	// display list ids
	static const int OGL_DL_FLOOR = 1;
	static const int OGL_DL_TEXTURED_FLOOR = 2;
	static const int OGL_DL_LOGO  = 3;

	class OGLContext
	{
	public:
        OGLContext(void) { /*empty*/}
	    virtual ~OGLContext(void) { /*empty*/}
        
        static OGLContext* getInstance();
        static void DestroyInstance();
		
		void setWindowSize(int width, int height);
		int  getHeight();
		int  getWidth();
		void changeFloorType();

		void Init();
		//void InitCursorDL();
		void InitNormalFloorDL(int size);
		bool InitTexturedFloorDL(int size);
		bool InitLogoDL();
		void RenderScene();
		void RenderLogo();

		void GlShadowProjection(float * l, float * e, float * n);
		void GlShadowProjection();
	private:
		static OGLContext* m_instance;
		bool m_floorType;
		int m_width, m_height;
	};
}; 

#endif //_GEN_OGL_CONTEXT_H