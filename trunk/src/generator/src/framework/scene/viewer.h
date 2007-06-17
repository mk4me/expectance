//----------------------------------------------------------------------------//
// viewer.h                                                                   //
// Copyright (C) 2007 Future Today                                            //
// based on Bruno 'Beosil' Heidelberger code                                  //
//----------------------------------------------------------------------------//

#ifndef VIEWER_H
#define VIEWER_H



#include "../core/global.h"
#include "../avatar/avatar.h"


using namespace ft;

class Viewer
{
// member variables
protected:
  int m_width;
  int m_height;

  float m_pitchAngle; // OX
  float m_yawAngle;   // OY
  float m_rollAngle;  // OZ
  float m_distance;
  float m_camLeftRight;
  float m_camUpDown;
  int m_mouseX;
  int m_mouseY;
  bool m_bLeftMouseButtonDown;
  bool m_bRightMouseButtonDown;
  bool m_bMiddleMouseButtonDown;
  float m_scale;
  int m_vertexCount;
  int m_faceCount;

  int m_renderMethod;
  int m_renderScene;
  float m_lightPosition[3]; // Coordinates of the light source
  float m_normal[3];        // Normal vector for the plane
  float m_e[3];             // Point of the plane
  TextureManager* m_textureManager;

public:
  Viewer();
  virtual ~Viewer();

// member functions
public:
  bool GetFullScreen();
  int  GetHeight();
  int  GetWidth();
  bool OnCreate(int argc, char *argv[]);
  void OnIdle();
  bool OnInit();
  void OnSpecial(int key, int x, int y);
  void OnMouseButtonDown(int button, int x, int y);
  void OnMouseButtonUp(int button, int x, int y);
  void OnMouseMove(int x, int y);
  void OnRender();
  void OnShutdown();
  void SetDimension(int width, int height);
  void ChangeSceneRenderMethod();

protected:
  void RenderCursor();
  void RenderScene();
  bool PrepareResources();
  void InitNormalFloor();
  void InitTexturedFloor(unsigned int textureNumber);
  void GlShadowProjection(float * l, float * e, float * n);
};

extern Viewer theViewer;

#endif

//----------------------------------------------------------------------------//
