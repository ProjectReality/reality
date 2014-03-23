cmake_minimum_required(VERSION 2.8)

project(ProjectReality)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		$ENV{OGRE_HOME}/Samples/include
	)
    link_libraries (winmm)
endif(WIN32)

if(UNIX)
	if(EXISTS "/usr/local/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

	elseif(EXISTS "/usr/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

	else ()
	  message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/local/lib/OGRE")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lXinerama -ludev")
endif(UNIX)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")

find_package(OpenCV REQUIRED)
find_package(OGRE REQUIRED)
find_package(OculusSDK REQUIRED)
find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()

# Find Boost
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
	if (WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else ()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif ()
	if (MINGW)
		# this is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif ()
	set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time system)
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	if (NOT Boost_FOUND)
		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	endif()
	find_package(Boost QUIET)

	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()

set(HDRS
        ./inc/IRenderer.hpp
        ./inc/OgreRenderer.hpp
        ./inc/StereoCamera.hpp
		./inc/Oculus.hpp
		./inc/VirtualOculus.hpp
)

set(SRCS
        ./src/main.cpp
        ./src/OgreRenderer.cpp
        ./src/StereoCamera.cpp
		./src/Oculus.cpp
		./src/VirtualOculus.cpp
)

include_directories(
    ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${OGRE_Overlay_INCLUDE_DIRS}
	${OCULUS_SDK_INCLUDE_DIRS}
    ${OPENCV_INCLUDE_DIR}
	./inc
)

add_executable(ProjectReality WIN32 ${HDRS} ${SRCS})

set_target_properties(ProjectReality PROPERTIES DEBUG_POSTFIX _d)

if(WIN32)
   set_target_properties(ProjectReality PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(ProjectReality PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   set_target_properties(ProjectReality PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(ProjectReality PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(ProjectReality PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
   set_target_properties(ProjectReality PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
endif(WIN32)

target_link_libraries(ProjectReality ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OCULUS_SDK_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} )
target_link_libraries(ProjectReality optimized ${OpenCV_LIBS})


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET ProjectReality POST_BUILD
		COMMAND copy \"$(TargetPath)\" . )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
endif(MINGW OR UNIX)

if(WIN32)

	install(TARGETS ProjectReality
		RUNTIME DESTINATION .
		CONFIGURATIONS All)

    #FIX PROBLEM WITH VS2013
	install(
        FILES ${CMAKE_SOURCE_DIR}/dllfix/msvcp100d.dll ${CMAKE_SOURCE_DIR}/dllfix/msvcr100d.dll ${CMAKE_SOURCE_DIR}/dllfix/cg.dll
		DESTINATION .
		CONFIGURATIONS Release RelWithDebInfo Debug
	)

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
        DESTINATION ./
        CONFIGURATIONS Release RelWithDebInfo Debug
    )

	install(FILES ${CMAKE_SOURCE_DIR}/config/plugins.cfg
		${CMAKE_SOURCE_DIR}/config/resources.cfg
		DESTINATION .
		CONFIGURATIONS Release RelWithDebInfo
	)

	install(FILES ${CMAKE_SOURCE_DIR}/config/plugins_d.cfg
		${CMAKE_SOURCE_DIR}/config/resources_d.cfg
		DESTINATION .
		CONFIGURATIONS Debug
	)

	install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
        ${OGRE_PLUGIN_DIR_REL}/Plugin_CgProgramManager.dll
        ${OGRE_PLUGIN_DIR_REL}/Plugin_OctreeSceneManager.dll
		${OGRE_PLUGIN_DIR_REL}/OIS.dll
		DESTINATION .
		CONFIGURATIONS Release RelWithDebInfo
	)

	install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
        ${OGRE_PLUGIN_DIR_DBG}/Plugin_CgProgramManager_d.dll
        ${OGRE_PLUGIN_DIR_DBG}/Plugin_OctreeSceneManager_d.dll
		${OGRE_PLUGIN_DIR_DBG}/OIS_d.dll
		DESTINATION .
		CONFIGURATIONS Debug
	)

   # as of sdk 1.7.2 we need to copy the boost dll's as well
   # because they're not linked statically (it worked with 1.7.1 though)
   install(FILES ${Boost_DATE_TIME_LIBRARY_RELEASE}
      ${Boost_THREAD_LIBRARY_RELEASE}
      DESTINATION .
      CONFIGURATIONS Release RelWithDebInfo
   )

   install(FILES ${Boost_DATE_TIME_LIBRARY_DEBUG}
      ${Boost_THREAD_LIBRARY_DEBUG}
      DESTINATION .
      CONFIGURATIONS Debug
   )
endif(WIN32)

if(UNIX)

	install(TARGETS ProjectReality
		RUNTIME DESTINATION .
		CONFIGURATIONS All)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
		DESTINATION .
		CONFIGURATIONS Release RelWithDebInfo Debug
	)

	install(FILES ${CMAKE_SOURCE_DIR}/plugins.cfg
		${CMAKE_SOURCE_DIR}/resources.cfg
		DESTINATION .
		CONFIGURATIONS Release RelWithDebInfo Debug
	)

endif(UNIX)

