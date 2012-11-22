#!/bin/bash

function make_lib()
{
	cmake_path=$(which cmake)

	if [[ -z $cmake_path ]]
	then
		echo "Please install cmake before building."
		exit
	fi

	mkdir build
	cd build

	if [[ $1 = 1 ]]
	then
		echo "Build debug version..."
		cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../../Output  ..
	else
		echo "Build release version..."
		cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../Output ..
	fi
	
	make 
	make install
}

function clean()
{
	echo "Clean..."
	rm -rf build/*
	rm -rf ../../Output/dso/*
}

if [[ -z $1 || $1 = "debug" ]]
then
	make_lib 1
elif [[ $1 = "release" ]]
then
	make_lib 0
elif [[ "$1" = "clean" ]] 
then
	clean 
else
	echo "Unknown parameter![debug|release|clean]"
fi

exit





