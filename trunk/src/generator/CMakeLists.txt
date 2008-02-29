# (c) by Future Today 2008
project( generator )

set(SOURCES 
  src/framework/avatar/avatar.cpp
  src/framework/avatar/avatarfactory.cpp
  src/framework/avatar/avatartype.cpp
  src/framework/ai/aiavatar.cpp
  src/framework/ai/aicontroller.cpp
  src/framework/ai/aimanager.cpp
  src/framework/ai/constraint.cpp
  src/framework/ai/goal.cpp
  src/framework/ai/goalmanager.cpp
  src/framework/ai/rule.cpp
  src/framework/ai/goals/changedirgoal.cpp
  src/framework/ai/goals/limitedareagoal.cpp
  src/framework/ai/goals/randommovegoal.cpp
  src/framework/ai/constraints/limitedareaconstraint.cpp
  src/framework/motion/motion.cpp
  src/framework/motion/movableavatar.cpp
  src/framework/motion/timelinevisualizer.cpp
  src/framework/timeline/timeline.cpp
  src/framework/timeline/timelineblender.cpp
  src/framework/timeline/timelinecontext.cpp
  src/framework/timeline/timelineexecutor.cpp
  src/framework/timeline/timelinemodifier.cpp
  src/framework/timeline/timelinemotion.cpp
  src/framework/timeline/timelineobject.cpp
  src/framework/timeline/lcsmodifier.cpp
  src/framework/timeline/footdetector.cpp
  src/framework/control/controlmanager.cpp
  src/framework/action/action.cpp
  src/framework/action/actionavatar.cpp
  src/framework/action/idleaction.cpp
  src/framework/action/runaction.cpp
  src/framework/action/walkaction.cpp
  src/framework/physics/magnetcontroller.cpp
  src/framework/physics/physicsavatar.cpp
  src/framework/physics/speedcontroller.cpp
  src/framework/physics/cdmanager.cpp
  src/framework/app/gendebug.cpp
  src/framework/app/generatorapp.cpp
  src/framework/app/genmain.cpp
  src/framework/app/genvisualizationmanager.cpp
)

include_directories( 
   ../
   ../cal3d
   ../engine/src
)

link_directories (
#     ../../src/engine
     ../engine
#     ../../src/cal3d
     ../cal3d
#    /usr/lib
)

add_definitions( 
  -D_CONSOLE
  -D_DEBUG
)

add_executable(generator ${SOURCES})

target_link_libraries( generator Cal3D engine glut )
