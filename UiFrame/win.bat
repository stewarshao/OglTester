@echo OFF

if "%1" == "clean" goto clean 

echo "Generate OglTester vc2008 project..."

if exist OglTester.vcproj goto end 

qmake -tp vc OglTester.pro

if not exist OglTester.vcproj goto error 

:end
echo "Done"
exit

:error
echo "Error!Please check the OglTester.pro!"

:clean
del *.ncb
del *.sln
del *.vcproj
del *.user
del *.idb
del *.pdb
del Makefile*
rd /Q /S build
rd /Q /S ..\Output\bin\debug
rd /Q /S ..\Output\bin\release


