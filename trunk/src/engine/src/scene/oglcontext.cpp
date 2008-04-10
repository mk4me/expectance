/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "oglcontext.h"
#include "texturemanager.h"
#include "../utility/mathconversions.h"
#include "../utility/mathutil.h"


using namespace ft;

OGLContext* OGLContext::m_instance = NULL;

OGLContext* OGLContext::getInstance()
{
    if (m_instance == NULL)
    {
		_dbg << "OGLContext::getInstace(): instance of OGLContext created " << std::endl;
        m_instance = new OGLContext();
    }

    return m_instance;
}

void OGLContext::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

int OGLContext::getHeight()
{
	return m_height;
}

int OGLContext::getWidth()
{
	return m_width;
}

void OGLContext::changeFloorType()
{
	m_floorType = !m_floorType;
}

void OGLContext::hideFTLogo()
{
	m_logoFT = !m_logoFT;
}

void OGLContext::setWindowSize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, m_width, m_height);
	m_dvpW = m_width;
	m_dvpH = m_height/4;
}


bool OGLContext::Init()
{

	m_width = glutGet (GLUT_SCREEN_WIDTH);
	m_height = (glutGet (GLUT_SCREEN_HEIGHT))/2;
	m_floorType = 0;
	m_logoFT = 0;

	m_dvpW = m_width;
	m_dvpH = m_height/4;
	GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

    GLfloat	LightPos[] = {1.0f, 100.0f, 1.0f, 1.0f};	// Light Position mka 2007-08-29

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Enable light


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);				// Set The Position For Light0
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	// check hardware acceleration possibility
	if(Config::getInstance()->IsKey("hardware_acceleration"))
	{
		HardwareAcceleration = Config::getInstance()->GetIntVal("hardware_acceleration");
	}
	else
		HardwareAcceleration = 0; // no config settings

	if (HardwareAcceleration == 1)
	{
		glewInit();

		if (!GLEW_ARB_vertex_program)
		{
		  std::cerr << "Error ARB_vertex_program OpenGL extension not found." << std::endl;
		  return false;
		}

		if (!GLEW_ARB_vertex_buffer_object)
		{
		  std::cerr << "Error ARB_vertex_buffer_object OpenGL extension not found." << std::endl;
		  return false;
		}
	}

	DATA_VIEWPORT = ((Config::getInstance()->IsKey("data_viewport")) && (Config::getInstance()->GetIntVal("data_viewport")==1));

    bool m_dvpGrid = ((Config::getInstance()->IsKey("data_viewport_draw_grid")) && (Config::getInstance()->GetIntVal("data_viewport_draw_grid")==1));

    m_dvpGridStep = 10;
    if (Config::getInstance()->IsKey("data_viewport_grid_step"))
    {
        m_dvpGridStep = Config::getInstance()->GetIntVal("data_viewport_grid_step");
    }

    return true;
}



void OGLContext::InitNormalFloorDL(int size)
{
	int i,j,k;
	float lightDots[] = {0.9f,0.9f,0.36f};
	float lightAxes[] = {0.298f,0.298f,0.36f};
	float darkAxes[] =  {0.25f,0.25f,0.298f};
	float mainAxes[] =  {0.349f,0.349f,0.419f};
	int tileSize	= 128;
	int ts;
	j = size/2;
	ts = tileSize*j;

	glNewList(OGL_DL_FLOOR, GL_COMPILE);
	glPushMatrix();

	glBegin(GL_QUADS); // draw carpet
		glNormal3f( 0.0f, 1.0f, 0.0f );
		glColor3f(0.245f,0.245f,0.245f);
		glVertex3f(-ts, 0,  ts);
		glVertex3f(ts, 0,  ts);
		glVertex3f(ts, 0, -ts);
		glVertex3f(-ts, 0,  -ts);
	glEnd();

	for (i = -j; i <= j; i++) // and some lines
	{
		k = tileSize*i;
		if ((i%5 == 0)&&(i!=0))
		{
			glColor3fv(lightDots);
			glBegin(GL_LINES); // drop short yellow crosses
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glVertex3f(k+2, 0,  -2);glVertex3f(k-2, 0,  2);
			glVertex3f(k-2, 0,  -2);glVertex3f(k+2, 0,  2);
			glVertex3f(-2, 0,  k+2);glVertex3f(2, 0,  k-2);
			glVertex3f(-2, 0,  k-2);glVertex3f(2, 0,  k+2);
			glEnd();
			glColor3fv(lightAxes);
		}
		else if (i == 0)
			glColor3fv(mainAxes);
		else
			glColor3fv(darkAxes);

		glBegin(GL_LINES);
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glVertex3f(k, 0,  ts);
			glVertex3f(k, 0,  -ts);
			glVertex3f(ts, 0,  k);
			glVertex3f(-ts, 0,  k);
		glEnd();

	}

	//and axes
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(30, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 30, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 30);
	glEnd();

	glPopMatrix();
	glEndList();
}



