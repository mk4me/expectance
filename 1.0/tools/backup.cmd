@echo off 
cls
rem backup generator project from command line (C) ftoday 2007
rem author Marek Kulbacki
echo .
echo USAGE: backup.cmd [revision number] 
echo ..
echo It has to start from the same level as solution directory.
echo Solution directory name has to start from "rev_"

echo ...

if "%1"=="" echo No parameters ... exit
if "%1"=="" goto finish

set hn=%computername%
echo backup uses ultra compression with 4 threads it's probably tea time for you :)

echo ==================(%date%)==================== >> %hn%_report.%date%.txt
echo Backup for generator - revision %1 >> %hn%_report.%date%.txt
echo started at: %time% >> %hn%_report.%date%.txt

call ..\clean_all.cmd
cd ..\..\

xcopy rev_%1 backup\tmp\rev_%1 /exclude:no.svn.txt /E /C /I /F /R /Y 
cd backup\tmp\rev_%1

"C:\Program Files\nw\Tools\7-Zip\7z.exe" a -tzip ..\..\generator_%1.zip   -mmt=4 -mx=5



echo clean all intermediate data
cd ..\..\..\
rmdir /S /Q backup\tmp

echo finished at: %time% >> %hn%_report.%date%.txt
echo =====================(end)======================== >> %hn%_report.%date%.txt

:finish
