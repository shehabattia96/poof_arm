message("Building Simple")  
cmake_minimum_required( VERSION 3.10 FATAL_ERROR )

project( Simple )

get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )

include("$ENV{CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")


ci_make_app(
	SOURCES		${APP_PATH}/Simple.cpp
	INCLUDES	${APP_PATH}/include
	CINDER_PATH $ENV{CINDER_PATH}
	RESOURCES   ${APP_PATH}/resources/cinder_app_icon.ico
)

message("Done building Simple")  
