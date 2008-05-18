@echo off
echo ...
echo Builder for generator VisualStudio configurations from command line (C) ftoday 2007
echo by MKA
echo ___________________________
echo .        
echo usage: build [project] [build mode] under /tools solution directory
echo project: cal3d, engine.lib, engine.dll, generator, generator.dll, pygen.dll, all
echo build mode: debug, release, all
echo example: test all all (builds for tests all configurations and all existing projects)
echo ___________________________
if "%1"=="" if "%2"=="" goto end


if "%2"=="release" goto release
if "%2"=="debug" goto debug
if "%2"=="all" goto debug
echo Missing type of solution, stopping ...
goto end


:debug
echo debug build started at: %time% >> build_report.%date%.log
if "%1"== "cal3d" goto cal3d
if "%1"== "engine.lib" goto d_engine_lib
if "%1"== "engine.dll" goto d_engine_dll
if "%1"== "generator" goto d_generator
if "%1"== "generator.dll" goto d_generator_dll
if "%1"== "pygen.dll" goto d_pygen_dll
if "%1"== "all" goto d_cal3d
echo Missing project definition, stopping ...
goto end



:d_cal3d

"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_debug.log %CD%\..\src\cal3d\cal3d.vcproj "Full Debug|Win32"  

mkdir ..\_out\win32\tst\debug\cal3d
xcopy /e /v /y ..\lib\win32\debug\cal3d*.* ..\_out\win32\tst\debug\cal3d

echo debug build of CAL3D finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto d_engine_lib
goto finish


:d_engine_lib

if "%1"== "all" goto d_engine_lib1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_debug.log %CD%\..\src\cal3d\cal3d.vcproj "Full Debug|Win32"  
:d_engine_lib1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_engine_lib_debug.log %CD%\..\src\engine\engine.vcproj "Full Debug|Win32"  

mkdir ..\_out\win32\tst\debug\engine_lib
xcopy /e /v /y ..\lib\win32\debug\engine*.* ..\_out\win32\tst\debug\engine_lib

echo debug build of ENGINE.LIB finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto d_engine_dll
goto finish

:d_engine_dll

if "%1"== "all" goto d_engine_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_debug.log %CD%\..\src\cal3d\cal3d.vcproj "DLL Debug|Win32"  
:d_engine_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_engine_dll_debug.log %CD%\..\src\engine\engine.vcproj "DLL Debug|Win32"  

mkdir ..\_out\win32\tst\debug\engine_dll
xcopy /e /v /y ..\lib\win32\debug\engine*.* ..\_out\win32\tst\debug\engine_dll

echo debug build of ENGINE.DLL finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto d_generator
goto finish


:d_generator

if "%1"== "all" goto d_generator1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_debug.log %CD%\..\src\cal3d\cal3d.vcproj "Full Debug|Win32"  
:d_generator1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_generator_dll_debug.log %CD%\..\Generator.sln "Full Debug|Win32"  

mkdir ..\_out\win32\tst\debug\generator_exe
mkdir ..\_out\win32\tst\debug\generator_exe\data
xcopy /e /v /y ..\lib\win32\debug ..\_out\win32\tst\debug\generator_exe
xcopy /e /v /y ..\data ..\_out\win32\tst\debug\generator_exe\data\

echo debug build of GENERATOR.EXE finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto d_generator_dll
goto finish

:d_generator_dll

if "%1"== "all" goto d_generator_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_debug.log %CD%\..\src\cal3d\cal3d.vcproj "DLL Debug|Win32"  
:d_generator_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_generator_dll_debug.log %CD%\..\Generator.sln "DLL Debug|Win32"  

mkdir ..\_out\win32\tst\debug\generator_dll
xcopy /e /v /y ..\lib\win32\debug ..\_out\win32\tst\debug\generator_dll

echo debug build of GENERATOR.DLL finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto d_pygen_dll
goto finish

:d_pygen_dll

rem if "%1"== "all" goto d_pygen_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_generator_dll_debug.log %CD%\..\Generator.sln "DLL Debug|Win32"  
:d_pygen_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_pygen_dll_debug.log %CD%\..\src\pygen\pygen.vcproj "DLL Debug|Win32"  

mkdir ..\_out\win32\tst\debug\pygen_dll
mkdir ..\_out\win32\tst\debug\pygen_dll\data
xcopy /e /v /y ..\lib\win32\debug ..\_out\win32\tst\debug\pygen_dll\
xcopy /e /v /y ..\data ..\_out\win32\tst\debug\pygen_dll\data\


