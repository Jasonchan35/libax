CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libax_test)
MESSAGE( "======== ${PROJECT_NAME} ============" )

INCLUDE( ../common.cmake )

find_package( PostgreSQL )
INCLUDE_DIRECTORIES( ${PostgreSQL_INCLUDE_DIR} )

ENABLE_TESTING()

INCLUDE_DIRECTORIES( ../../include )
LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/${axPLATFORM_NAME} )

SET( my_libs  ${my_default_libs} ax )
SET( src_files_path  ${CMAKE_CURRENT_SOURCE_DIR}/../../test )

FILE( GLOB_RECURSE  src_cpp_files	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}	"${src_files_path}/*.cpp"  )

FOREACH( f IN LISTS src_cpp_files  )
	MESSAGE( STATUS "test file: ${f}" )
	GET_FILENAME_COMPONENT( path ${f} PATH )
	GET_FILENAME_COMPONENT( name ${f} NAME )


	ADD_EXECUTABLE( ${name} ${f} )
	SET_TARGET_PROPERTIES( ${name} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ../../../bin/${axPLATFORM_NAME}
	)

	TARGET_LINK_LIBRARIES( ${name} ${my_libs} ${PostgreSQL_LIBRARIES} )


	ADD_TEST( ${name} "${EXECUTABLE_OUTPUT_PATH}/${name}" Hello )

ENDFOREACH()
