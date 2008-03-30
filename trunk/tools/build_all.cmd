rem builds generator VisualStudio project from command line (C) ftoday 2007
rem by MKA
echo usage: build_all [revision number] [release or debug] under solution directory
if "%1"=="" goto finish
if "%2"=="" goto finish

if "%2"=="release" goto release
if "%2"=="debug" goto debug
else goto finish

:debug
"%VS80COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_all.log %CD%\..\Generator.sln "FT Full Debug Max 6|Win32"  

mkdir ..\_deploy\win\rev_%1\debug
mkdir ..\_deploy\win\rev_%1\debug\data\
xcopy /e /v /y ..\bin\debug ..\_deploy\win\rev_%1\debug\
xcopy /e /v /y ..\data ..\_deploy\win\rev_%1\debug\data\
cd ..\_deploy\win\rev_%1\debug\
echo starting generator.exe debug revision %1
generator.exe > ..\rev_%1_debug.log
cd ..\..\..

goto finish

:release
"%VS80COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_all.log %CD%\..\Generator.sln "FT Full Release Max 6|Win32"  

mkdir ..\_deploy\win\rev_%1\release
mkdir ..\_deploy\win\rev_%1\release\data\
xcopy /e /v /y ..\bin\release ..\_deploy\win\rev_%1\release\
xcopy /e /v /y ..\data ..\_deploy\win\rev_%1\release\data\
cd ..\_deploy\win\rev_%1\release\
echo starting generator.exe release revision %1
generator.exe > ..\rev_%1_release.log
cd ..\..\..

:finish
