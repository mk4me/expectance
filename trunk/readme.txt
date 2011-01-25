rev 364
o----------------------------------------------------------------o
|                         ARENA 0.1.0		                 |
|                     (C) 2007 Future Today          		 |
o----------------------------------------------------------------o


----------------------------------------
1. How To Use  SVN repository

a) when you want to introduce your modifications to SVN:

- first check out latest repository from SVN to a new directory on your PC
- run above version in Visual Studio and check if compilation is correct and apllication 
	runs correctly
- apply your changes (new files and/or mofifications) to above version 
- check if compilation succeed and application runs properly
- add new files to SVN if any
- COMMIT this version to SVN

- check if commited version works properly by checking out the trunk again to another directory 
	on your PC (then check if compilation succeed and application runs properly)
 
----------------------------------------
2. Documentation 

Prerequisities
1. download the newest version of software: 
a) Doxygen ver >= 1.5.3 from http://www.stack.nl/~dimitri/doxygen/
b) Graphviz from http://www.graphviz.org/
c) HTML Workshop (default location C:\Program Files\HTML Help Workshop\ in MS Windows XP Professional) or download from http://go.microsoft.com/fwlink/?linkid=14188
d) eventually mscgen (for sequence diagrams) from http://www.mcternan.me.uk/mscgen/index.html and copy it under Graphviz\bin directory
2. check localization of your installed tools and correct values of HHC_LOCATION, MSCGEN_PATH, DOT_PATH from Generator.doxy file


----------------------------------------
3. How To Use Mantis

Mantis is used for bugtracking.

Assigned issue to person means that this person has plan of working on this bug (immediately 
or in the near future). But it doesn't mean that this person is working on this issue 
at the moment. To mark issue as being cuurently worked on the assigned person should change 
its status to 'ASSIGNED'. 


------------------------------------------
4. How To Use CMake tool for making projects ( under Linux for now)

Generator is made up of 4 parts : cal3d, engine, geneator and pygen (pygen is ommited here because it uses windows only compatybile threads)
Prerequisities:
a) prepare 4 CMakeLists.txt files (in main directory, in src/cal3d, scr/engine and src/generator)
The simplest method is to use ruby sctipt (tools/vproj2cmake) under directory where vcproj file exists and after that customize generated file CMakeLists.txt manually or just do everything manually

b) using CMake prepare makefiles on the base of each CMakeLists.txt using command:
cmake /subdirectiories ... /main directory
cmake /subdirectiories ... /src/cal3d
cmake /subdirectiories ... /src/engine
cmake /subdirectiories ... /src/generator
after that each location (project) has it's own makefile

c) under main directory run make clean and then make all - it will build the project under linux

------------------------------------------
5. Building and using python scripting

5.1. Windows platform


a) Release version:

- select DLL Release configuration in Visual C++
- build all projects
- copy from /lib/win32/release  to '_out/win32' directory following files: boost_python.dll, cal3d.dll, engine.dll, generator.dll
	pygen.pyd

- copy to '_out/win32' the directory wiht data
- start 'python' interpreter
- run in python following commands:
	import pygen
	pygen.runApp()

- Generator should start in separate thread and python interpreter should be ready for next commands


a) Debug version:

- select DLL Debug configuration in Visual C++
- build all projects
- copy from /lib/win32/debug  to '_out/win32' directory following files: boost_python_debug.dll, 
	cal3d_d.dll, engine_d.dll, generator_d.dll, pygen_d.pyd

- copy to '_out/win32' the directory wiht data
- start 'python_d' interpreter
- run in python following commands:
	import pygen
	pygem.runApp()

- Generator should start in separate hread and python_d interpreter should be ready for next commands


5.2. Linux platform

To build Generator as library for python you must perform following steps:
- switch ON the option BUILD_FOR_PYTHON in main  CMakeList.txt file (main directory)
	OPTION(BUILD_FOR_PYTHON "Build library for use in python" ON)
- goto /tools directory and execute build.sh command (ensure that you have rights execute this file)
- libypygen.so shuld be built in /src/pygen directory

To use library for python:
- place libpygen.so in any '_out/linux' directory together with /data directory
- run python interpreter
- run in python following commands:
	import pygen
	pygem.runApp()

- NOTE that both  DEBUG and RELEASE version of libpygen.so shoud work correctly in the same 'python´ interpreter in linux system






