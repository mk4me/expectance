rem cleans all unnnecesary files in project (C) ftoday 2007
rem by MKA
"%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /clean debug
"%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /clean release
del /F Generator.ncb