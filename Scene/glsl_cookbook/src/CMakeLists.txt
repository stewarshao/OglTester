FILE(GLOB_RECURSE LIB_SRC "*.cpp" "*.h")
FOREACH(src ${LIB_SRC}) 
	MESSAGE( ${src} ) 
ENDFOREACH() 

SET(DSO_NAME glslcookbook)

if(WIN32)
	INCLUDE_DIRECTORIES("C:/OpenGL/include")
	LINK_DIRECTORIES("C:/OpenGL/lib")
	SET (LD_LIBS "glew32.lib")
else() 
	INCLUDE_DIRECTORIES("/opt/OpenGL/include")
	LINK_DIRECTORIES("/opt/OpenGL/lib")
	SET (LD_LIBS "GLEW" "GLU")
endif()

ADD_LIBRARY(${DSO_NAME} SHARED ${LIB_SRC})
TARGET_LINK_LIBRARIES(${DSO_NAME} ${LD_LIBS})

IF(CMAKE_BUILD_TYPE STREQUAL Release)
	if(WIN32)
		SET(LIBRARY_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/dso)
		install(DIRECTORY shader/ 
		        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/shader/cookbook)
		install(DIRECTORY media/ 
		        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/media)		
	else()
		SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/release)
		INSTALL(TARGETS ${DSO_NAME} 
				LIBRARY DESTINATION dso/Release) 
		install(DIRECTORY shader/ 
		        DESTINATION bin/shader/cookbook)
		install(DIRECTORY media/ 
		        DESTINATION bin/media)
	endif()	
ELSE()
	if(WIN32)
		SET(LIBRARY_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/dso) 
		install(DIRECTORY shader/ 
		        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/shader/cookbook)
		install(DIRECTORY media/ 
		        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/media)
	else()
		SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/debug)
		INSTALL(TARGETS ${DSO_NAME} 
				LIBRARY DESTINATION dso/Debug) 
		install(DIRECTORY shader/ 
		        DESTINATION bin/shader/cookbook)
		install(DIRECTORY media/ 
		        DESTINATION bin/media)
	endif()
ENDIF()

find_package(OpenGL)
if (OPENGL_FOUND)
	MESSAGE("OpenGL Correctly Found")
	include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
	MESSAGE("OpenGL environment missing")
endif()