/*----- Prepare textured floor -----*/
bool  OGLContext::InitTexturedFloorDL(int size)
{
	int x, y, kx, ky, j;
    int tileSize = 128;
	GLuint floorTexture;
	float fAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float fDiffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float fSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float fShine[] = { 5.0 };
	j = size/2;

	// CREATE THE DISPLAY LIST
	glNewList(OGL_DL_TEXTURED_FLOOR,GL_COMPILE);
		glMaterialfv( GL_FRONT, GL_AMBIENT, fAmbient );
		glMaterialfv( GL_FRONT, GL_DIFFUSE, fDiffuse );
		glMaterialfv( GL_FRONT, GL_SPECULAR, fSpecular);
		glMaterialfv( GL_FRONT, GL_SHININESS, fShine);

		glPushMatrix();
			glEnable(GL_TEXTURE_2D);

			if ((floorTexture = ft::TextureManager::getInstance()->LoadTexture(FT_TEXTUREPATH + Config::getInstance()->GetStrVal("floor_texture")))==0)
				return false;

			for(x = -j; x <= j; x++)   // for every row
			{
				kx= tileSize*x;
				for(y = -j; y <= j; y++)   // for every column
				{
					ky = tileSize*y;
					glBindTexture(GL_TEXTURE_2D, floorTexture);
					glColor3f(1.0f, 1.0f, 1.0f);
					glBegin(GL_QUADS);
					glNormal3f( 0.0f, 1.0f, 0.0f );
					glTexCoord2f(0.0f, 0.0f); glVertex3f(kx,  0, ky);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(kx, 0, tileSize*(y+1));
					glTexCoord2f(1.0f, 1.0f); glVertex3f(tileSize*(x+1), 0, tileSize*(y+1));
					glTexCoord2f(0.0f, 1.0f); glVertex3f(tileSize*(x+1), 0, ky);
					glEnd();
				}
			}
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glEndList();
	return true;
}

bool OGLContext::InitLogoDL()
{
	GLuint logoTexture;
	GLfloat _width,_height;
	_width = 90;
	_height = 32;

	glNewList(OGL_DL_LOGO,GL_COMPILE);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		if ((logoTexture = ft::TextureManager::getInstance()-> LoadTexture(FT_TEXTUREPATH + Config::getInstance()->GetStrVal("logo_texture")))==0)
			return false;

		glBindTexture(GL_TEXTURE_2D,logoTexture);
		glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(_width, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(_width, _height);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0, _height);
	    glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	glEndList();
	return true;
}


