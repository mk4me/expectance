#!/bin/sh
echo "Prepares linux version of generator (C) ftoday 2008"
echo "by MKA"
cd ..
pwd
echo "1. Recursively removing current cmake configurations: CMakeCache.txt files and CMakeFiles folders"
rm -rf `find . -type f -name CMakeCache.txt`
rm -rf `find . -type d -name CMakeFiles`

echo "2. Prepare new cmake configurations for projects"
cd src/cal3d
cmake .
cd ../engine
cmake .
cd ../generator
cmake .

#pygen actually doesn't work because of windows threads implementation
#cd ../pygen
#cmake .

cd ../..
cmake .

echo "3. Make projects"
make clean
make

# generator needs /data directory in the same scope so must be copied into apropriate place (main source directory)
echo "4. Moves output into _deploy/linux directory"
mkdir -p -v _deploy/linux/data
cp scr/generator/generator _deploy/linux
cp -r -v data _deploy/linux/



	
