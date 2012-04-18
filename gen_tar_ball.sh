sh update_file_list.sh

if ! aclocal; then exit; fi;
if ! autoconf; then exit; fi;
if ! automake -a; then exit; fi;
if ! ./configure; then exit; fi;
make dist
