/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka, abak
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "global.h"
#include "glutapp.h"
#include "application.h"

#include "inputmanager.h"
#include "updatemanager.h"
#include "../scene/visualizationmanager.h"
#include "../scene/oglcontext.h"

#include "lockmanager.h"

using namespace ft;

/// \brief GLUT callback function
void DisplayFunc()
{
    LockManager::getUpdateLock()->enter();
	VisualizationManager::getInstance()->OnRender(); 
    LockManager::getUpdateLock()->leave();
}

/// \brief GLUT callback function
void ExitFunc()
{
   // shut down OGL Content
   LockManager::getUpdateLock()->enter();
   VisualizationManager::getInstance()->CleanUp();
   Debug::Destroy();
   LockManager::getUpdateLock()->leave();
}

/// \brief GLUT callback function
void IdleFunc()
{
  LockManager::getUpdateLock()->enter();
  // redirect to the viewer instance
  //theViewer.OnIdle(); //sman or contr man
  
  //update UpdateManager
  UpdateManager::getInstance()->OnUpdate();
  glutPostRedisplay();

  LockManager::getUpdateLock()->leave();
}

/// \brief GLUT callback function
void KeyboardFunc(unsigned char key, int x, int y)
{
    LockManager::getUpdateLock()->enter();
    InputManager::getInstance()->OnKey(key, x,  OGLContext::getInstance()->getHeight() - y - 1);
    LockManager::getUpdateLock()->leave();
}

/// \brief GLUT callback function
void SpecialFunc(int key, int x, int y)
{
    LockManager::getUpdateLock()->enter();
	InputManager::getInstance()->OnSpecial(key, x, OGLContext::getInstance()->getHeight() - y - 1);
    LockManager::getUpdateLock()->leave();
}

/// \brief GLUT callback function
void MotionFunc(int x, int y)
{
    LockManager::getUpdateLock()->enter();
	InputManager::getInstance()->OnMouseMove(x, OGLContext::getInstance()->getHeight() - y - 1);
    LockManager::getUpdateLock()->leave();
}

/// \brief GLUT callback function
void MouseFunc(int button, int state, int x, int y)
{
  LockManager::getUpdateLock()->enter();
  if(state == GLUT_DOWN)
  {
    InputManager::getInstance()->OnMouseButtonDown(button, x, OGLContext::getInstance()->getHeight() - y - 1);
  }
  else if(state == GLUT_UP)
  {
    InputManager::getInstance()->OnMouseButtonUp(button, x, OGLContext::getInstance()->getHeight() - y - 1);
  }
  LockManager::getUpdateLock()->leave();
}


/// \brief GLUT callback function
void ReshapeFunc(int width, int height)
{
    LockManager::getUpdateLock()->enter();
	OGLContext::getInstance()->setWindowSize(width, height);
    LockManager::getUpdateLock()->leave();
}

/**
 * \brief Main entry to application
 *
 * \param int argc - number of arguments
 * \param char *[] argv - list of arguments
 * \return int - result of application
 **/
int InitGlutApplication(int argc, char *argv[], Application *app)
{
    LockManager::getUpdateLock()->enter();

	app->InitConfig();
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

    if (app->Init() < 0)
    {
        LockManager::getUpdateLock()->leave();
        return -1;
    }
    
    LockManager::getUpdateLock()->leave();

	// run the GLUT message loop
	glutMainLoop();

	return 0;
}






/*-------------------------------------------- some examples of markers use -------------------------------------*/
//	Line *ln, *ln1, *ln2, *ln3;
//	ln  = new Line("Linia0"); 
//	ln1 = new Line(CalVector(100,100,100), CalVector(30,40,10), "Linia I");
//	ln1->setStart(CalVector(0,0,0)).setEnd(CalVector(100,200,200)).setColor(CalVector(1,0,0));
//	
//	TraceLine *tn;
//	tn = new TraceLine("TL1");
//    tn->AddPoint(CalVector(0,0,0));
//    tn->AddPoint(CalVector(10,80,10));
//    tn->AddPoint(CalVector(100,200,100));
//    tn->AddPoint(CalVector(100,400,100));
//	//tn->HideMarker();
//	//tn->ClearTrace();
//	ft::VisualizationManager::getInstance()->AddObject(tn);
//
//	ft::VisualizationManager::getInstance()->AddObject(ln);
//	ft::VisualizationManager::getInstance()->AddObject(ln1);
//    ln2 = new Line(CalVector(100,100,100), CalVector(30,40,10), 100, "LiniaII");
//
//
//	ft::VisualizationManager::getInstance()->AddObject(ln2);
//    
//    ln3 = new Line("LiniaIII");
//	ft::VisualizationManager::getInstance()->AddObject(ln3);
//	
//	ln1->setColor(CalVector(1,1,0));
//	ln->setStart(CalVector(100,0,0)).setEnd(CalVector(400,0,0)).setColor(CalVector(0,0,1));
////	ln2 = dynamic_cast<Line*>(VisualizationManager::getInstance()->getObject("LiniaII"));
//	ln2->setColor(CalVector(0,1,0));
////	ln3 = dynamic_cast<Line*>(VisualizationManager::getInstance()->getObject("LiniaIII"));
//	ln3->setStart(CalVector(0,100,0)).setEnd(CalVector(200,100,0)).setColor(CalVector(1,1,0));
//	ln1->Hide();
//	ln1->setColor(CalVector(0,1,1));
//	ln1->Show();
//	ln1->setArrow(true);
/*-------------------------------------------- some examples of markers use finish -------------------------------------*/
