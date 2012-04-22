cd `dirname "$0"`

NAME=libax_core
VER=1.1


rm -Rf SOURCES/$NAME-$VER
svn export ../../ SOURCES/$NAME-$VER
tar -czf SOURCES/$NAME-$VER.tar.gz SOURCES/$NAME-$VER
rm -Rf SOURCES/$NAME-$VER


echo "%_topdir $PWD" > ~/.rpmmacros

rpmbuild -ba SPECS/libax_core.spec
