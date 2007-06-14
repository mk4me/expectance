//----------------------------------------------------------------------------//
// viewer.h                                                                   //
// Copyright (C) 2007 Future Today                                            //
// based on Bruno 'Beosil' Heidelberger code                                  //
//----------------------------------------------------------------------------//

#ifndef VIEWER_H
#define VIEWER_H



#include "../core/global.h"



class Viewer
{
// member variables
protected:
  int m_width;
  int m_height;
  bool m_bFullscreen;
  float m_pitchAngle; // OX
  float m_yawAngle;   // OY
  float m_rollAngle;  // OZ
  float m_distance;
  float m_camheight;
  int m_mouseX;
  int m_mouseY;
  bool m_bLeftMouseButtonDown;
  bool m_bRightMouseButtonDown;
  unsigned int m_lastTick;
  bool m_bPaused;

  CalCoreModel* m_calCoreModel;
  CalModel* m_calModel;
  float m_scale;
  int m_currentAnimationId;
  float m_leftAnimationTime;
  float m_blendTime;
  float m_lodLevel;
  int m_vertexCount;
  int m_faceCount;

  float m_fpsDuration;
  int m_fpsFrames;
  int m_fps;
  float m_timeScale;

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
  bool getFullscreen();
  int getHeight();
  int getWidth();
  bool onCreate(int argc, char *argv[]);
  void onIdle();
  bool onInit();
  void onKey(unsigned char key, int x, int y);
  void onSpecial(int key, int x, int y);
  void onMouseButtonDown(int button, int x, int y);
  void onMouseButtonUp(int button, int x, int y);
  void onMouseMove(int x, int y);
  void onRender();
  void onShutdown();
  void setDimension(int width, int height);

protected:
  GLuint loadTexture(const std::string& strFilename);
  bool parseModelConfiguration(const std::string& strFilename);

  void renderCursor();
  void renderModel(bool shadow);
  void renderModelMesh(bool shadow);
  void renderModelSkeleton(bool shadow);
  void renderModelBoundingBox(bool shadow);
  void renderScene();
  bool prepareResources();
  void initNormalFloor();
  void initTexturedFloor(unsigned int textureNumber);
  void glShadowProjection(float * l, float * e, float * n);
};

extern Viewer theViewer;

#endif

//----------------------------------------------------------------------------//
