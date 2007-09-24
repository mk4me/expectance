rem builds generator VisualStudio project from command line (C) ftoday 2007
rem by MKA
"%VS80COMNTOOLS%..\IDE\devenv.exe" %CD%\Generator.sln /build "FT Full Debug Max 6|Win32"  /out build_all.log 
