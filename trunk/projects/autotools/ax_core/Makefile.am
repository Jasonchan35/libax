#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LIBRARIES = libax_core.a

#######################################
# Build information for each library

include src_file_list.mk

# Sources for libTest
libax_core_a_SOURCES = $(cpp_file_list)

# Linker options libTestProgram
#libax_core_a_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libax_core_a_CPPFLAGS = -I$(top_srcdir)/include

