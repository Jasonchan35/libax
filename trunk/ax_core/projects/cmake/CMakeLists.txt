CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libax_core)
MESSAGE( "======== ${PROJECT_NAME} ============" )

#INCLUDE( ../common.cmake )

INCLUDE_DIRECTORIES( ../../include )

SET( axPLATFORM_DIR ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR} )

SET( header_files_path   "../../include" )
SET( src_files_path "../../src" )

FILE( GLOB_RECURSE  header_files	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${header_files_path}/*.h" )
FILE( GLOB_RECURSE  src_cpp_files	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${src_files_path}/core/*.cpp"  )
FILE( GLOB_RECURSE  src_h_files		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${src_files_path}/core/*.h"  )

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

ADD_LIBRARY( ax_core ${header_files} ${src_cpp_files} ${src_h_files} )
SET_TARGET_PROPERTIES( ax_core PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ../../${axLIB_DIR}/${axPLATFORM_DIR}
)

if (MSVC)
	set( precompiled_header_h 	"../../src/core/precompiled_header/precompiled_header.h" )
	set( precompiled_header_cpp	"../../src/core/precompiled_header/precompiled_header.cpp" )
	
	set_target_properties( ax_core PROPERTIES COMPILE_FLAGS "/Yu${precompiled_header_h} /FI${precompiled_header_h}")
	set_source_files_properties	( ${precompiled_header_cpp}   PROPERTIES COMPILE_FLAGS "/Yc${precompiled_header_h}")

endif(MSVC)

INSTALL(DIRECTORY	"../../include/"
		DESTINATION include PATTERN .svn EXCLUDE )
		
INSTALL(FILES		"../../${axLIB_DIR}/${axPLATFORM_DIR}/libax_core.a"
		DESTINATION ${axLIB_DIR} )


