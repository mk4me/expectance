# (c) by Future Today 2008
# The name of our project is "generator".  CMakeLists files in this project can
# refer to the root source directory of the project as ${GENERATOR_SOURCE_DIR} and
# to the root binary directory of the project as ${GENERATOR_BINARY_DIR}.

project (generator)

# Recurse into the "cal3d", "engine" and "generator" subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.

#OPTION(DEBUG_BUILD "Compile projects in debug mode" ON)
OPTION(DEBUG_BUILD "Compile projects in debug mode" OFF)

#OPTION(BUILD_FOR_PYTHON "Build library for use in python" OFF)
OPTION(BUILD_FOR_PYTHON "Build library for use in python" ON)


add_subdirectory (src/cal3d)
add_subdirectory (src/engine)
add_subdirectory (src/generator)

IF (BUILD_FOR_PYTHON)
	add_subdirectory (src/pygen)
ENDIF(BUILD_FOR_PYTHON)



