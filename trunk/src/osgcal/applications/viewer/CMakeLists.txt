SET(TARGET_NAME osgCalViewer)

SET(OSG_LIBS OpenThreads osg osgDB osgUtil osgGA osgViewer osgText)

SET(SOURCE_FILES osgCalViewer.cpp)

INCLUDE_DIRECTORIES(
  ${OSGCAL_INCLUDE_DIR}
  ${OSG_INCLUDE_DIR}
  ${CAL3D_INCLUDE_DIR}
  ${OPENTHREADS_INCLUDE_DIR}
)

LINK_DIRECTORIES(
  ${OPENTHREADS_LIBRARY_DIR}
  ${OSG_LIBRARY_DIR}
  ${CAL3D_LIBRARY_DIR}
)

OSGCAL_APPLICATION( ${TARGET_NAME} ${SOURCE_FILES} )

LINK_INTERNAL(${TARGET_NAME} ${OSG_LIBS} osgCal)
