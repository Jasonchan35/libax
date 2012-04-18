#sh gen_tar_ball.sh

echo ======= gen_rpm =======
 
echo "%_topdir ~/rpmbuild" > ~/.rpmmacros

rmdir ~/rpmbuild
mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS,tmp}
cp rpm/libax.spec ~/rpmbuild/SPECS/
cp ../libax-1.1.tar.gz ~/rpmbuild/SOURCES

rpmbuild -ba ~/rpmbuild/SPECS/libax.spec

rmdir rpmbuild
mkdir -p rpmbuild
cp -Rf ~/rpmbuild/RPMS	rpmbuild/
cp -Rf ~/rpmbuild/SRPMS	rpmbuild/
