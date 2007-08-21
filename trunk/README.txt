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
