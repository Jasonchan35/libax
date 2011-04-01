include(CheckTypeSize)
CHECK_TYPE_SIZE(void* SIZEOF_PTR)
message("size of pointer = ${SIZEOF_PTR}")


SET( axPLATFORM_NAME  "noarch" )

if(MSVC)
	foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/MD")
		if(${flag_var} MATCHES "/MDd")
			string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/MDd")
	endforeach(flag_var)

	if( SIZEOF_PTR EQUAL  4 )
		SET( axPLATFORM_NAME  "Win32" )
	elseif( SIZEOF_PTR EQUAL  8 )
		SET( axPLATFORM_NAME  "Win64" )
	else()
		message( FATAL_ERROR "Unknown SIZEOF_PTR" )
	endif()

	SET( my_default_libs ${my_default_libs}	libpq )
endif(MSVC)


if (CMAKE_COMPILER_IS_GNUCC)
#	set( MY_C_FLAGS "-Wall -fPIC -Wdouble-promotion -Wmissing-include-dirs" )
#	set( CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   ${MY_C_FLAGS} )
#	set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${MY_C_FLAGS} )

	SET( my_default_libs ${my_default_libs}	dl pthread pq )
endif()

add_definitions( -D${axPLATFORM_NAME} )

message( "CMAKE_SYSTEM           = ${CMAKE_SYSTEM}" )
message( "CMAKE_SYSTEM_NAME      = ${CMAKE_SYSTEM_NAME}" )
message( "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}" )
message( "CMAKE_SYSTEM_VERSION   = ${CMAKE_SYSTEM_VERSION}" )

message( "axPLATFORM             = ${axPLATFORM_NAME}" )

