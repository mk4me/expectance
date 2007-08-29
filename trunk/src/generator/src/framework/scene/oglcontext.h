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

	//! A OpenGL Context class
	/*!
	 *	This class is responsible for all operations typical for OpenGL API. 
	 */
	class OGLContext
	{
	public:
        OGLContext(void) { /*empty*/}
	    virtual ~OGLContext(void) { /*empty*/}
        /// \brief singleton - Returns the only instance of OGLContext
        static OGLContext* getInstance();
		//! destroy all resources owned by OGLContext
        static void DestroyInstance();
		
		//! set Viewport for graphics window
		void setWindowSize(int width, int height);
		/// \brief Gets the value of graphics window Height
		int  getHeight();
		/// \brief Gets the value of graphics window Width
		int  getWidth();
		/// \brief Changes rendered floor between normal and textured floor consecutively
		void changeFloorType();
		/// \brief Hides Future Today graphics logo 
		void hideFTLogo();
		/// \brief Initializes OpenGL context (hardware or software) and sets base parameters for lights and rendering
		bool Init();
		//void InitCursorDL();
		/// \brief Creates display list for normal floor objects
		void InitNormalFloorDL(int size);
		/// \brief Creates display list for textured floor objects
		bool InitTexturedFloorDL(int size);
		/// \brief Creates display list for logo texture
		bool InitLogoDL();
		/// \brief Sets camera and viewports parameters and renders floor
		void RenderScene();
		/// \brief Renders FT logo 
		void RenderLogo();
		/// \brief Calculates projection matrix for shadow effect with customized parameters
		void GlShadowProjection(float * l, float * e, float * n);
		//! calculate projecton matrix for shadow effect with default parameters
		void GlShadowProjection();
		//! calculate correct ortho2D surface
		void GLOrtho2DCorrection();
		/// \brief Writes bitmap text on the 2D area given by parameters: location (x,y), font type and text to be displayed
		void OGLWriteBitmap(int font, int x, int y, const char *text);
		//! write stroked text on the 2D area given by parameters: location (x,y), and text to be displayed
		void OGLWriteStroke(int x, int y, const char *text);
		//! inform about FT logo visibility
		bool IsLogoFTActive();
		mutable int HardwareAcceleration;
	private:

		static OGLContext* m_instance;
		bool m_floorType, m_logoFT;
		int m_width, m_height;
	};
}; 

#endif //_GEN_OGL_CONTEXT_H