CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libax)
MESSAGE( "======== ${PROJECT_NAME} ============" )

INCLUDE( ../common.cmake )

#-- svn ------
FIND_PACKAGE(Subversion)
#if(Subversion_FOUND)
#	Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} ER)
#	SET(SUBVERSION_REVISION ${ER_WC_REVISION})
#endif(Subversion_FOUND)

#SET(CPACK_PACKAGE_VERSION_PATCH "${SUBVERSION_REVISION}")

#CONFIGURE_FILE(
#	svn_revision.h.in
#	${CMAKE_HOME_DIRECTORY}/../../include/ax/svn_revision.h
#)

#----------

SET( h_files_path   "../../include/ax" )
SET( cpp_files_path "../../src" )
INCLUDE_DIRECTORIES( ../../include )

#-------
FILE( GLOB_RECURSE  h_files   RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${h_files_path}/*.h" )
FILE( GLOB_RECURSE  cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${cpp_files_path}/*.cpp"  )

FOREACH( f IN LISTS h_files )
#	MESSAGE( STATUS "File: ${f}" )
	GET_FILENAME_COMPONENT( g ${f} PATH )
	STRING(REPLACE ${h_files_path} "" g "${g}" )
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

FOREACH( f IN LISTS cpp_files )
#	MESSAGE( STATUS "File: ${f}" )
	GET_FILENAME_COMPONENT( g ${f} PATH )
	STRING(REPLACE ${cpp_files_path} "" g "${g}" )
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

ADD_LIBRARY( ax ${h_files} ${cpp_files} )
SET_TARGET_PROPERTIES( ax PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ../../../lib
)
