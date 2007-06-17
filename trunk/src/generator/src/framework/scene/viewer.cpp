//----------------------------------------------------------------------------//
// viewer.cpp                                                                 //
// Copyright (C) 2007 Future Today                                            //
// based on Bruno 'Beosil' Heidelberger code                                  //
//----------------------------------------------------------------------------//


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include "Viewer.h"
#include "../avatar/avatarfactory.h"
#include "../scene/scenemanager.h"
#include "../core/controlmanager.h"
#include "../core/application.h"

using namespace ft;

Viewer theViewer;

Viewer::Viewer()
{
  m_width = glutGet (GLUT_SCREEN_WIDTH);
  m_height = (glutGet (GLUT_SCREEN_HEIGHT))/2;

  m_pitchAngle = 20.0f;
  m_yawAngle = 0; 
  m_distance = 800.0f;
  m_camLeftRight = 0.0f;
  m_camUpDown = 0.0f;
  m_mouseX = 0;
  m_mouseY = 0;
  m_bLeftMouseButtonDown = false;
  m_bRightMouseButtonDown = false;

  m_renderScene = 1;
  m_renderMethod = 0;
  m_scale = 1.0f;

  m_vertexCount = 0;
  m_faceCount = 0;

  m_lightPosition[0] = 3000.0; m_lightPosition[1] = 6000.0; m_lightPosition[2] = 0.0; // Coordinates of the light source
  m_normal[0] = 0.0; m_normal[1] = -1.0; m_normal[2] = 0.0;                           // Normal vector for the plane
  m_e[0] = 0.0; m_e[1] = 0.0; m_e[2] = 0.0;                                           // Point of the plane
  m_textureManager = new TextureManager();

}


Viewer::~Viewer()
{
}

int Viewer::GetHeight()
{
  return m_height;
}

int Viewer::GetWidth()
{
  return m_width;
}

/*----- Read a int from file stream (to avoid Little/Big endian issue) -----*/
int ReadInt( std::ifstream *file ) 
{
	int x = 0;
	for ( int i = 0; i < 32; i+=8 ) 
	{
		char c;
		file->read ( &c, 1 );
		x += c << i;
 	}
 	return x;
 }


/*----- Create the Viewer -----*/
bool Viewer::OnCreate(int argc, char *argv[])
{
  // show some information
  std::cout << "o----------------------------------------------------------------o" << std::endl;
  std::cout << "|                         ARENA 0.1.0		                     |" << std::endl;
  std::cout << "|                     (C) 2007 Future Today          			 |" << std::endl;
  std::cout << "o----------------------------------------------------------------o" << std::endl;

  std::cout << std::endl;

  bool bModelConfiguration;
  bModelConfiguration = false;

  // parse the command line arguments
  int arg;
  for(arg = 1; arg < argc; arg++)
  {
     // check for dimension flag
    if((strcmp(argv[arg], "--dimension") == 0) && (argc - arg > 2))
    {
      m_width = atoi(argv[++arg]);
      m_height = atoi(argv[++arg]);
      if((m_width <= 0) || (m_height <= 0))
      {
        std::cerr << "Invalid dimension!" << std::endl;
        return false;
      }
    }
    // check for help flag
    else if(strcmp(argv[arg], "--help") == 0)
    {
      std::cerr << "Usage: " << argv[0] << " [--fullscreen] [--window] [--dimension width height] [--help] model-configuration-file" << std::endl;
      return false;
    }
  }

  return true;
}

/*----- Handle an idle event -----*/
void Viewer::OnIdle()
{
  //update ControlManager
  ft::ControlManager::getInstance()->OnUpdate();
  
  // update the screen
  glutPostRedisplay();
}


/*----- Initialize the Viewer -----*/
bool Viewer::OnInit()
{
  //OGL lighting
  GLfloat light_ambient[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
  GLfloat light_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    
  Application::getInstance()->InitSceneObjects();

  //OGL Initialization mka 2007.06.10
  
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);							    // Enable Smooth Shading
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				    // Black Background
  glClearDepth(1.0f);									// Depth Buffer Setup
  glEnable(GL_DEPTH_TEST);							    // Enables Depth Testing
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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
  // load and create textures for application mka 2007.06.10
  if (!PrepareResources()) return false;

  ft::ControlManager::getInstance()->Init();
//  m_lastTick = ft::Simulation::GetTick();
  
  // we're done
  std::cout << "Initialization done." << std::endl;
  std::cout << std::endl;
  std::cout << "Quit the Viewer by pressing 'q' or ESC" << std::endl;
  std::cout << std::endl;
  
  return true;
}
/*----- prepare resources  -----*/
bool Viewer::PrepareResources()
{
	// load necessary textures - openGL context must be active otherwise TextureGeneration doesn't work
	//std::string texturePath = "..\\textures\\";
	GLuint floorTexture = m_textureManager->LoadTexture(FT_TEXTUREPATH + "grass.bmp");
	// LoadTexture(strFilename+ "grassfloor128x128.tga");
    
    // PREPARE DISPLAY LISTS
	// REMEMBER ABOUT preparing display lists before starting drawing
    InitTexturedFloor(floorTexture);
	InitNormalFloor();
    return true;
}

