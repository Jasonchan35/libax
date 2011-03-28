#-- svn ------
FIND_PACKAGE(Subversion)
if(Subversion_FOUND)
	Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} ER)
	SET(SUBVERSION_REVISION ${ER_WC_REVISION})
endif(Subversion_FOUND)

SET(CPACK_PACKAGE_VERSION_PATCH "${SUBVERSION_REVISION}")

CONFIGURE_FILE( 
	svn_revision.h.in  
	${CMAKE_HOME_DIRECTORY}/../../include/ax/svn_revision.h
)

#----------

INCLUDE_DIRECTORIES( ../../include )
#-------
FILE( GLOB_RECURSE  h_files   RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../include/ax/*.h" )
FILE( GLOB_RECURSE  cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../src/*.cpp"  )

#set( h_files
#	../../include/ax/core/system/SOFile.h
#)
#set( cpp_files
#	../../src/core/system/SOFile.cpp
#)

FOREACH( f IN LISTS h_files )
#	MESSAGE( STATUS "File: ${f}" )
	GET_FILENAME_COMPONENT( g ${f} PATH )
	STRING(REPLACE "../../include/ax" "" g "${g}" )
	STRING(REPLACE "/" "\\\\" g "${g}" )
	
	if( "${g}" STREQUAL  "" )
		GET_FILENAME_COMPONENT( g ${f} NAME_WE )
#		MESSAGE( STATUS "group: ${g} ${f}" )
		SOURCE_GROUP( "" FILES ${g} )	
	ELSE()
#		MESSAGE( STATUS "group: ${g} ${f}" )
		SOURCE_GROUP( "${g}" FILES ${f} )
	ENDIF()
ENDFOREACH()

FOREACH( f IN LISTS cpp_files )
#	MESSAGE( STATUS "File: ${f}" )
	GET_FILENAME_COMPONENT( g ${f} PATH )
	STRING(REPLACE "../../src" "" g "${g}" )
	STRING(REPLACE "/" "\\\\" g "${g}" )
	
	IF( "${g}" STREQUAL  "" )
		GET_FILENAME_COMPONENT( g ${f} NAME_WE )
#		MESSAGE( STATUS "group: ${g} ${f}" )
		SOURCE_GROUP( "" FILES ${f} )	
	ELSE()
#		MESSAGE( STATUS "group: ${g} ${f}" )
		SOURCE_GROUP( "${g}" FILES ${f} )
	ENDIF()
ENDFOREACH()

ADD_LIBRARY( libax ${h_files} ${cpp_files} )
