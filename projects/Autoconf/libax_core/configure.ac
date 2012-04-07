AC_INIT(libax,[0.1],[email],libax)
AC_PREREQ(2.59)
AC_CANONICAL_TARGET

axCCFLAGS=
case ${target_os} in
darwin*) 
	axCCFLAGS="-x objective-c++" 
;;
esac

AC_SUBST(axCCFLAGS)

AC_CONFIG_SRCDIR(src)
AM_INIT_AUTOMAKE


AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
