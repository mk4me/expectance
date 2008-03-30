#!/bin/sh
echo "Cleans all unnnecesary files in project (C) ftoday 2008"
echo "by MKA"
pwd
echo "1. Recursively removing .svn folders"
rm -rf `find . -type d -name .svn`
echo "2. Recursively removing CMakeCache.txt files and CMakeFiles folders"
rm -rf `find . -type f -name CMakeCache.txt`
rm -rf `find . -type f -name cmake_install.cmake`
rm -rf `find . -type d -name CMakeFiles`

echo "3. Recursively removing all folders starting with Debug and Release from /src"
rm -rf `find src -type d -name Debug*`
rm -rf `find src -type d -name Release*`

echo "4. Remove the rest unnecessary files"

rm  -rf _ReSharper.Generator
rm  -rf _doc
rm -f doc/generator.log
rm -f doc/generator.aux
rm -f doc/generator.out
rm -f doc/generator.toc
rm -f doc/generator.chm
rm -f bin/release/*.exp
rm -f bin/release/*.ilk 
rm -f bin/release/*.exe
rm -f bin/debug/*.exp 
rm -f bin/debug/*.ilk 
rm -f bin/debug/*.exe
rm -f Generator.ncb
rm -f Generator.resharper
rm -f Generator.resharper.user
rm -f Cal3d.resharper
rm -f Cal3d.resharper.user
rm -f tools/build_all.log
rm -f generator
rm -rf _deploy