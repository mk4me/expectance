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
#include "../scene/visualizationmanager.h"
#include "../utility/debug.h"


using namespace ft;

/// \brief GLUT callback function
void DisplayFunc()
{
	VisualizationManager::getInstance()->OnRender(); 
}

/// \brief GLUT callback function
void ExitFunc()
{
   // shut down OGL Content
   VisualizationManager::getInstance()->CleanUp();
   Application::DestroyInstance();
   Debug::Destroy();
}

/// \brief GLUT callback function
void IdleFunc()
{
  // redirect to the viewer instance
  //theViewer.OnIdle(); //sman or contr man
  
  //update UpdateManager
  UpdateManager::getInstance()->OnUpdate();
  glutPostRedisplay();
}

/// \brief GLUT callback function
void KeyboardFunc(unsigned char key, int x, int y)
{
    InputManager::getInstance()->OnKey(key, x,  OGLContext::getInstance()->getHeight() - y - 1);
}

/// \brief GLUT callback function
void SpecialFunc(int key, int x, int y)
{
	InputManager::getInstance()->OnSpecial(key, x, OGLContext::getInstance()->getHeight() - y - 1);
}

/// \brief GLUT callback function
void MotionFunc(int x, int y)
{
	InputManager::getInstance()->OnMouseMove(x, OGLContext::getInstance()->getHeight() - y - 1);
}

/// \brief GLUT callback function
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


/// \brief GLUT callback function
void ReshapeFunc(int width, int height)
{
	OGLContext::getInstance()->setWindowSize(width, height);
}


/**
 * \brief Main entry to application
 *
 * \param int argc - number of arguments
 * \param char *[] argv - list of arguments
 * \return int - result of application
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

    if (!Application::getInstance()->InitModules())
	{
        cout << Debug::ERR_STR << "Application::InitModules error" << endl;
		return -1;
	}

	VisualizationManager::getInstance()->AddObject(new TraceLine("TL1")); //lines for tracing
	VisualizationManager::getInstance()->AddObject(new TraceLine("TL2"));
	VisualizationManager::getInstance()->AddObject(new TraceLine("TL3"));	

    Application::getInstance()->InitAvatars();
    Application::getInstance()->SetCameraToActiveAvatar();
    Application::getInstance()->InitStaticObjects();
    Application::getInstance()->StartAISimulation();
	


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