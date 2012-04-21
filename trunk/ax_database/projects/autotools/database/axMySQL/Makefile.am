#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LIBRARIES = libaxMySQL.a

#######################################
# Build information for each library

include src_file_list.mk

# Sources for libTest
libaxMySQL_a_SOURCES = $(src_file_list)

# Linker options libTestProgram
#libaxMySQL_a_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libaxMySQL_a_CPPFLAGS = -I$(top_srcdir)/../ax_core/include -I$(top_srcdir)/include

