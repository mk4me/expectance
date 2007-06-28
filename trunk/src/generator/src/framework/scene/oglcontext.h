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
    
	static void* bitmap_fonts[7] = {
      GLUT_BITMAP_9_BY_15,
      GLUT_BITMAP_8_BY_13,
      GLUT_BITMAP_TIMES_ROMAN_10,
      GLUT_BITMAP_TIMES_ROMAN_24,
      GLUT_BITMAP_HELVETICA_10,
      GLUT_BITMAP_HELVETICA_12,
      GLUT_BITMAP_HELVETICA_18     
   };

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
		void hideFTLogo();

		void Init();
		//void InitCursorDL();
		void InitNormalFloorDL(int size);
		bool InitTexturedFloorDL(int size);
		bool InitLogoDL();
		void RenderScene();
		void Render2D();

		void GlShadowProjection(float * l, float * e, float * n);
		void GlShadowProjection();
		void OGLWriteBitmap(int font, int x, int y, const char *text);
		void OGLWriteStroke(int x, int y, const char *text);

	private:
		void GLOrtho2DCorrection();
		static OGLContext* m_instance;
		bool m_floorType, m_logoFT;
		int m_width, m_height;
	};
}; 

#endif //_GEN_OGL_CONTEXT_H