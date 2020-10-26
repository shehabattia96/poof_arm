MESSAGE("Building BulletDemo")  

project( BulletDemo )

cmake_minimum_required( VERSION 3.10 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE ON )


get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )

include("$ENV{CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")
include("D:/projects/vcpkg/scripts/buildsystems/vcpkg.cmake")





find_package(GLUT REQUIRED)
IF (GLUT_FOUND)
	MESSAGE("GLUT FOUND")
	MESSAGE(${GLUT_LIBRARIES})
ELSE (GLUT_FOUND)
	MESSAGE("OPENGL NOT FOUND")
ENDIF (GLUT_FOUND)

FIND_PACKAGE(OpenGL)
IF (OPENGL_FOUND)
	MESSAGE("OPENGL FOUND")
	MESSAGE(${OPENGL_LIBRARIES})
ELSE (OPENGL_FOUND)
	MESSAGE("OPENGL NOT FOUND")
	SET(OPENGL_gl_LIBRARY opengl32)
	SET(OPENGL_glu_LIBRARY glu32)
ENDIF (OPENGL_FOUND)





INCLUDE_DIRECTORIES(
$ENV{BULLET_PHYSICS_SOURCE_DIR}/src
)

link_directories(
$ENV{BULLET_PHYSICS_SOURCE_DIR}/build_backup/lib/Debug/
)

LINK_LIBRARIES(
 BulletDynamics_Debug BulletCollision_Debug LinearMath_Debug
)

# IF (WIN32)
# 	ADD_EXECUTABLE(BulletDemo
# 		BulletDemo.cpp
# 		$ENV{BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc
# 	)
# ELSE()
# 	ADD_EXECUTABLE(BulletDemo
# 		BulletDemo.cpp
# 	)
# ENDIF()
# target_link_libraries(BulletDemo PRIVATE GLUT::GLUT)


# IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
# 			SET_TARGET_PROPERTIES(BulletDemo PROPERTIES  DEBUG_POSTFIX "_Debug")
# 			SET_TARGET_PROPERTIES(BulletDemo PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
# 			SET_TARGET_PROPERTIES(BulletDemo PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
# ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)


ci_make_app(
	SOURCES		${APP_PATH}/src/BulletDemo.cpp
	INCLUDES	${APP_PATH}/include
	CINDER_PATH $ENV{CINDER_PATH}
)

MESSAGE("Done building BulletDemo")  