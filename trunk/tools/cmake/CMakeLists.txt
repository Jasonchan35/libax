cmake_minimum_required(VERSION 2.8)
project(libax)


#-- svn ------
find_package(Subversion)
if(Subversion_FOUND)
#  Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} ER)
#  set(SUBVERSION_REVISION ${ER_WC_REVISION})
endif(Subversion_FOUND)

set(CPACK_PACKAGE_VERSION_PATCH "${SUBVERSION_REVISION}")

configure_file( 
	svn_revision.h.in  
	${CMAKE_HOME_DIRECTORY}/../../include/ax/svn_revision.h
)

configure_file( 
	svn_revision.cpp.in  
	svn_revision.cpp
)


#----------

include_directories( ../../include )
#-------
file( GLOB_RECURSE  h_files   RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../include/ax/*.h" )
file( GLOB_RECURSE  cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../src/*.cpp"  )

#set( h_files
#	../../include/ax/core/system/SOFile.h
#)
#set( cpp_files
#	../../src/core/system/SOFile.cpp
#)

foreach( F IN LISTS h_files )
#	message( STATUS "File: ${F}" )
	get_filename_component( O ${F} PATH )
	string(REPLACE "../../include/ax" "" O "${O}" )
	string(REPLACE "/" "\\\\" O "${O}" )
	
	if( "${O}" STREQUAL  "" )
		get_filename_component( O ${F} NAME_WE )
#		message( STATUS "group: ${O} ${F}" )
		source_group( "" FILES ${F} )	
	else()
#		message( STATUS "group: ${O} ${F}" )
		source_group( "${O}" FILES ${F} )
	endif()
endforeach()

foreach( F IN LISTS cpp_files )
	message( STATUS "File: ${F}" )
	get_filename_component( O ${F} PATH )
	string(REPLACE "../../src" "" O "${O}" )
	string(REPLACE "/" "\\\\" O "${O}" )
	
	if( "${O}" STREQUAL  "" )
		get_filename_component( O ${F} NAME_WE )
#		message( STATUS "group: ${O} ${F}" )
		source_group( "" FILES ${F} )	
	else()
#		message( STATUS "group: ${O} ${F}" )
		source_group( "${O}" FILES ${F} )
	endif()
endforeach()

add_library( libaxcore ${h_files} ${cpp_files} )
