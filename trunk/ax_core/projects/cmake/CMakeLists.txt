CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libax_core)

if (MSVC)
	include(CheckTypeSize)
	CHECK_TYPE_SIZE(void* SIZEOF_PTR)
	if( SIZEOF_PTR EQUAL  4 )
		SET( axPLATFORM_DIR ${CMAKE_SYSTEM_NAME}/Win32 )
	elseif( SIZEOF_PTR EQUAL  8 )
		SET( axPLATFORM_DIR ${CMAKE_SYSTEM_NAME}/x64 )
	else()
		message( FATAL_ERROR "Unknown SIZEOF_PTR" )
	endif()
else()
	SET( axPLATFORM_DIR ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR} )
endif()

if(NOT axLIB_DIR)
	set( axLIB_DIR "lib" )
endif()


MESSAGE( "======== ${PROJECT_NAME} ============" )
MESSAGE( "CMAKE_SYSTEM           = ${CMAKE_SYSTEM}" )
MESSAGE( "CMAKE_SYSTEM_NAME      = ${CMAKE_SYSTEM_NAME}" )
MESSAGE( "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}" )
MESSAGE( "CMAKE_SYSTEM_VERSION   = ${CMAKE_SYSTEM_VERSION}" )
MESSAGE( "CMAKE_INSTALL_PREFIX   = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( "axLIB_DIR              = ${axLIB_DIR}" )
MESSAGE( "axPLATFORM_DIR = ${axPLATFORM_DIR}" )


INCLUDE_DIRECTORIES( ../../include )



SET( header_files_path   "../../include/ax" )
SET( src_files_path "../../src" )

FILE( GLOB_RECURSE  header_files	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${header_files_path}/*.h" )
FILE( GLOB_RECURSE  src_cpp_files	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${src_files_path}/*.cpp"  )
FILE( GLOB_RECURSE  src_h_files		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${src_files_path}/*.h"  )

FOREACH( f ${header_files} )
#	MESSAGE( STATUS "File: ${f}" )
	GET_FILENAME_COMPONENT( g ${f} PATH )
	STRING(REPLACE ${header_files_path} "" g "${g}" )
	STRING(REPLACE "/" "\\\\" g "${g}" )

	if( "${g}" STREQUAL  "" )
		GET_FILENAME_COMPONENT( g ${f} NAME_WE )
#		MESSAGE( STATUS "group .h: ${g} ${f}" )
		SOURCE_GROUP( "" FILES ${g} )
	ELSE()
#		MESSAGE( STATUS "group .h: ${g} ${f}" )
		SOURCE_GROUP( "${g}" FILES ${f} )
	ENDIF()
ENDFOREACH()

FOREACH( f ${src_cpp_files} ${src_h_files} )
#	MESSAGE( STATUS "File: ${f}" )
	GET_FILENAME_COMPONENT( g ${f} PATH )
	STRING(REPLACE ${src_files_path} "" g "${g}" )
	STRING(REPLACE "/" "\\\\" g "${g}" )

	IF( "${g}" STREQUAL  "" )
		GET_FILENAME_COMPONENT( g ${f} NAME_WE )
#		MESSAGE( STATUS "group .cpp: ${g} ${f}" )
		SOURCE_GROUP( "" FILES ${f} )
	ELSE()
#		MESSAGE( STATUS "group .cpp: ${g} ${f}" )
		SOURCE_GROUP( "${g}" FILES ${f} )
	ENDIF()
ENDFOREACH()

ADD_LIBRARY( ${PROJECT_NAME} ${header_files} ${src_cpp_files} ${src_h_files} )
SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ../../${axLIB_DIR}/${axPLATFORM_DIR}
)

if (MSVC)
	set( precompiled_header_h 	"../../src/precompiled_header/precompiled_header.h" )
	set( precompiled_header_cpp	"../../src/precompiled_header/precompiled_header.cpp" )
	set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yu${precompiled_header_h} /FI${precompiled_header_h}")
	set_source_files_properties	( ${precompiled_header_cpp}   PROPERTIES COMPILE_FLAGS "/Yc${precompiled_header_h}")
endif(MSVC)

INSTALL(DIRECTORY	"../../include/"
		DESTINATION include PATTERN .svn EXCLUDE )
		
INSTALL(FILES		"../../${axLIB_DIR}/${axPLATFORM_DIR}/${PROJECT_NAME}.a"
		DESTINATION ${axLIB_DIR} )


