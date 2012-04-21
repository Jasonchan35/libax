dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libax_database, 1.1)
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

AC_ARG_WITH([sqlite3],
            [AS_HELP_STRING([--with-sqlite3], 
            [use sqlite3 (default is yes)])], 
            [use_sqlite3=$withval], 
            [use_sqlite3=yes])

AC_CONFIG_FILES(Makefile
				include/Makefile
                projects/autotools/database/axSQLite3/Makefile 
                projects/autotools/database/axMySQL/Makefile 
                projects/autotools/database/axPostgreSQL/Makefile 
                )
               
                
AC_OUTPUT


echo use_sqlite3 = $use_sqlite3

