cmake_minimum_required (VERSION 2.6)
project (ax_core)

if(NOT axLIB_DIR)
	set( axLIB_DIR "lib" )
endif()

message( "CMAKE_SYSTEM           = ${CMAKE_SYSTEM}" )
message( "CMAKE_SYSTEM_NAME      = ${CMAKE_SYSTEM_NAME}" )
message( "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}" )
message( "CMAKE_SYSTEM_VERSION   = ${CMAKE_SYSTEM_VERSION}" )

message( "CMAKE_INSTALL_PREFIX   = ${CMAKE_INSTALL_PREFIX}" )
message( "axLIB_DIR              = ${axLIB_DIR}" )

add_subdirectory(projects/cmake)
