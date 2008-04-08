rem cleans all unnnecesary files in project (C) ftoday 2007
rem by MKA
rem "%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /clean debug
rem "%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /clean release
echo T | del /F "src/cal3d/win32/." > null
echo T | del /F "src/engine/win32/." > null
echo T | del /F "src/generator/win32/." > null
echo T | del /F "src/pygen/win32/." > null
rmdir /S /Q "src/cal3d/win32"
rmdir /S /Q "src/engine/win32"
rmdir /S /Q "src/generator/win32"
rmdir /S /Q "src/pygen/win32"
rmdir /S /Q _ReSharper.Generator
rmdir /S /Q _deploy
rmdir /S /Q _out
rmdir /S /Q _doc

del /F doc\generator.log
del /F doc\generator.aux
del /F doc\generator.out
del /F doc\generator.toc
del /F doc\generator.chm

del /F lib\win32\release\libpygen*.*
del /F lib\win32\release\engine*.*
del /F lib\win32\release\generator*.*
del /F lib\win32\release\*.exp
del /F lib\win32\release\*.ilk 
del /F lib\win32\release\*.exe

del /F lib\win32\debug\engine*.*
del /F lib\win32\debug\generator*.*
del /F lib\win32\debug\libpygen*.* 
del /F lib\win32\debug\*.ilk 
del /F lib\win32\debug\*.exe
del /F lib\win32\debug\*.exp 

del /F Generator.ncb
del /F Generator.resharper
del /F Generator.resharper.user
del /F Cal3d.resharper
del /F Cal3d.resharper.user
del /F tools\build_all.log
del /F null
