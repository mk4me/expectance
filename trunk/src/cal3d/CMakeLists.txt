# (c) by Future Today 2008

SET(LIB_NAME Cal3D)

SET(SOURCE_FILES 
  cal3d/animation.cpp
  cal3d/animation_action.cpp
  cal3d/animation_cycle.cpp
  cal3d/bone.cpp
  cal3d/buffersource.cpp
  cal3d/cal3d_wrapper.cpp
  cal3d/coreanimation.cpp
  cal3d/corebone.cpp
  cal3d/corekeyframe.cpp
  cal3d/corematerial.cpp
  cal3d/coremesh.cpp
  cal3d/coremodel.cpp
  cal3d/coremorphanimation.cpp
  cal3d/coreskeleton.cpp
  cal3d/coresubmesh.cpp
  cal3d/coresubmorphtarget.cpp
  cal3d/coretrack.cpp
  cal3d/dualquaternion.cpp
  cal3d/error.cpp
  cal3d/global.cpp
  cal3d/hardwaremodel.cpp
  cal3d/loader.cpp
  cal3d/matrix.cpp
  cal3d/mesh.cpp
  cal3d/mixer.cpp
  cal3d/model.cpp
  cal3d/morphtargetmixer.cpp
  cal3d/physique.cpp
  cal3d/physiquedualquaternion.cpp
  cal3d/platform.cpp
  cal3d/quaternion.cpp
  cal3d/renderer.cpp
  cal3d/saver.cpp
  cal3d/skeleton.cpp
  cal3d/springsystem.cpp
  cal3d/streamsource.cpp
  cal3d/submesh.cpp
  cal3d/tinyxml.cpp
  cal3d/tinyxmlerror.cpp
  cal3d/tinyxmlparser.cpp
  cal3d/vector.cpp
)

SET(HEADER_FILES
  cal3d/animation_action.h
  cal3d/animation_cycle.h
  cal3d/animation.h
  cal3d/animcallback.h
  cal3d/bone.h
  cal3d/buffersource.h
  cal3d/cal3d.h
  cal3d/cal3d_wrapper.h
  cal3d/coreanimation.h
  cal3d/corebone.h
  cal3d/corekeyframe.h
  cal3d/corematerial.h
  cal3d/coremesh.h
  cal3d/coremodel.h
  cal3d/coremorphanimation.h
  cal3d/coreskeleton.h
  cal3d/coresubmesh.h
  cal3d/coresubmorphtarget.h
  cal3d/coretrack.h
  cal3d/datasource.h
  cal3d/dualquaternion.h
  cal3d/error.h
  cal3d/global.h
  cal3d/hardwaremodel.h
  cal3d/loader.h
  cal3d/matrix.h
  cal3d/mesh.h
  cal3d/mixer.h
  cal3d/model.h
  cal3d/morphtargetmixer.h
  cal3d/physiquedualquaternion.h
  cal3d/physique.h
  cal3d/platform.h
  cal3d/quaternion.h
  cal3d/refcounted.h
  cal3d/refptr.h
  cal3d/renderer.h
  cal3d/resource.h
  cal3d/saver.h
  cal3d/skeleton.h
  cal3d/springsystem.h
  cal3d/streamsource.h
  cal3d/submesh.h
  cal3d/tinyxml.h
  cal3d/transform.h
  cal3d/vector.h
)

include_directories( 
  ../cal3d
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

ADD_LIBRARY(${LIB_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
#add_library( Cal3D STATIC ${SOURCES} )

