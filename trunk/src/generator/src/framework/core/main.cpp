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
/**
 * DisplayFunc: <describe the responsibilities and behavior of this method>
 *
 **/
void DisplayFunc()
{
	SceneManager::getInstance()->OnRender(); 
}

/**
 * ExitFunc: <describe the responsibilities and behavior of this method>
 *
 **/
void ExitFunc()
{
   // shut down OGL Content
   SceneManager::getInstance()->CleanUp();
}

/**
 * IdleFunc: <describe the responsibilities and behavior of this method>
 *
 **/
void IdleFunc()
{
  // redirect to the viewer instance
  //theViewer.OnIdle(); //sman or contr man
  
  //update ControlManager
  ControlManager::getInstance()->OnUpdate();
  glutPostRedisplay();
}

/**
 * KeyboardFunc: <describe the responsibilities and behavior of this method>
 *
 * \param unsigned char key <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void KeyboardFunc(unsigned char key, int x, int y)
{
    InputManager::getInstance()->OnKey(key, x,  OGLContext::getInstance()->getHeight() - y - 1);
}

/**
 * SpecialFunc: <describe the responsibilities and behavior of this method>
 *
 * \param int key <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void SpecialFunc(int key, int x, int y)
{
	InputManager::getInstance()->OnSpecial(key, x, OGLContext::getInstance()->getHeight() - y - 1);
}

/**
 * MotionFunc: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void MotionFunc(int x, int y)
{
	InputManager::getInstance()->OnMouseMove(x, OGLContext::getInstance()->getHeight() - y - 1);
}

/**
 * MouseFunc: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int state <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
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

/**
 * ReshapeFunc: <describe the responsibilities and behavior of this method>
 *
 * \param int width <argument description>
 * \param int height <argument description>
 **/
void ReshapeFunc(int width, int height)
{
	OGLContext::getInstance()->setWindowSize(width, height);
}


/**
 * main: <describe the responsibilities and behavior of this method>
 *
 * \param int argc <argument description>
 * \param char *[] argv <argument description>
 * \return int <describe what is returned if appropriate>
 **/
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

    Application::getInstance()->InitModules();

	SceneManager::getInstance()->AddObject(new TraceLine("TL1")); //lines for tracing
	SceneManager::getInstance()->AddObject(new TraceLine("TL2"));
	SceneManager::getInstance()->AddObject(new TraceLine("TL3"));	

    Application::getInstance()->InitAvatars();
	


	// run the GLUT message loop
	glutMainLoop();

	return 0;
}






/*-------------------------------------------- some examples of markers use -------------------------------------*/
//	Line *ln, *ln1, *ln2, *ln3;
//	ln  = new Line("Linia0"); 
//	ln1 = new Line(FTVect(100,100,100), FTVect(30,40,10), "Linia I");
//	ln1->setStart(FTVect(0,0,0)).setEnd(FTVect(100,200,200)).setColor(FTVect(1,0,0));
//	
//	TraceLine *tn;
//	tn = new TraceLine("TL1");
//    tn->AddPoint(FTVect(0,0,0));
//    tn->AddPoint(FTVect(10,80,10));
//    tn->AddPoint(FTVect(100,200,100));
//    tn->AddPoint(FTVect(100,400,100));
//	//tn->HideMarker();
//	//tn->ClearTrace();
//	ft::SceneManager::getInstance()->AddObject(tn);
//
//	ft::SceneManager::getInstance()->AddObject(ln);
//	ft::SceneManager::getInstance()->AddObject(ln1);
//    ln2 = new Line(FTVect(100,100,100), FTVect(30,40,10), 100, "LiniaII");
//
//
//	ft::SceneManager::getInstance()->AddObject(ln2);
//    
//    ln3 = new Line("LiniaIII");
//	ft::SceneManager::getInstance()->AddObject(ln3);
//	
//	ln1->setColor(FTVect(1,1,0));
//	ln->setStart(FTVect(100,0,0)).setEnd(FTVect(400,0,0)).setColor(FTVect(0,0,1));
////	ln2 = dynamic_cast<Line*>(SceneManager::getInstance()->getObject("LiniaII"));
//	ln2->setColor(FTVect(0,1,0));
////	ln3 = dynamic_cast<Line*>(SceneManager::getInstance()->getObject("LiniaIII"));
//	ln3->setStart(FTVect(0,100,0)).setEnd(FTVect(200,100,0)).setColor(FTVect(1,1,0));
//	ln1->Hide();
//	ln1->setColor(FTVect(0,1,1));
//	ln1->Show();
//	ln1->setArrow(true);
/*-------------------------------------------- some examples of markers use finish -------------------------------------*/