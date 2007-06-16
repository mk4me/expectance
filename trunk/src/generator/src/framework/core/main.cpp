//----------------------------------------------------------------------------//
// main.cpp                                                                   //
// Copyright (C) 2007 Future Today                                            //
// based on Bruno 'Beosil' Heidelberger code                                  //
//----------------------------------------------------------------------------//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "global.h"
#include "../scene/viewer.h"
//#include "../scene/primitiveManager.h"
#include "application.h"

/*----- GLUT callback functions  -----*/                                        
void DisplayFunc()
{
  // render the scene
  theViewer.OnRender();

}

void ExitFunc()
{
  // shut the viewer instance down
  theViewer.OnShutdown();
}

void IdleFunc()
{
  // redirect to the viewer instance
  theViewer.OnIdle();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
  // redirect the message to the viewer instance
  theViewer.OnKey(key, x, theViewer.GetHeight() - y - 1);
}

void SpecialFunc(int key, int x, int y)
{
  theViewer.OnSpecial(key, x, theViewer.GetHeight() - y - 1);
}

void MotionFunc(int x, int y)
{
  // redirect the message to the viewer instance
  theViewer.OnMouseMove(x, theViewer.GetHeight() - y - 1);
}

void MouseFunc(int button, int state, int x, int y)
{
  // redirect the message to the viewer instance
  if(state == GLUT_DOWN)
  {
    theViewer.OnMouseButtonDown(button, x, theViewer.GetHeight() - y - 1);
  }
  else if(state == GLUT_UP)
  {
    theViewer.OnMouseButtonUp(button, x, theViewer.GetHeight() - y - 1);
  }
}

void ReshapeFunc(int width, int height)
{
  // set the new width/height values
  theViewer.SetDimension(width, height);
}

/*----- Main entry point of the application -----*/
int main(int argc, char *argv[])
{
  // initialize the GLUT system
  glutInit(&argc, argv);

  // create our view instance
  if(!theViewer.OnCreate(argc, argv))
  {
    std::cerr << "Creation of the viewer failed." << std::endl;
    return -1;
  }

  // register our own exit callback
  atexit(ExitFunc);

  // set all GLUT modes
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(theViewer.GetWidth(), theViewer.GetHeight());
  glutCreateWindow("Arena (c) FToday 2007");
  if(theViewer.GetFullScreen()) glutFullScreen();
  glutSetCursor(GLUT_CURSOR_NONE);

  // register all GLUT callback functions
  glutIdleFunc(IdleFunc);
  glutMouseFunc(MouseFunc);
  glutMotionFunc(MotionFunc);
  glutPassiveMotionFunc(MotionFunc);
  glutReshapeFunc(ReshapeFunc);
  glutDisplayFunc(DisplayFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutSpecialFunc(SpecialFunc);
  
  // initialize our viewer instance
  if(!theViewer.OnInit())
  {
    std::cerr << "Initialization of the viewer failed." << std::endl;
    return -1;
  }

    ft::Application::getInstance()->InitModules();
    ft::Application::getInstance()->InitSceneObjects();

  // run the GLUT message loop
  glutMainLoop();

  return 0;
}