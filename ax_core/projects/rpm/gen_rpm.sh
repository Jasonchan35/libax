cd `dirname "$0"`

NAME=libax_core
VER=1.1


rm -Rf $NAME-$VER
svn export ../../ $NAME-$VER
tar -czf SOURCES/$NAME-$VER.tar.gz $NAME-$VER
rm -Rf $NAME-$VER


echo "%_topdir $PWD" > ~/.rpmmacros

rpmbuild -ba SPECS/libax_core.spec
