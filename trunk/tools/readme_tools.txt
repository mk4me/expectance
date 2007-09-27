TOOLS

1. automatic_build.cmd
Complex batch for taking revision from subversion server, building that and putting necessary files into deploy directory. It utilizes the same mechanism as build_all.cmd for build process. It must start from directory level equal to revision directory location.	
Prerequisites:
-copy that file two levels up from that directory
-revision directory must have format rev_[number] - where number stands for revision number we want to take

USAGE: automatic_build.cmd [revision number] [build mode: debug or release]
EXAMPLE: automatic_build.cmd 137 debug        
It starts taking revision number 137 and builds it in debug mode, next copies all output files to \deploy\tests\rev_137\debug\ directory and tries to start application

2. build_all.cmd
It builds project without Visual Studio environment. Runs from tools directory.
Prerequisites:
-visual studio tool vcbuild.exe is required
-revision directory must have format rev_[number] - where number stands for revision number we want to take

USAGE: build_all.cmd [revision number] [build mode: debug or release] where revision number stands for actual revision number
EXAMPLE: build_all.cmd 137 debug        
It builds revision 137 in debug mode, next copies all output files to ..\..\deploy\tests\rev_137\debug\ directory and tries to start application

3. backup.cmd
Creates backup on the base of the given revision directory but without .svn directories. It works from the tools directory. On the same level you should keep the file no.svn.txt
Prerequisites:
-it's necessary to have free tool 7z for zip compression. It's easy to exchange it on other tool (http://www.7-zip.org/pl/7z.html)
-thanks to the file no.svn.txt backup doesn't copy directories with .svn patters so it's necessary to be in the same directory as backup.cmd
-revision directory must have format rev_[number] - where number stands for revision number we want to take

USAGE: backup.cmd [revision number] where revision number stands for actual revision number
EXAMPLE: backup.cmd 137
It cleans current solution from output files and prepares using 7z free tool package under directory ..\..\backup\generator_rev[number].


4. documentation.cmd
Runs from tools directory.
Prerequisites:
-Doxygen and Graphvis is required to be installed in system

USAGE: documentation.cmd 
EXAMPLE: documentation.cmd
It builds documentation on the base generator.doxy file from tools directory, compilation of TeX documentation from ..\doc\ directory. All documentation is gathered automatically under ..\dc directory.
