/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef _GEN_OGL_CONTEXT_H
#define _GEN_OGL_CONTEXT_H

#ifndef _CRT_SECURE_NO_WARNINGS    //against fopen ms warnings
#define _CRT_SECURE_NO_WARNINGS 1
#endif


#include "../core/global.h"


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
		///// \brief Initializes context for OGL rendering  - TMP
		//void InitRendering();
		/// \brief Sets ViewPort and Perspective for Scene 
		void setSceneViewport(const bool zoom);
		/// \brief Sets Viewport and Perspective for Data
		void setDataViewport();
		/// \brief Sets View perspective for Data viewport
		void setPerspectiveDVPR();
		/// \brief renders OGL primitives for Scene Vieport
		void DrawSceneViewPortPrimitives();
		/// \brief renders OGL primitives for Data Vieport
		void DrawDataViewPortPrimitives();
		//! set the scope for Data Viewport window width
		const float getDataViewportWidth() { return m_dvpW;};
		//! set the scope for Data Viewport window height
		const float getDataViewportHeight() { return m_dvpH;};
		/// \brief Renders FT logo 
		void RenderLogo();

		//! inform about FT logo visibility
		bool IsLogoFTActive();

    	mutable int HardwareAcceleration;
		bool DATA_VIEWPORT;

	private:
		
		double m_ymin, m_ymax;
		static OGLContext* m_instance;
		bool m_floorType, m_logoFT;
		float m_dvpW,m_dvpH;
		int m_width, m_height;

        bool m_dvpGrid;
        float m_dvpGridStep;
	};

	/// \brief Calculates projection matrix for shadow effect with customized parameters
	void GlShadowProjection(float * l, float * e, float * n);
	//! calculate projecton matrix for shadow effect with default parameters
	void GlShadowProjection();
	//! calculate correct ortho2D surface
	void GLOrtho2DCorrection(const int width, const int height);
	
	//! draw circle or disk given by radius, segments, color, center and info about filling it by color
	void OGLdrawCircleXZ(const float radius, const CalVector& center, const CalVector& color, const int segments = 20, const bool filled = false);

	//! draw arc or pie given by start point, center, color, segments and info about filling it by color
	void OGLdrawArcXZ(const CalVector &start, const CalVector& center, const CalVector& color, const float alpha = 1.0f, const float arcLength = 0.0f, const int segments = 20, const bool filled = false);

	//! draw arc or pie given by two vectors start point, end point , center, color, segments and info about filling it by color
	void OGLdrawArcXZ(const CalVector &start, const CalVector &end, const CalVector& center, const CalVector& color, const float alpha = 1.0f, const int segments = 20, const bool filled = false);


	// \brief Writes bitmap text on the 3D area given by parameters: text to be displayed, location(x,y,z) and text color
	void OGLdraw2DTextAt3D(const char& text, const CalVector& location, const CalVector& color);
	// \brief Writes bitmap text on the 3D area given by parameters: text to be displayed, location(x,y,z) and text color
	void OGLdraw2DTextAt3D(const std::ostringstream& text, const CalVector& location, const CalVector& color);

	/// \brief Writes bitmap text on the 2D area given by parameters: location (x,y), font type and text to be displayed
	void OGLWriteBitmap(int font, int x, int y, const char *text);
	//! write stroked text on the 2D area given by parameters: location (x,y), and text to be displayed
	void OGLWriteStroke(int x, int y, const char *text);
	//! load vertex program from file
	GLuint loadVertexProgram(const std::string fn);
	//! load fragment program from file
	GLuint loadFragmentProgram(const std::string fn);
}

#endif //_GEN_OGL_CONTEXT_H