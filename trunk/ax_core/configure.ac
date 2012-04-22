dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libax_core, 1.1)
AC_PROG_CC
AC_PROG_CXX

AC_CANONICAL_SYSTEM

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
#LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries 
AC_PROG_LIBTOOL

HOST_CPU_COUNT=1
case $host in
	*darwin*)	HOST_CPU_COUNT=`sysctl -n hw.ncpu`;;
	*freebsd*)	HOST_CPU_COUNT=`sysctl -n hw.ncpu`;;
	*linux*)	HOST_CPU_COUNT=`grep -c processor /proc/cpuinfo`;;
esac
echo "HOST_CPU_COUNT = ${HOST_CPU_COUNT}"
AC_SUBST([HOST_CPU_COUNT])



AC_SUBST([MY_PROJECTS])
AC_CONFIG_FILES([Makefile])  

AC_DEFUN([MY_ADD_PROJECT], [ 
	AC_CONFIG_FILES([$1/Makefile])
	sh $1/update_file_list.sh
	MY_PROJECTS="$MY_PROJECTS $1"
])

                
#----- modules ---- 
MY_ADD_PROJECT(include)
MY_ADD_PROJECT(projects/autotools/ax_core)
                
AC_OUTPUT
