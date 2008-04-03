# (c) by Future Today 2008
project( Cal3D )

set(SOURCES 
  src/animation.cpp
  src/animation_action.cpp
  src/animation_cycle.cpp
  src/bone.cpp
  src/buffersource.cpp
  src/cal3d.rc
  src/cal3d_wrapper.cpp
  src/coreanimation.cpp
  src/corebone.cpp
  src/corekeyframe.cpp
  src/corematerial.cpp
  src/coremesh.cpp
  src/coremodel.cpp
  src/coremorphanimation.cpp
  src/coreskeleton.cpp
  src/coresubmesh.cpp
  src/coresubmorphtarget.cpp
  src/coretrack.cpp
  src/dualquaternion.cpp
  src/error.cpp
  src/global.cpp
  src/hardwaremodel.cpp
  src/loader.cpp
  src/matrix.cpp
  src/mesh.cpp
  src/mixer.cpp
  src/model.cpp
  src/morphtargetmixer.cpp
  src/physique.cpp
  src/physiquedualquaternion.cpp
  src/platform.cpp
  src/quaternion.cpp
  src/renderer.cpp
  src/saver.cpp
  src/skeleton.cpp
  src/springsystem.cpp
  src/streamsource.cpp
  src/submesh.cpp
  src/tinyxml.cpp
  src/tinyxmlerror.cpp
  src/tinyxmlparser.cpp
  src/vector.cpp
)

include_directories( 
  ..
)


IF (DEBUG_BUILD)

	add_definitions( 
  	  -DCAL3D_EXPORTS
	  -D_DEBUG
 	  -D_USRDLL
	)
        SET(CMAKE_BUILD_TYPE "Debug")

ELSE (DEBUG_BUILD)

	add_definitions( 
  	  -DCAL3D_EXPORTS
	  -D_NDEBUG
 	  -D_USRDLL
	)

        SET(CMAKE_BUILD_TYPE "Release")

ENDIF(DEBUG_BUILD)


add_library( Cal3D STATIC ${SOURCES} )
#add_library( Cal3D SHARED ${SOURCES} )

