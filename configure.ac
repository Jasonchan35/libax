dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libax, 1.1)
AC_PROG_CC

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


case $host in
	*FreeBSD*)	HOST_CPU_COUNT=`sysctl -n hw.ncpu`;;
	*linux*)	HOST_CPU_COUNT=`grep -c processor /proc/cpuinfo`;;
esac
echo "HOST_CPU_COUNT = ${HOST_CPU_COUNT}"
AC_SUBST([HOST_CPU_COUNT])


AC_CONFIG_FILES(Makefile
				include/Makefile
                src/core/Makefile)
AC_OUTPUT