echo debug build of LIBPYGEN.DLL finished at: %time% >> build_report.%date%.log
if "%2"=="all" goto release
goto finish





:release
echo release build started at: %time% >> build_report.%date%.log
if "%1"== "cal3d" goto cal3d
if "%1"== "engine.lib" goto r_engine_lib
if "%1"== "engine.dll" goto r_engine_dll
if "%1"== "generator" goto r_generator
if "%1"== "generator.dll" goto r_generator_dll
if "%1"== "pygen.dll" goto r_pygen_dll
if "%1"== "all" goto r_cal3d
echo Missing project definition, stopping ...
goto end



:r_cal3d

"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_release.log %CD%\..\src\cal3d\cal3d.vcproj "Full Release|Win32"  

mkdir ..\_out\win32\tst\release\cal3d
xcopy /e /v /y ..\lib\win32\release\cal3d*.* ..\_out\win32\tst\release\cal3d

echo release build of CAL3D finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto r_engine_lib
goto finish


:r_engine_lib

if "%1"== "all" goto r_engine_lib1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_release.log %CD%\..\src\cal3d\cal3d.vcproj "Full Release|Win32"  
:r_engine_lib1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_engine_lib_release.log %CD%\..\src\engine\engine.vcproj "Full Release|Win32"  

mkdir ..\_out\win32\tst\release\engine_lib
xcopy /e /v /y ..\lib\win32\release\engine*.* ..\_out\win32\tst\release\engine_lib

echo release build of ENGINE.LIB finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto r_engine_dll
goto finish

:r_engine_dll

if "%1"== "all" goto r_engine_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_release.log %CD%\..\src\cal3d\cal3d.vcproj "DLL Release|Win32"  
:r_engine_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_engine_dll_release.log %CD%\..\src\engine\engine.vcproj "DLL Release|Win32"  

mkdir ..\_out\win32\tst\release\engine_dll
xcopy /e /v /y ..\lib\win32\release\engine*.* ..\_out\win32\tst\release\engine_dll

echo release build of ENGINE.DLL finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto r_generator
goto finish


:r_generator

if "%1"== "all" goto r_generator1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_release.log %CD%\..\src\cal3d\cal3d.vcproj "Full Release|Win32"  
:r_generator1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_generator_exe_release.log %CD%\..\Generator.sln "Full Release|Win32"  

mkdir ..\_out\win32\tst\release\generator_exe
mkdir ..\_out\win32\tst\release\generator_exe\data
xcopy /e /v /y ..\lib\win32\release ..\_out\win32\tst\release\generator_exe
xcopy /e /v /y ..\data ..\_out\win32\tst\release\generator_exe\data\

echo release build of GENERATOR.EXE finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto r_generator_dll
goto finish

:r_generator_dll

if "%1"== "all" goto r_generator_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_cal3d_release.log %CD%\..\src\cal3d\cal3d.vcproj "DLL Release|Win32"  
:r_generator_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_generator_dll_release.log %CD%\..\Generator.sln "DLL Release|Win32"  

mkdir ..\_out\win32\tst\release\generator_dll
xcopy /e /v /y ..\lib\win32\release ..\_out\win32\tst\release\generator_dll


echo release build of GENERATOR.DLL finished at: %time% >> build_report.%date%.log
if "%1"== "all" goto r_pygen_dll
goto finish

:r_pygen_dll

 
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_generator_dll_release.log %CD%\..\Generator.sln "DLL Release|Win32"  
rem :r_pygen_dll1
"%VS90COMNTOOLS%..\..\VC\vcpackages\vcbuild.exe" /r /time /logfile:build_pygen_dll_release.log %CD%\..\src\pygen\pygen.vcproj "DLL Release|Win32"  

mkdir ..\_out\win32\tst\release\pygen_dll
mkdir ..\_out\win32\tst\release\pygen_dll\data
xcopy /e /v /y ..\lib\win32\release ..\_out\win32\tst\release\pygen_dll\
xcopy /e /v /y ..\data ..\_out\win32\tst\release\pygen_dll\data\

echo release build of PYGEN.DLL finished at: %time% >> build_report.%date%.log

goto finish




:finish

echo whole build finished at: %time% >> build_report.%date%.log
mkdir ..\_out\win32\tst\logs
move /y build*.log ..\_out\win32\tst\logs\


:end
