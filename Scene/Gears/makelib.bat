@echo OFF

if "%1" == "" goto make_debug

if "%1" == "clean" goto clean

if "%1" == "debug" goto make_debug

if "%1" == "release" goto make_release

:error
echo "error"
exit

:clean
echo "Clean..."
rd /Q /S build
md build
cd ../../Output
rd /Q /S dso
md dso
exit

:make_debug
echo "Build debug...'
md build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../../Output -G"Visual Studio 9 2008"  ..
exit

:make_release
echo "Build release..."
md build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../Output -G"Visual Studio 9 2008"  ..
exit