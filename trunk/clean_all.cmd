rem cleans all unnnecesary files in project (C) ftoday 2007
rem by MKA
rem "%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /clean debug
rem "%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /clean release
echo T | del /F "src/cal3d/Debug/." > null
echo T | del /F "src/cal3d/Release/." > null
echo T | del /F "src/engine/Debug/." > null
echo T | del /F "src/engine/Release/." > null
echo T | del /F "src/generator/Debug/." > null
echo T | del /F "src/generator/Release/." > null
rmdir /S /Q "src/cal3d/FT Full Debug Max 6"
rmdir /S /Q "src/cal3d/FT Full Release Max 6"
rmdir /S /Q "src/engine/FT Full Debug Max 6"
rmdir /S /Q "src/engine/FT Full Release Max 6"
rmdir /S /Q "src/generator/FT Full Debug Max 6"
rmdir /S /Q "src/generator/FT Full Release Max 6"
rmdir /S /Q _ReSharper.Generator
rmdir /S /Q "_doc"
rmdir /S /Q "_deploy"

del /F doc\generator.log
del /F doc\generator.aux
del /F doc\generator.out
del /F doc\generator.toc
del /F doc\generator.chm

del /F bin\release\*.exp
del /F bin\release\*.ilk 
del /F bin\release\*.exe
del /F bin\debug\*.exp 
del /F bin\debug\*.ilk 
del /F bin\debug\*.exe

del /F Generator.ncb
del /F Generator.resharper
del /F Generator.resharper.user
del /F Cal3d.resharper
del /F Cal3d.resharper.user
del /F tools\build_all.log
del /F null

