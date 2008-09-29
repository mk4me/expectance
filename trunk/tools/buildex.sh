#!/bin/sh
echo "Prepares linux version of expectance (C) ftoday 2008"
echo "by MKA"
cd ..
pwd
echo "1. Recursively removing current cmake configurations: CMakeCache.txt files and CMakeFiles folders"
rm -rf `find . -type f -name CMakeCache.txt`
rm -rf `find . -type d -name CMakeFiles`
rm -rf `find . -type d -name *.so`

echo "2. Prepare new cmake configurations for projects"

cd src/cal3d
cmake .

cd ../evolution
cmake .

cd ../osgcal
cmake .

cd ../expectance
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
#echo "4. Moves output into _deploy/linux directory"
#mkdir -p -v _out/linux/data
#find _out/linux/ -maxdepth 1 -type f -name "*" -delete
#rsync -rv data _out/linux/ --exclude=\".svn\"
#rsync -rv python/* _out/linux/ --exclude=\".svn\"

#rsync -v src/generator/generator _out/linux
#rsync -v src/pygen/libpygen.so _out/linux


	
