project( pygen )

set(SOURCES 
  src/app/pyapp.cpp
  src/bindings/test_bindings.cpp
)

include_directories( 
   ../
   ../cal3d
   ../engine/src
   ../generator/src/framework
  /usr/include/boost
  /usr/include/python2.5
)

link_directories (
     ../engine
     ../cal3d
     ../generator
     /usr/lib
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


add_library( pygen SHARED ${SOURCES} )

#abak:  NOTE that:  maybe -lboost_python-mt should be used instead of -lboost_python in case of problems with multithreading
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_python -lboost_thread-mt")


target_link_libraries( pygen generator Cal3D engine glut )