void Viewer::ChangeSceneRenderMethod()
{
    m_renderScene = (m_renderScene+1) % 2;
}

/*----- Handle special keys (F1, F2, UP, DOWN, etc.)   -----*/
void Viewer::OnSpecial(int key, int x, int y)
{
  switch(key) 
  {
    case GLUT_KEY_UP:
 //     m_distance -= 3.0f;
      break;
    case GLUT_KEY_DOWN:
 //     m_distance += 3.0f;
      break;
    case GLUT_KEY_RIGHT:
//      m_camheight += 3.0f;
      break;
    case GLUT_KEY_LEFT:
//      m_camheight -= 3.0f;
      break;
    default:
      break;	
  }
}

/*----- Handle a mouse button down event -----*/
void Viewer::OnMouseButtonDown(int button, int x, int y)
{
  // update mouse button states
  if(button == GLUT_LEFT_BUTTON)
  {
    m_bLeftMouseButtonDown = true;
  }

  if(button == GLUT_RIGHT_BUTTON)
  {
    m_bRightMouseButtonDown = true;
  }

  if(button == GLUT_MIDDLE_BUTTON)
  {
	m_bMiddleMouseButtonDown = true;
  }
  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}

/*----- Handle a mouse button up event -----*/
void Viewer::OnMouseButtonUp(int button, int x, int y)
{
  // update mouse button states
  if(button == GLUT_LEFT_BUTTON)
  {
    m_bLeftMouseButtonDown = false;
  }

  if(button == GLUT_RIGHT_BUTTON)
  {
    m_bRightMouseButtonDown = false;
  }

  if(button == GLUT_MIDDLE_BUTTON)
  {
	m_bMiddleMouseButtonDown = false;
  }

  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}


/*----- Handle a mouse move event -----*/
void Viewer::OnMouseMove(int x, int y)
{
  // update pitch/yaw angles
  if(m_bLeftMouseButtonDown)
  {
    // calculate new angles
    m_yawAngle += (float)(x - m_mouseX);
    m_pitchAngle -= (float)(y - m_mouseY);
  }

  // update distance
  if(m_bRightMouseButtonDown)
  {
    // calculate new distance
    m_distance -= (float)(y - m_mouseY); 
    if(m_distance < 0.0f) m_distance = 0.0f;
  }

  // update Y screen position
  if(m_bMiddleMouseButtonDown)
  {
	//calculate new Y position
	m_camUpDown +=(float)(y-m_mouseY);	
  }	
  // update internal mouse position
  m_mouseX = x;
  m_mouseY = y;
}


/*----- Handle a render event -----*/
void Viewer::OnRender()
{
  // clear the vertex and face counters
  m_vertexCount = 0;
  m_faceCount = 0;

  // clear all the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 
  // set the projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLdouble)m_width / (GLdouble)m_height, 0.01, 10000);

  // set the model transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // set camera position
  glTranslatef(0.0f, m_camUpDown, -m_distance * m_scale);
  glRotatef(m_pitchAngle, 1.0f, 0.0f, 0.0f);
  glRotatef(m_yawAngle, 0.0f, 1.0f, 0.0f);
  //glTranslatef(0.0f, 0.0f, -90.0f * m_scale);


  // glLightfv(GL_LIGHT0, GL_POSITION, l);
  RenderScene();
  glEnable(GL_CULL_FACE);


  // now we draw the shadow
  glPushMatrix();
  GlShadowProjection(m_lightPosition,m_e,m_normal);  
  glColor3f(0.0f,0.0f,0.0f);
  ft::SceneManager::getInstance()->Render(); //RenderModel(true);	
  glPopMatrix();

 // draw the object that casts the shadow
  glPushMatrix();
  //RenderModel(false);	
  ft::SceneManager::getInstance()->Render();
  glPopMatrix();

  // render the cursor
  RenderCursor();

  // swap the front- and back-buffer
  glutSwapBuffers();

  // increase frame counter
//  m_fpsFrames++;  
  ft::ControlManager::getInstance()->increraseFramesCounter();
}

/*----- Shut the Viewer down -----*/
void Viewer::OnShutdown()
{
//  delete m_calModel;
  m_textureManager->DeleteAllTextures();
}


