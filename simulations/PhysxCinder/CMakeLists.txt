# Notes: 
# 1. Make sure the following are set in environment variables:
# CINDER_PATH - path to the cloned repository git://github.com/cinder/Cinder.git
# PHYSX_PATH - path to the `{cloned repository}/physx` directory git://github.com/NVIDIAGameWorks/PhysX.git
# 2. This CMakeLists file is hardcoded to build for the Windows platform.

message("Building PhysxCinder")  
cmake_minimum_required( VERSION 3.10 FATAL_ERROR )

project( PhysxCinder ) # this becomes the name of the executable

get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )

include("$ENV{CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

#### Build PhysX library ####
# PHYSX_PATH - path to the `{cloned repository}/physx` directory git://github.com/NVIDIAGameWorks/PhysX.git
set( PHYSX_ROOT_DIR $ENV{PHYSX_PATH} ) #This is needed for $ENV{PHYSX_PATH}/compiler/public/CMakeLists.txt
set( PHYSX_INCLUDE_DIRS $ENV{PHYSX_PATH}/include/ $ENV{PHYSX_PATH}/../pxshared/include/ )
set( PHYSX_LIBRARIES
	PhysXExtensions
	PhysX
	PhysXPvdSDK
	PhysXVehicle
	PhysXCharacterKinematic
	PhysXCooking
	PhysXCommon
	PhysXFoundation
	# SnippetUtils
)

message("CMAKE_BUILD_TYPE " ${CMAKE_BUILD_TYPE})
set(CMAKE_CONFIGURATION_TYPES "debug") #enum configTypes {debug checked profile release}
if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CONFIGURATION_TYPES "checked") #debug checked profile release
endif()
# from physx\samples\compiler\cmake\CMakeLists.txt
IF(CMAKE_CONFIGURATION_TYPES)
	SET(CMAKE_CONFIGURATION_TYPES debug checked profile release)
	SET(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
		"Reset config to what we need" 
		FORCE)
	
	# Need to define these at least once.	
	SET(CMAKE_EXE_LINKER_FLAGS_PROFILE "")
	SET(CMAKE_EXE_LINKER_FLAGS_CHECKED "")	
ENDIF()
set(TARGET_BUILD_PLATFORM "windows") # has to match the TARGET_BUILD_PLATFORM in $ENV{PHYSX_PATH}/physix/buildtools/cmake_generate_projects.py
set(PX_BUILDSNIPPETS OFF CACHE BOOL "Generate the snippets")
set(PX_BUILDPUBLICSAMPLES OFF CACHE BOOL "Generate the samples projects")
set(PX_GENERATE_STATIC_LIBRARIES OFF CACHE BOOL "Generate static libraries")
set(PX_FLOAT_POINT_PRECISE_MATH OFF CACHE BOOL "Float point precise math")
set(NV_USE_STATIC_WINCRT ON CACHE BOOL "Use the statically linked windows CRT")
set(NV_USE_DEBUG_WINCRT ON CACHE BOOL "Use the debug version of the CRT")
set(PXSHARED_PATH $ENV{PHYSX_PATH}/../pxshared)
set(PXSHARED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CMAKEMODULES_VERSION "1.27")
set(CMAKEMODULES_PATH $ENV{PHYSX_PATH}/../externals/cmakemodules)
set(PX_OUTPUT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/externals/physx)
set(PX_OUTPUT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/externals/physx)
  
# Call into PhysX's CMake scripts
add_subdirectory($ENV{PHYSX_PATH}/compiler/public externals/physx)

SET(CMAKE_BUILD_TYPE "" CACHE PATH "Current build configuration")

#### Build Cinder dependencies and main executable ####
ci_make_app(
	SOURCES		${APP_PATH}/PhysxCinder.cpp
	INCLUDES	${APP_PATH}/include ${PHYSX_INCLUDE_DIRS}
	CINDER_PATH $ENV{CINDER_PATH}
  	RESOURCES   ${APP_PATH}/resources/cinder_app_icon.ico
	LIBRARIES ${PHYSX_LIBRARIES} #same as `target_link_libraries(PhysxCinder cinder ${PHYSX_LIBRARIES})`
)

#### Windows only: Copy the Physx dll files to the PhysxCinder executable####

if (TARGET_BUILD_PLATFORM STREQUAL "windows")
	# References NvidiaBuildOptions.cmake to figure out if system is 32/64 bit
	IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET(LIBPATH_SUFFIX "64")
	ELSE()
		SET(LIBPATH_SUFFIX "32")
	ENDIF()
	GetPlatformBinName(PLATFORM_BIN_NAME ${LIBPATH_SUFFIX})
	set(PhysxOutputPath ${PX_OUTPUT_LIB_DIR}/bin/${PLATFORM_BIN_NAME}/)
	message("Physx Output Path: " ${PhysxOutputPath})

	# Create a command to run every time target is built:
	add_custom_command(TARGET PhysxCinder POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${PhysxOutputPath}" "$<TARGET_FILE_DIR:PhysxCinder>/..")
endif()


message("Done building PhysxCinder")  
