if(MSVC)

foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)

endif(MSVC)


if (CMAKE_COMPILER_IS_GNUCC)
	set( MY_C_FLAGS -Wall -fPIC -Wdouble-promotion -Wmissing-include-dirs )
	set( CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   ${MY_C_FLAGS} )
	set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${MY_C_FLAGS} )
endif()