/*------------------------------------------- VIEWER SPECIFIC METHODS --------------------------------------------------------------*/


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
void Viewer::GlShadowProjection(float * l, float * e, float * n)
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



/*----- Render the cursor -----*/
void Viewer::RenderCursor()
{
  // switch to orthogonal projection for the cursor
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, (GLdouble)m_width, 0, (GLdouble)m_height, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // render the cursor
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_TRIANGLES);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex2i(m_mouseX, m_mouseY);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glVertex2i(m_mouseX + 16, m_mouseY - 32);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glVertex2i(m_mouseX + 32, m_mouseY - 16);
  glEnd();

  glDisable(GL_BLEND);
}


/*----- Render Scene elements (floor, walls etc) -----*/
void Viewer::RenderScene()
{
	glPushMatrix();
	//glRotatef(-90,1,0,0);

	if (m_renderScene == 0)
		glCallList(1);

	if (m_renderScene == 1)
	{
		glEnable(GL_TEXTURE_2D);			   // Enable Texture Mapping
		glCallList(2);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

}
/*----- Prepare non textured floor -----*/
void Viewer::InitNormalFloor()
{
	int i;
	float lightDots[] = {0.9f,0.9f,0.36f}; 
	float lightAxes[] = {0.298f,0.298f,0.36f}; 
	float darkAxes[] =  {0.25f,0.25f,0.298f}; 
	float mainAxes[] =  {0.449f,0.449f,0.519f}; 
	int tileSize	= 128;

	glNewList(1, GL_COMPILE);
	glPushMatrix();
	for (i = -25; i <= 25; i++)
	{
		if ((i%5 == 0)&&(i!=0))
		{
			glColor3fv(lightDots);
			glBegin(GL_LINES); // drop short yellow crosses
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glVertex3f(tileSize*i+1, 0,  -1);glVertex3f(tileSize*i-1, 0,  1);
			glVertex3f(tileSize*i-1, 0,  -1);glVertex3f(tileSize*i+1, 0,  1);
			glVertex3f(-1, 0,  tileSize*i+1);glVertex3f(1, 0,  tileSize*i-1);
			glVertex3f(-1, 0,  tileSize*i-1);glVertex3f(1, 0,  tileSize*i+1);
			glEnd();
			glColor3fv(lightAxes);
		}
		else if (i == 0)
			glColor3fv(mainAxes);
		else
			glColor3fv(darkAxes);

		glBegin(GL_LINES);
		glNormal3f( 0.0f, 1.0f, 0.0f );
		//glColor3fv(darkAxes);
		glVertex3f(tileSize*i, 0,  -3200);
		glVertex3f(tileSize*i, 0,  3200);
		glVertex3f(-3200, 0, tileSize*i);
		glVertex3f(3200, 0,  tileSize*i);
		glEnd();
		
		////glBegin(GL_QUADS);
		////glNormal3f( 0.0f, 1.0f, 0.0f );
		////glColor3fv(darkAxes);
		////glVertex3f(tileSize*i, 0,  tileSize*i);
		////glVertex3f(tileSize*i, 0,  tileSize*(i+1));
		////glVertex3f(tileSize*(i+1), 0,  tileSize*(i+1));
		////glVertex3f(tileSize*(i+1), 0,  tileSize*i);
		////glEnd();

	}

	//axes
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
void  Viewer::InitTexturedFloor(unsigned int textureNumber)
{
    int tileSize = 128;
	int x, y;
	
	float fAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float fDiffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float fSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float fShine[] = { 5.0 };

	// CREATE THE DISPLAY LIST 
	glNewList(2,GL_COMPILE);
	glMaterialfv( GL_FRONT, GL_AMBIENT, fAmbient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, fDiffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, fSpecular);
	glMaterialfv( GL_FRONT, GL_SHININESS, fShine);

	glPushMatrix();
	for(x = -25; x <= 25; x++)   // for every row
	{
		for(y = -25; y <= 25; y++)   // for every column
		{
			glBindTexture(GL_TEXTURE_2D, textureNumber);            
			glBegin(GL_QUADS);
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glTexCoord2f(0.0f, 0.0f); glVertex3f(tileSize*x,  0, tileSize*y);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(tileSize*(x), 0, tileSize*(y+1));
			glTexCoord2f(1.0f, 1.0f); glVertex3f(tileSize*(x+1), 0, tileSize*(y+1));
			glTexCoord2f(0.0f, 1.0f); glVertex3f(tileSize*(x+1), 0, tileSize*(y));
			glEnd();
		}
	}
	glPopMatrix();
	glEndList();
}





/*----- Set the viewer window dimension  -----*/
void Viewer::SetDimension(int width, int height)
{
  // store new width and height values
  m_width = width;
  m_height = height;

  // set new viewport dimension
  glViewport(0, 0, m_width, m_height);
}
