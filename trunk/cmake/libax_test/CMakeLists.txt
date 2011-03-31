CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libax_test)
MESSAGE( "======== ${PROJECT_NAME} ============" )

INCLUDE( ../common.cmake )

ENABLE_TESTING()

INCLUDE_DIRECTORIES( ../../include )
LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/${axPLATFORM_NAME} )

SET( my_libs  ${my_default_libs} ax )
SET( my_src_path  ${CMAKE_CURRENT_SOURCE_DIR}/../../test )

message( "${my_src_path}/core/database/test_axDBO_pgsql.cpp" )

ADD_EXECUTABLE( test_axDBO_pgsql  "${my_src_path}/core/database/test_axDBO_pgsql.cpp" )
TARGET_LINK_LIBRARIES( test_axDBO_pgsql ${my_libs} )
ADD_TEST( test_axDBO_pgsql ${EXECUTABLE_OUTPUT_PATH}/test_axDBO_pgsql Hello)

