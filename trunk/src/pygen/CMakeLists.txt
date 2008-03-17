project( pygen )

set(SOURCES 
  src/app/genthread.cpp
  src/app/pyapp.cpp
  src/app/threads/threads.cpp
  src/bindings/test_bindings.cpp
)

include_directories( 
  ../../src
  ../engine/src
  ../../src/engine
  ../../src/cal3d
  ../../src/generator
  utils/textures
)

add_definitions( 
  -D_CONSOLE
  -D_DEBUG
)

add_executable( pygen ${SOURCES} )

target_link_libraries( pygen glut )
