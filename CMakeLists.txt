project(LogiLCDStudio)

if(NOT DEFINED LCDSDK_PATH)
  set(LCDSDK_PATH "D:/LCDSDK_8.57.148/")
endif()

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( LCDSDK_LIB_PATH "${LCDSDK_PATH}/Lib/x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( LCDSDK_LIB_PATH "${LCDSDK_PATH}/Lib/x32" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

if(NOT WIN32)
	set(DISABLE_LOGILCDSTUDIO 1)
endif(NOT WIN32)

if(DISABLE_LOGILCDSTUDIO)
	message(STATUS "LogiLCDStudio plugin disabled")
	return()
endif()

set(LOGILCDSTUDIO_HEADERS
	DataFunctions.h
	LCDThreads.h
	mono_background.h
	#"${LCDSDK_PATH}/Include/LogitechLCDLib.h"
	)

set(LOGILCDSTUDIO_SOURCES
	LogiLCD.cpp
	ColourThread.cpp
	DataFunctions.cpp
	DualThread.cpp
	MonoThread.cpp
	)

add_library(LogiLCDStudio MODULE
	${LOGILCDSTUDIO_SOURCES}
	${LOGILCDSTUDIO_HEADERS})
	
target_link_libraries(LogiLCDStudio
	libobs
	obs-frontend-api
	"${LCDSDK_LIB_PATH}/LogitechLCDLib.lib"	
	)
	
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/UI/" "${LCDSDK_PATH}/Include/")

install_obs_plugin_with_data(LogiLCDStudio data)
