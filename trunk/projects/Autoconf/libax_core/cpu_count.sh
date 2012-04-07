axOS=`uname`

case ${axOS} in
darwin*)
	echo sysctl -n hw.ncpu
;;
FreeBSD*)
	echo sysctl -n hw.ncpu
;;
Linux*)
	grep -c processor /proc/cpuinfo
;;
*)
	echo 1
esac
