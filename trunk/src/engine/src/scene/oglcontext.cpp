/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "oglcontext.h"
using namespace ft;

OGLContext* OGLContext::m_instance = NULL;

OGLContext* OGLContext::getInstance()
{
    if (m_instance == NULL)
    {
        _dbg << "OGLContext::getInstace(): instance of OGLContext created " << endl;
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

	m_yscope = abs(m_ymin) + abs(m_ymax);
	m_dvpW = m_width;
	m_dvpH = m_height/4;
	SetDataViewportLegend(-100,100);
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

//void OGLContext::InitCursorDL()
//{
//	glNewList(5,GL_COMPILE);
//		// switch to orthogonal projection for the cursor
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glOrtho(0, (GLdouble)m_width, 0, (GLdouble)m_height, -1.0f, 1.0f);
//
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//
//		// render the cursor
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//		glBegin(GL_TRIANGLES);
//			////glColor4f(1.0f, 1.0f, 1.0f, 1.0f); glVertex2i(m_mouseX, m_mouseY);
//			////glColor4f(1.0f, 1.0f, 1.0f, 0.2f); glVertex2i(m_mouseX + 16, m_mouseY - 32);
//			////glColor4f(1.0f, 1.0f, 1.0f, 0.2f); glVertex2i(m_mouseX + 32, m_mouseY - 16);
//		glEnd();
//
//		glDisable(GL_BLEND);
//	glEndList();
//}

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
		////glEnable(GL_ALPHA_TEST); //alternative method
		////glAlphaFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		if ((logoTexture = ft::TextureManager::getInstance()-> LoadTexture(FT_TEXTUREPATH + Config::getInstance()->GetStrVal("logo_texture")))==0)
			return false;

		glBindTexture(GL_TEXTURE_2D,logoTexture);
		glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(_width, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(_width, _height);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, _height);
	    glEnd();

		////glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	glEndList();
	return true;
}

//void OGLContext::InitRendering()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, 1, 100000);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//
//}


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
	glViewport(0, 0, m_dvpW, m_dvpH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, m_dvpW, 0, m_dvpH);
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

    // commented by abak 17-12-2007
	//draw axis
	//int hdiv = m_dvpH/5;
	//for(int i = 1; i < 5; i++)
	//{
	//	glColor4f(1,1,1,0.1f);
	//	glBegin(GL_LINES);
	//	glVertex2d(4,hdiv*i); glVertex2d(m_dvpW/2-4,hdiv*i);
	//	glEnd();
	//	//get apropriate text
	//	glEnable(GL_LINE_SMOOTH);
	//	glColor4f(1,1,1,0.5f);
	//	OGLContext::getInstance()->OGLWriteBitmap(1,1, hdiv*i-5, m_dvpLegend[i-1].c_str());
	//	glDisable(GL_LINE_SMOOTH);

	//}
	glPopMatrix();
}

void OGLContext::SetDataViewportLegend(const float y_min, const float y_max)
{
	float scope  = y_max - y_min;
	float scope_5 = scope/5.0f;
	float res = 0.0f;
	stringstream s, s1;
	for (int i = 1; i<5; i++)
	{
		res = y_min+scope_5*i;
		
		s << res;
		//m_dvpLegend[i-1].precision(5);
		if (res >= 0)
			m_dvpLegend[i-1] = " " + s.str().substr(0,8);
		else
			m_dvpLegend[i-1] = s.str().substr(0,8);
	}
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

	//glPushMatrix();
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	glEnable(GL_BLEND);
	//	glEnable(GL_LINE_SMOOTH);
	//	//glLineWidth(2.0);
	//	glColor4f(1,1,1,0.1f);
	//	//OGLWriteStroke(-m_width/-10, -m_height/2+20, "This is antialiased.");
	//	OGLWriteBitmap(6,-m_width/2+5, -m_height/2+65, "This is antialiased.");
	//	glDisable(GL_LINE_SMOOTH);
	//	glDisable(GL_BLEND);
	//glPopMatrix();

}

