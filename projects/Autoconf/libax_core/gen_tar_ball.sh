sh clean_gen.sh

echo ====== gen tar ball =======

svn export ../../src src

mkdir -p ax
svn export ../../../../include/ax/ax_base.h	ax/ax_base.h
svn export ../../../../include/ax/base		ax/base

svn export ../../../../include/ax/ax_core.h	ax/ax_core.h
svn export ../../../../include/ax/core		ax/core

mkdir -p ax/external
svn export ../../../../include/ax/external/glew	ax/external/glew


find src 		>  axSRC_file_list
find ax -name "*.h" 	>  axHEADER_file_list

sed -e "s|@axSRC@|`paste -s axSRC_file_list`|g" \
    -e "s|@axHEADER@|`paste -s axHEADER_file_list`|g" \
	 Makefile.am.in > Makefile.am

if ! aclocal; then exit; fi;
if ! autoconf; then exit; fi;
if ! automake -a; then exit; fi;
if ! ./configure; then exit; fi;
make dist

cp libax-0.1.tar.gz ~/rpmbuild/SOURCES/
