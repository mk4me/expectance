# (c) by Future Today 2008
project( engine )

set(SOURCES 
  src/core/application.cpp
  src/core/baseobject.cpp
  src/core/config.cpp
  src/core/globalmsgsender.cpp
  src/core/glutapp.cpp
  src/core/inputmanager.cpp
  src/core/message.cpp
  src/core/msgsender.cpp
  src/core/propmanager.cpp
  src/core/simulation.cpp
  src/core/updatemanager.cpp
  src/core/updateobject.cpp
  src/core/lockmanager.cpp
  src/scene/glew.c
  src/scene/oglcontext.cpp
  src/scene/renderobject.cpp
  src/scene/sceneobject.cpp
  src/scene/camera.cpp
  src/scene/cameraconfiguration.cpp
  src/scene/cameramanager.cpp
  src/scene/scenemanager.cpp
  src/scene/texturemanager.cpp
  src/scene/transformmanager.cpp
  src/scene/visualizationmanager.cpp
  src/scene/circle.cpp
  src/scene/circulararc.cpp
  src/scene/datacollector.cpp
  src/scene/line.cpp
  src/scene/spline.cpp
  src/scene/traceline.cpp
  src/scene/cal3dobject.cpp
  src/scene/cal3dobjectfactory.cpp
  src/scene/cal3dtype.cpp
  src/scene/transform.cpp
  src/ui/menuitem.cpp
  src/ui/menumanager.cpp
  src/utility/debug.cpp
  src/utility/mathutil.cpp
  src/utility/mtxlib.cpp
  src/utility/quat.cpp
  src/utility/randomgen.cpp
  src/utility/stringhelper.cpp
  src/utility/vec.cpp
  src/utility/vishelper.cpp
)

include_directories( 
  ../
  ../cal3d
)

link_directories (
    ../../src/cal3d
     ../cal3d
)

IF (DEBUG_BUILD)

	add_definitions( 
	   -D_CONSOLE
	   -D_DEBUG
	)
        SET(CMAKE_BUILD_TYPE "Debug")

ELSE (DEBUG_BUILD)

	add_definitions( 
	   -D_CONSOLE
	   -D_NDEBUG
	)
        SET(CMAKE_BUILD_TYPE "Release")

ENDIF(DEBUG_BUILD)

add_library( engine STATIC ${SOURCES} )

target_link_libraries ( engine Cal3D )