void OGLContext::setSceneViewport(const bool zoom)
{
	static float _tempAng = 0;
	//static bool _switch = false;
	double _fovy;
	glClear(GL_COLOR_BUFFER_BIT);
	if(DATA_VIEWPORT)
		glViewport(0, m_height/4, m_width, m_height);
	else
		glViewport(0, 0, m_width, m_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (zoom) //zoom is on
	{
		if (_tempAng <= 3.141592f)
		{
			_fovy = cos(_tempAng += 0.05F)*15; // decrease _fovy value in the scope [15...-15]
		}
		else
			_fovy = -15;					   // after that keep constant value
	}
	else  //zom is off
	{
		if (_tempAng >= 0.0f)
		{
			_fovy = cos(_tempAng -= 0.05F)*15; // increase _fovy value in the scope [-15...15]
		}
		else
			_fovy = 15;						   // after that keep constant value
	}

	gluPerspective(_fovy+45.0f, (GLdouble)m_width / (GLdouble)m_height, 1, 100000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear (GL_DEPTH_BUFFER_BIT);

}

void OGLContext::setDataViewport()
{
	glViewport(0, 0, (int)m_dvpW, (int)m_dvpH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, (int)m_dvpW, 0, (int)m_dvpH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OGLContext::DrawSceneViewPortPrimitives()
{
	//render floor
	glPushMatrix();
		if (m_floorType == 0)
			glCallList(OGL_DL_FLOOR);

		if (m_floorType == 1)
		{
			glDisable(GL_BLEND);
			glCallList(OGL_DL_TEXTURED_FLOOR);
		}
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}

void OGLContext::DrawDataViewPortPrimitives()
{
	glPushMatrix();	// draw axis and so on ...

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw background for (x) 1D
	glColor3f(1, 1, 1);
	glRectf(0.0f, 0.0f, m_dvpW, m_dvpH);
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(1.0f, 1.0f, m_dvpW/2-1, m_dvpH-2);
	glRectf(m_dvpW/2+1, 1.0f, m_dvpW-1, m_dvpH-2);

    //draw x axis
	glColor4f(1,1,1,0.3f);
	glBegin(GL_LINES);
	glVertex2d(4,m_dvpH/2); glVertex2d(m_dvpW/2-4,m_dvpH/2);
	glEnd();

    //draw grid
    if (m_dvpGrid && m_dvpGridStep>0)
    {
        float halfOfHeight = m_dvpH/2;
        float startLine = 0;

        while (startLine < halfOfHeight)
        {
	        glColor4f(1,1,1,0.1f);
	        glBegin(GL_LINES);
	        glVertex2d(4,halfOfHeight + startLine); glVertex2d(m_dvpW/2-4,halfOfHeight + startLine);
            glVertex2d(4,halfOfHeight - startLine); glVertex2d(m_dvpW/2-4,halfOfHeight - startLine);
	        glEnd();
            startLine += m_dvpGridStep;
        }
    }

	glPopMatrix();
}

void OGLContext::RenderLogo()
{
	// use GLOrtho2DCorrection before calling this
	if (m_logoFT)
	{
		glPushMatrix();
			glTranslatef(m_width/2-100,-m_height/2+10,0);
			glCallList(OGL_DL_LOGO);
		glPopMatrix();
	}
}


bool OGLContext::IsLogoFTActive()
{
	return m_logoFT;
}



//OpenGL utility functions ================================================================ //

namespace ft {



	// projection functions ------------------------------------------------------------------ //

	void GLOrtho2DCorrection(const int width, const int height)
	{
		int w;
		int h;
		GLdouble size;
		GLdouble aspect;

		w = width;
		h = height;
		aspect = (GLdouble)w / (GLdouble)h;

		// Use the whole window.
		//glViewport(0, 0, w, h);

		// We are going to do some 2-D orthographic drawing.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		size = (GLdouble)((w >= h) ? w : h) / 2.0;
		if (w <= h)
		{
			aspect = (GLdouble)h/(GLdouble)w;
			glOrtho(-size, size, -size*aspect, size*aspect,	-100000.0, 100000.0);
		}
		else
		{
			aspect = (GLdouble)w/(GLdouble)h;
			glOrtho(-size*aspect, size*aspect, -size, size,	-100000.0, 100000.0);
		}

		// Make the world and window coordinates coincide so that 1.0 in
		// model space equals one pixel in window space.
		glScaled(aspect, aspect, 1.0);

		// Now determine where to draw things.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	/*-----  prepare shadow projection for current model    -----*/
	// Multiply the current ModelView-Matrix with a shadow-projetion
	// matrix.
	// l is the position of the light source
	// e is a point on within the plane on which the shadow is to be
	//   projected.
	// n is the normal vector of the plane.
	// Everything that is drawn after this call is "squashed" down
	// to the plane. Hint: Gray or black color and no lighting
	// looks good for shadows *g*
	void GlShadowProjection(float * l, float * e, float * n)
	{
	  float d, c;
	  float mat[16];

	  // These are c and d (corresponding to the tutorial)

	  d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
	  c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;

	  // Create the matrix. OpenGL uses column by column
	  // ordering

	  mat[0]  = l[0]*n[0]+c;
	  mat[4]  = n[1]*l[0];
	  mat[8]  = n[2]*l[0];
	  mat[12] = -l[0]*c-l[0]*d;

	  mat[1]  = n[0]*l[1];
	  mat[5]  = l[1]*n[1]+c;
	  mat[9]  = n[2]*l[1];
	  mat[13] = -l[1]*c-l[1]*d;

	  mat[2]  = n[0]*l[2];
	  mat[6]  = n[1]*l[2];
	  mat[10] = l[2]*n[2]+c;
	  mat[14] = -l[2]*c-l[2]*d;

	  mat[3]  = n[0];
	  mat[7]  = n[1];
	  mat[11] = n[2];
	  mat[15] = -d;

	  // Finally multiply the matrices together
	  glMultMatrixf(mat);
	}

	void GlShadowProjection()
	{
		float lightPosition[] ={3000,6000,0}; // Coordinates of the light source
		float normal[] = {0,-1,0};            // Normal vector for the plane
		float point[] = {0,0,0};              // Point of the plane

		GlShadowProjection(lightPosition,point,normal);
	}


	void OGLdrawCircleXZ(const float radius, const CalVector& center, const CalVector& color, const int segments, const bool filled)
	{
		const float step = (2 * Pi) / segments;
		// set drawing color
		glColor3f (color.x, color.y, color.z);
		if (filled){
			glPushAttrib (GL_ENABLE_BIT);
			glDisable (GL_CULL_FACE);
		}
		// begin drawing a triangle fan (for disk) or line loop (for circle)
		glBegin (filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
		const int vertexCount = filled ? segments+1 : segments;
		float _x, _z;
		for (int i = 0; i < vertexCount; i++)
		{
			_x = cos(step*i)*radius + center.x;
			_z = sin(step*i)*radius + center.z;
			glVertex3f(_x,1,_z);
		}
		glEnd();
		if (filled) glPopAttrib();
	}

	void OGLdrawArcXZ(const CalVector &start, const CalVector& center, const CalVector& color, const float alpha,
					  const float arcLength, const int segments, const bool filled)
	{



		// determine the angular step per segment
		const float radius = start.length ();
		const float twoPi = 2 * Pi;
		const float circumference = twoPi * radius;
		const float arcAngle = twoPi* arcLength / circumference;
		const float step = arcAngle / segments;

		CalVector _ox = CalVector(1,0,0);
		CalVector _start = start;
		_start.normalize();
		float _startAngValue = UTIL_GetVectorsAngle(_start, _ox);


//		const float step = (2 * Pi) / segments;
		// set drawing color
		glColor4f (color.x, color.y, color.z, alpha);
		if (filled){
			glPushAttrib (GL_ENABLE_BIT);
			glDisable (GL_CULL_FACE);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// begin drawing a triangle fan (for disk) or line loop (for circle)
		glBegin (filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
		const int vertexCount = filled ? segments+1 : segments;
		float _x, _z;
		for (int i = 0; i < vertexCount; i++)
		{
			_x = cos(_startAngValue+step*i)*radius + center.x;
			_z = sin(_startAngValue+step*i)*radius + center.z;
			glVertex3f(_x,1,_z);
		}
		glVertex3f(center.x,1,center.z);
		glEnd();
		glDisable(GL_BLEND);
		if (filled) glPopAttrib();

	}

	void OGLdrawArcXZ(const CalVector &start, const CalVector &end, const CalVector& center,
					  const CalVector& color, const float alpha, const int segments, const bool filled)
	{

		CalVector _ox = CalVector(1,0,0);
		CalVector _start = start;
		CalVector _end = end;
		_start.normalize();
		_end.normalize();
		float _startAngValue = UTIL_GetVectorsAngle(_start, _ox);
		float sign = UTIL_GetSignForDirChange(_ox, _start);
		if (sign==-1)
			_startAngValue = 2*Pi - _startAngValue;

		float _angleStartEndValue = UTIL_GetVectorsAngle(_start,_end);
		sign = UTIL_GetSignForDirChange(_start,_end);
		// determine the angular step per segment
		if (sign==-1)
			_angleStartEndValue = 2*Pi - _angleStartEndValue;

		const float radius = start.length ();
		//const float twoPi = 2 * Pi;
		//const float circumference = twoPi * radius;
		//const float arcAngle = twoPi* arcLength / circumference;
		//const float step = arcAngle / segments;

		const float step = _angleStartEndValue / segments;
		// set drawing color
		glColor4f (color.x, color.y, color.z,alpha);
		if (filled){
			glPushAttrib (GL_ENABLE_BIT);
			glDisable (GL_CULL_FACE);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		// begin drawing a triangle fan (for disk) or line loop (for circle)
		glBegin (filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
		const int vertexCount = filled ? segments+1 : segments;
		float _x, _z;
		glVertex3f(center.x,1,center.z);
		for (int i = 0; i < vertexCount; i++)
		{
			_x = cos(_startAngValue+step*i)*radius + center.x;
			_z = sin(_startAngValue+step*i)*radius + center.z;
			glVertex3f(_x,1,_z);
		}

//			_x = cos(_startAngValue)*radius + center.x;
//			_z = sin(_startAngValue)*radius + center.z;
//			glVertex3f(_x,1,_z);

		glEnd();
		glDisable(GL_BLEND);
		if (filled) glPopAttrib();
	}

	// text functions ------------------------------------------------------------------ //
	//internal helpers
   inline GLint begin2dDrawing (void)
    {
        // store OpenGL matrix mode
        GLint originalMatrixMode;
        glGetIntegerv (GL_MATRIX_MODE, &originalMatrixMode);

        // clear projection transform
        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();

        // set up orthogonal projection onto window's screen space
        const float w = glutGet (GLUT_WINDOW_WIDTH);
        const float h = glutGet (GLUT_WINDOW_HEIGHT);
        glOrtho (0.0f, w, 0.0f, h, -1.0f, 1.0f);

        // clear model transform
        glMatrixMode (GL_MODELVIEW);
        glPushMatrix ();
        glLoadIdentity ();

        // return original matrix mode for saving (stacking)
        return originalMatrixMode;
    }


    inline void end2dDrawing (GLint originalMatrixMode)
    {
        // restore previous model/projection transformation state
        glPopMatrix ();
        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();

        // restore OpenGL matrix mode
        glMatrixMode (originalMatrixMode);
    }


	void OGLdraw2DTextAt3D(const char& text, const CalVector& location, const CalVector& color)
	{
		// set text color and raster position
		glEnable(GL_LINE_SMOOTH);
		glColor3f (color.x, color.y, color.z);
		glRasterPos3f (location.x, location.y, location.z);

		// switch into 2d screen space in case we need to handle a new-line
		GLint rasterPosition[4];
		glGetIntegerv (GL_CURRENT_RASTER_POSITION, rasterPosition);
		const GLint originalMatrixMode = begin2dDrawing ();

		// loop over each character in string (until null terminator)
		int lines = 0;
		for (const char* p = &text; *p; p++)
		{
			if (*p == '\n')
			{
				// handle new-line character, reset raster position
				lines++;
				const int fontHeight = 13; // for GLUT_BITMAP_9_BY_15
				const int vOffset = lines * (fontHeight + 1);
				glRasterPos2i (rasterPosition[0], rasterPosition[1] - vOffset);
			}
			else
			{
				// otherwise draw character bitmap
				glutBitmapCharacter (GLUT_BITMAP_8_BY_13, *p);
			}
		}

		// switch back out of 2d screen space
		end2dDrawing (originalMatrixMode);
		glDisable(GL_LINE_SMOOTH);
	}

	void OGLdraw2DTextAt3D(const std::ostringstream& text, const CalVector& location, const CalVector& color)
	{
		OGLdraw2DTextAt3D(*text.str().c_str(), location, color);
	}


	void OGLWriteBitmap(int font, int x, int y, const char *text)
	{
		int len, i;
		if ((font>6)||(font<0)) font = 4;
		glRasterPos2f(x, y);
		len = (int) strlen(text);
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(bitmap_fonts[font], text[i]);
		}
	}

	void OGLWriteStroke(int x, int y, const char *text)
	{
		int i, len;
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScaled(0.1,0.1,0.1);
		len = (int) strlen(text);
		for (i = 0; i < len; i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
		}
		glPopMatrix();
	}


	// shader functions ------------------------------------------------------------------ //
	GLuint loadVertexProgram(const std::string fn)
	{
        GLuint vp;
        string str;

        ifstream in(fn.c_str(), ifstream::in);

        if(!in.good())
        {
            cerr << "Error opening " << fn << endl;
            return false;
        }

        const int bufsize = 100;
        char buf[bufsize];

        do
        {
            in.read(buf, bufsize);
            str.append(buf, in.gcount());
        }
        while(!in.eof());

        in.close();

        if (Debug::RENDER>0)
			_dbg << "Loading vertex program: " << fn << std::endl;

        glGenProgramsARB( 1,&vp);
        GLenum err = glGetError();
        // mka commented because it gives error on ATI cards (why?)
        //if (err != GL_NO_ERROR)
        // {
        //     cerr << "glProgramStringARB precheck failed:"
        //          << gluErrorString(err) << endl;
        //     return false;
        // }

        glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                                (GLsizei)str.size(), str.c_str());
        err = glGetError();

        if(err == GL_INVALID_OPERATION)
        {
            // print compiler error
            GLint pos;
            glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &pos);

            cerr << "Error " << glGetString(GL_PROGRAM_ERROR_STRING_ARB)
                 << " at " << pos << " compiling vertex program!" << endl;

            int start = 0, end, line = 0;

            for(end = 0; end < static_cast<int>(str.size()); ++end)
            {
                if(str[end] == '\n')
                {
                    ++line;
                    if (pos < end)
                    break;

                    start = end + 1;
                }
            }

            std::string mesg;
            std::string loc;

            for(int i = start; i < end; ++i)
            {
                if (i == pos)
                    loc += '^';
                else if (i < pos)
                    loc += '-';
                else
                    loc += ' ';
                mesg += str[i];
            }

            cerr << mesg << endl << loc << endl;
            return false;
        }
        else if (err != GL_NO_ERROR) {
            cerr << "glProgramStringARB postcheck failed:"
                 << gluErrorString(err) << endl;
            return false;
        }

        glBindProgramARB( GL_VERTEX_PROGRAM_ARB, vp);

		if (Debug::RENDER>0)
            _dbg << " Vertex Program - Load succeeded\n"<< std::endl;

	   return vp;
	}


	GLuint loadFragmentProgram(const std::string fn)
	{
        GLuint fp;
        string str;

        ifstream in(fn.c_str(), ifstream::in);

        if(!in.good())
        {
            cerr << "Error opening " << fn << endl;
            return false;
        }

        const int bufsize = 100;
        char buf[bufsize];

        do
        {
            in.read(buf, bufsize);
            str.append(buf, in.gcount());
        }
        while(!in.eof());

        in.close();

        if (Debug::RENDER>0)
			_dbg << "Loading fragment program: " << fn << std::endl;

        glGenProgramsARB( 1,&fp);
        GLenum err = glGetError();
        // mka commented because it gives error on ATI cards (why?)
        //if (err != GL_NO_ERROR)
        // {
        //     cerr << "glProgramStringARB precheck failed:"
        //          << gluErrorString(err) << endl;
        //     return false;
        // }

        glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                                (GLsizei)str.size(), str.c_str());
        err = glGetError();

        if(err == GL_INVALID_OPERATION)
        {
            // print compiler error
            GLint pos;
            glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &pos);

            cerr << "Error " << glGetString(GL_PROGRAM_ERROR_STRING_ARB)
                 << " at " << pos << " compiling fragment program!" << endl;

            int start = 0, end, line = 0;

            for(end = 0; end < static_cast<int>(str.size()); ++end)
            {
                if(str[end] == '\n')
                {
                    ++line;
                    if (pos < end)
                    break;

                    start = end + 1;
                }
            }

            std::string mesg;
            std::string loc;

            for(int i = start; i < end; ++i)
            {
                if (i == pos)
                    loc += '^';
                else if (i < pos)
                    loc += '-';
                else
                    loc += ' ';
                mesg += str[i];
            }

            cerr << mesg << endl << loc << endl;
            return false;
        }
        else if (err != GL_NO_ERROR) {
            cerr << "glProgramStringARB postcheck failed:"
                 << gluErrorString(err) << endl;
            return false;
        }

        glBindProgramARB( GL_VERTEX_PROGRAM_ARB, fp);

		if (Debug::RENDER>0)
            _dbg << " Fragment Program - Load succeeded" << std::endl;

	   return fp;
	}

};
