#!/bin/bash

function make_lib()
{
	cmake_path=$(which cmake)

	if [[ -z $cmake_path ]]
	then
		echo "Please install cmake before building."
		exit
	fi
	
	if [[ ! -f  /usr/lib/libGLU.so ]]
	then
		lglu=`find /usr -name libGLU.so.1 |head -n 1`
		if [[ -z $lglu ]]
		then
			echo "Please install libGLU before building."
			exit
		fi
		sudo ln -s $lglu /usr/lib/libGLU.so
	fi

	if [[ -d "/opt/OpenGL" ]]
	then
		sudo rm -rf /opt/OpenGL
	fi
	sudo mkdir -p /opt/OpenGL
	sudo cp -arf  ./3rd/OpenGL/include /opt/OpenGL
	file /sbin/init |grep -q 64-bit
	if [[ $? = 0 ]]
	then
		# 64bit
		sudo cp -arf ./3rd/OpenGL/lib/Linux/x64  /opt/OpenGL/lib
	else
		# 32bit
		sudo cp -arf ./3rd/OpenGL/lib/Linux/x32  /opt/OpenGL/lib
	fi
	sudo cp -af /opt/OpenGL/lib/*  /usr/lib

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





