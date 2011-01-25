# (c) by Future Today 2008

SET(TARGET_NAME expectance)

SET(OSG_LIBS OpenThreads osg osgDB osgUtil osgGA osgViewer osgText osgShadow)

SET(SOURCE_FILES 
  src/app/config.cpp
  src/app/osgmain.cpp
  src/utility/mathutil.cpp
  src/utility/stringhelper.cpp
  src/motion/animation.cpp
  src/motion/transform.cpp
  src/timeline/lcsmodifier.cpp
  src/timeline/stopcontroller.cpp
  src/timeline/footdetector.cpp
  src/timeline/speedcontroller.cpp
  src/evolution_impl/cal3danimprovider.cpp
  src/evolution_impl/cal3dimpl.cpp
  src/control/controlmanager.cpp
  src/avatar/osgavatar.cpp
  src/avatar/osgavatarfactory.cpp
  src/avatar/osgavatartype.cpp
  src/scene/view/follownodemanip.cpp
  src/scene/view/transformaccumulator.cpp
  src/scene/worldmanager.cpp
  src/scene/object/traceline.cpp
#  src/scene/shadowmanager.cpp
#  src/data/tinyxml/tinyxml.cpp
#  src/data/tinyxml/tinyxmlerror.cpp
#  src/data/tinyxml/tinyxmlparser.cpp
#  src/data/tinyxml/tinystr.cpp
#  src/data/xmlsceneparser.cpp
#  src/data/datacollector.cpp
  src/utility/randomgen.cpp
  src/ai/goals/limitedareagoal.cpp
  src/ai/goals/changedirgoal.cpp
  src/ai/goals/randommovegoal.cpp
  src/ai/constraints/limitedareaconstraint.cpp
)

INCLUDE_DIRECTORIES(
   ../evolution/api
   ../cal3d
   ../osgcal/include

)


LINK_DIRECTORIES(
  ${OPENTHREADS_LIBRARY_DIR}
  ${OSG_LIBRARY_DIR}
  ../evolution
  ../cal3d
  ../osgCal/lib
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

#LINK_INTERNAL(${TARGET_NAME} ${OSG_LIBS} evolution Cal3D osgCal)
add_executable( ${TARGET_NAME} ${SOURCE_FILES} )
target_link_libraries(${TARGET_NAME} ${OSG_LIBS}  evolution Cal3D osgCal)






