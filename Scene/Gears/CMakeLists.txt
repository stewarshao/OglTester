cmake_minimum_required(VERSION 2.8)

PROJECT(GEARSLIB)	

#cmake -DCMAKE_BUILD_TYPE=Debug£®ªÚRelease£©
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

ADD_SUBDIRECTORY(src)

