/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka, abak
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "global.h"
#include "application.h"
#include "config.h"
#include "inputmanager.h"
#include "../scene/scenemanager.h"

using namespace ft;

/*----- GLUT callback functions  -----*/                                        
void DisplayFunc()
{
	SceneManager::getInstance()->OnRender(); 
}

void ExitFunc()
{
   // shut down OGL Content
   SceneManager::getInstance()->CleanUp();
}

void IdleFunc()
{
  // redirect to the viewer instance
  //theViewer.OnIdle(); //sman or contr man
  
  //update ControlManager
  ControlManager::getInstance()->OnUpdate();
  glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    InputManager::getInstance()->OnKey(key, x,  OGLContext::getInstance()->getHeight() - y - 1);
}

void SpecialFunc(int key, int x, int y)
{
	InputManager::getInstance()->OnSpecial(key, x, OGLContext::getInstance()->getHeight() - y - 1);
}

void MotionFunc(int x, int y)
{
	InputManager::getInstance()->OnMouseMove(x, OGLContext::getInstance()->getHeight() - y - 1);
}

void MouseFunc(int button, int state, int x, int y)
{
  if(state == GLUT_DOWN)
  {
    InputManager::getInstance()->OnMouseButtonDown(button, x, OGLContext::getInstance()->getHeight() - y - 1);
  }
  else if(state == GLUT_UP)
  {
    InputManager::getInstance()->OnMouseButtonUp(button, x, OGLContext::getInstance()->getHeight() - y - 1);
  }
}

void ReshapeFunc(int width, int height)
{
	OGLContext::getInstance()->setWindowSize(width, height);
}


int main(int argc, char *argv[])
{
	ft::Application::getInstance()->InitConfig();
	// initialize the GLUT system
	glutInit(&argc, argv);
	atexit(ExitFunc);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(glutGet (GLUT_SCREEN_WIDTH), (glutGet (GLUT_SCREEN_HEIGHT))/2);
	glutCreateWindow(Config::getInstance()->GetStrVal("application_name"));

	if (Config::getInstance()->GetIntVal("full_screen") == 1)
	{
		glutFullScreen();
	}
	//TODOMKA glutSetCursor(GLUT_CURSOR_NONE);

	// register all GLUT callback functions
	glutIdleFunc(IdleFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutPassiveMotionFunc(MotionFunc);
	glutReshapeFunc(ReshapeFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);

	if(!ft::SceneManager::getInstance()->Init())
	{
		std::cerr << "Initialization of the Arena failed." << std::endl;
		return -1;
	}

	ft::Application::getInstance()->InitModules();
	ft::Application::getInstance()->InitSceneObjects();

	// run the GLUT message loop
	glutMainLoop();

	return 0;
}