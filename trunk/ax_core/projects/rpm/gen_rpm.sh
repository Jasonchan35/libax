cd `dirname "$0"`

#sh gen_tar_ball.sh

echo ======= gen_rpm =======

echo "%_topdir $PWD" > ~/.rpmmacros
cp ../../libax_core-1.1.tar.gz SOURCES

rpmbuild -ba SPECS/libax_core.spec