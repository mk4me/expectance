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
        DBG("OGLContext::getInstace(): instance of OGLContext created ");
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


void OGLContext::setWindowSize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, m_width, m_height);
}


void OGLContext::Init()
{
	m_width = glutGet (GLUT_SCREEN_WIDTH);
	m_height = (glutGet (GLUT_SCREEN_HEIGHT))/2;
	m_floorType = 0;

	GLfloat light_ambient[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Enable light 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
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
	_width = 112;
	_height = 40;

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

void OGLContext::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, 0.01, 1000000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set camera position
	glTranslatef(0.0f, Camera::getInstance()->getCamUpDown(), -Camera::getInstance()->getDistance());
	glRotatef(Camera::getInstance()->getPitchAngle(), 1.0f, 0.0f, 0.0f);
	glRotatef(Camera::getInstance()->getYawAngle(), 0.0f, 1.0f, 0.0f);



	//render floor
	glPushMatrix();
		if (m_floorType == 0)
			glCallList(OGL_DL_FLOOR);

		if (m_floorType == 1)
		{
			glCallList(OGL_DL_TEXTURED_FLOOR);
			glDisable(GL_TEXTURE_2D);
		}
	glPopMatrix();

	glEnable(GL_CULL_FACE);

}

void OGLContext::RenderLogo()
{
	// must be accesible in the global scope for 2D stuff
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (GLdouble)m_width, 0, (GLdouble)m_height, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
		glTranslatef(m_width-122,10,0);
		glCallList(OGL_DL_LOGO);
	glPopMatrix();
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