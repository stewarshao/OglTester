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
if exist "C:\OpenGL" (
   rd /Q /S C:\OpenGL
)
md C:\OpenGL
xcopy .\3rd\OpenGL\include C:\OpenGL\include /e /i
xcopy .\3rd\OpenGL\lib\Win\*.lib C:\OpenGL\lib /e /i
if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86" (
	xcopy .\3rd\OpenGL\lib\Win\*.dll C:\Windows\System32 /e /i /y
) else (
	xcopy .\3rd\OpenGL\lib\Win\*.dll C:\Windows\SysWOW64 /e /i /y
)
md build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../../Output -G"Visual Studio 9 2008"  ..
exit

:make_release
echo "Build release..."
if exist "C:\OpenGL" (
   rd /Q /S C:\OpenGL
)
md C:\OpenGL
xcopy .\3rd\OpenGL\include C:\OpenGL\include /e /i
xcopy .\3rd\OpenGL\lib\Win\*.lib C:\OpenGL\lib /e /i
if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86" (
	xcopy .\3rd\OpenGL\lib\Win\*.dll C:\Windows\System32 /e /i /y
) else (
	xcopy .\3rd\OpenGL\lib\Win\*.dll C:\Windows\SysWOW64 /e /i /y
)
md build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../Output -G"Visual Studio 9 2008"  ..
exit