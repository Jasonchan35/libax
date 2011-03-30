CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libax_test)
MESSAGE( "======== ${PROJECT_NAME} ============" )

INCLUDE( ../common.cmake )

ENABLE_TESTING()

INCLUDE_DIRECTORIES( ../../include )
FILE( GLOB_RECURSE  test_cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../test/base/*.cpp"  )

ADD_EXECUTABLE( libax_test ${test_cpp_files} )
ADD_TEST(SimpleTest ${EXECUTABLE_OUTPUT_PATH}/libaxcore_test Hello)

#target_link_libraries( libax_test libax )