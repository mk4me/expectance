# (c) by Future Today 2008
project( Cal3D )

set(SOURCES 
  animation.cpp
  animation_action.cpp
  animation_cycle.cpp
  bone.cpp
  buffersource.cpp
  cal3d.rc
  cal3d_wrapper.cpp
  coreanimation.cpp
  corebone.cpp
  corekeyframe.cpp
  corematerial.cpp
  coremesh.cpp
  coremodel.cpp
  coremorphanimation.cpp
  coreskeleton.cpp
  coresubmesh.cpp
  coresubmorphtarget.cpp
  coretrack.cpp
  dualquaternion.cpp
  error.cpp
  global.cpp
  hardwaremodel.cpp
  loader.cpp
  matrix.cpp
  mesh.cpp
  mixer.cpp
  model.cpp
  morphtargetmixer.cpp
  physique.cpp
  physiquedualquaternion.cpp
  platform.cpp
  quaternion.cpp
  renderer.cpp
  saver.cpp
  skeleton.cpp
  springsystem.cpp
  streamsource.cpp
  submesh.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
  vector.cpp
)

include_directories( 
  ..
)

add_definitions( 
  -DCAL3D_EXPORTS
#  -DWIN32
  -D_DEBUG
  -D_USRDLL
#  -D_WINDOWS
)

#add_library( Cal3D STATIC ${SOURCES} )
add_library( Cal3D SHARED ${SOURCES} )