void OGLContext::GLOrtho2DCorrection()
{
	int w;
	int h;
	GLdouble size;
	GLdouble aspect;

	w = m_width;
	h = m_height;
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
void OGLContext::GlShadowProjection(float * l, float * e, float * n)
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

void OGLContext::GlShadowProjection()
{
    float lightPosition[] ={3000,6000,0}; // Coordinates of the light source
	float normal[] = {0,-1,0};            // Normal vector for the plane
    float point[] = {0,0,0};              // Point of the plane

	GlShadowProjection(lightPosition,point,normal);
}

//void OGLContext::RenderInfo(const char *text)
//{
//	int _x,_y;
//	_x = getWidth() / 2;
//	_y = getHeight() / 2;
//	static float _alpha = 0.0f;
//	static float _p = 0.0f;
//	glPushMatrix();
//		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//		glEnable(GL_BLEND);
//		glTranslatef(-_x,-_y,0);
//		
//		if ((_alpha < 1)&&(_p<1)) _alpha+=0.001f;
//		else if (_alpha == 1) 
//		{
//			_p+=0.001f;
//			if(_p == 1)
//			{
//				_alpha+=0.1f;
//			}
//		}
//		else if ((_alpha > 1.0f)&&(_p==1)) _alpha-=0.001f;
//
//		//get apropriate text
//		glEnable(GL_LINE_SMOOTH);
//			glColor4f(1.0f,0.0f,0.0f,_alpha);
//			OGLContext::getInstance()->OGLWriteBitmap(5,5, _y-50, text);
//		glDisable(GL_LINE_SMOOTH);
//
//		glDisable(GL_BLEND);
//	glPopMatrix();
//
////} TODO MKA DELETE IT - now for alpha timing purpose.

void OGLContext::OGLWriteBitmap(int font, int x, int y, const char *text)
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

void OGLContext::OGLWriteStroke(int x, int y, const char *text)
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

bool OGLContext::IsLogoFTActive()
{
	return m_logoFT;
}

GLuint OGLContext::loadVertexProgram(const std::string fn)
{
   FILE *fp;
   GLubyte *buf;
   int length;
   bool ret = true;
   GLuint vp;

   if (!(fp = fopen(fn.c_str(),"rb")))
   {
      return false;
   }
   
   if (Debug::RENDER>0)
        _dbg << "Loading vertex program: " << fn << std::endl;
   
   fseek(fp, 0, SEEK_END);
   length = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   buf = new GLubyte[length+1];

   fread( buf, 1, length, fp);
   buf[length] = '\0'; // make it a regular C string so str* functions work

   glGenProgramsARB( 1, &vp);
   glBindProgramARB( GL_VERTEX_PROGRAM_ARB, vp);

   glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, length, buf);

   if (glGetError() != 0)
   {
      int lineno;
      printf("%s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB));
      glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &lineno);
      printf(" @%d\n", lineno);
      ret = false;
   }
   else
   {
      if (Debug::RENDER>0)
        printf(" Vertex Program - Load succeeded\n");

	  glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );
   }

   fclose(fp);

   delete []buf;
   return ret ? vp :0;
}


GLuint OGLContext::loadFragmentProgram(const std::string fn)
{
   FILE *fp;
   GLubyte *buf;
   int length;
   bool ret = true;
   GLuint rp;

   if (!(fp = fopen(fn.c_str(),"rb")))
   {
      return false;
   }

   printf("\nLoading fragment program: '%s'\n", fn);

   fseek(fp, 0, SEEK_END);
   length = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   buf = new GLubyte[length+1];

   fread( buf, 1, length, fp);
   buf[length] = '\0'; // make it a regular C string so str* functions work

   glGenProgramsARB( 1, &rp);
   glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, rp);

   glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, length, buf);

   if (glGetError() != 0)
   {
      int position;
      printf("%s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB));
      glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &position);
      printf(" @%d\n", position);
      ret = false;
   }
   else
   {
      printf(" Fragment Program - Load succeeded\n");
	  glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, 0 );
   }

   fclose(fp);

   delete []buf;
   return ret ? rp : 0;
}