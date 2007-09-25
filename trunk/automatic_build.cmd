@echo off 
cls
rem Build automation for generator project from command line (C) ftoday 2007
rem author Marek Kulbacki
echo .
echo USAGE: automatic_build.cmd [ revision number ] [ release or debug ] 
echo ..
echo It has to start from the same level as solution directory.
echo Solution directory name has to start from "rev_"
echo ...

if "%1"=="" echo No parameters ... exit
if "%1"=="" goto finish
if "%2"=="" goto finish

@echo on

set hn=%computername%

echo Hello it's automatic build time at %date% for generator files
echo ==================(build date:%date%)==================== >> %hn%_report.%date%.txt
echo Build for generator - revision %1 >> %hn%_report.%date%.txt
echo started at: %time% >> %hn%_report.%date%.txt

mkdir rev_%1
rem get files from repository
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:checkout /url:"http://tools.assembla.com/svn/aErVZ6gMOr3lOqabIlDkbG/trunk" /path:"C:\Work\current\generator\rev_%1" /revision:%1 /closeonend:1 /notempfile
rem ___ copy build_all.cmd rev_%1
cd rev_%1
rem prepare build, deploy and try to run it

echo build started at: %time% >> ..\%hn%_report.%date%.txt

if "%2"=="release" goto release
elseif "%2"=="debug" goto debug
else goto finish


:debug
"%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /build "FT Full Debug Max 6|Win32"  /out build_all.log 

mkdir ..\deploy\tests\rev_%1\debug
mkdir ..\deploy\tests\rev_%1\debug\data\
xcopy /e /v /y bin\debug ..\deploy\tests\rev_%1\debug\
xcopy /e /v /y data ..\deploy\tests\rev_%1\debug\data\
cd ..\deploy\tests\rev_%1\debug\
echo starting generator.exe debug revision %1
generator.exe > ..\..\rev_%1_debug.log
cd ..\..\..\..

GOTO finish

:release
"%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /build "FT Full Release Max 6|Win32"  /out build_all.log 

mkdir ..\deploy\tests\rev_%1\release
mkdir ..\deploy\tests\rev_%1\release\data\
xcopy /e /v /y bin\release ..\deploy\tests\rev_%1\release\
xcopy /e /v /y data ..\deploy\tests\rev_%1\release\data\
cd ..\deploy\tests\rev_%1\release\
echo starting generator.exe release revision %1
generator.exe > ..\..\rev_%1_release.log
cd ..\..\..\..


:finish

echo build finished at: %time% >> %hn%_report.%date%.txt
echo ====================(end)====================== >> %hn%_report.%date%.txt



