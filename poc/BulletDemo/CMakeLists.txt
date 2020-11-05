message("Building BulletDemo")  

project( BulletDemo )

cmake_minimum_required( VERSION 3.10 FATAL_ERROR )


get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )

include("$ENV{CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")
include("$ENV{vcpkgRoot}/scripts/buildsystems/vcpkg.cmake")

find_package(GLUT REQUIRED)
if (GLUT_FOUND)
	message("GLUT FOUND")
	message(${GLUT_LIBRARIES})
else (GLUT_FOUND)
	message("OPENGL NOT FOUND")
endif (GLUT_FOUND)

find_package(OpenGL)
if (OPENGL_FOUND)
	message("OPENGL FOUND")
	message(${OPENGL_LIBRARIES})
else (OPENGL_FOUND)
	message("OPENGL NOT FOUND")
	set(OPENGL_gl_LIBRARY opengl32)
	set(OPENGL_glu_LIBRARY glu32)
endif (OPENGL_FOUND)


include_directories(
$ENV{BULLET_PHYSICS_SOURCE_DIR}/src
)

link_directories(
$ENV{BULLET_PHYSICS_SOURCE_DIR}/build_backup/lib/Debug/
)

link_libraries(
 BulletDynamics_Debug BulletCollision_Debug LinearMath_Debug
)

ci_make_app(
	SOURCES		${APP_PATH}/src/BulletDemo.cpp
	INCLUDES	${APP_PATH}/include
	CINDER_PATH $ENV{CINDER_PATH}
)

message("Done building BulletDemo")  