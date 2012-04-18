cd `dirname "$0"`

echo "cpp_file_list=\\" > cpp_file_list.mk
echo "h_file_list=\\" > h_file_list.mk

add_cpp_dir() {
	find $1 -name "*.cpp" | sed -e "s/$/ \\\\/" -e"s/^/\t/" >> cpp_file_list.tmp
}

add_h_dir() {
#	(cd $h_dir && find $1 -name "*.h" | sed -e "s/$/ \\\\/" -e"s/^/\t/")	 >> h_file_list.tmp
	find $1 -name "*.h" | sed -e "s/$/ \\\\/" -e"s/^/\t/" >> h_file_list.tmp
}

add_h_file() {
#	(cd $h_dir && echo $1 | sed -e "s/$/ \\\\/" -e"s/^/\t/" ) >> h_file_list.tmp
	echo $1 | sed -e "s/$/ \\\\/" -e"s/^/\t/" >> h_file_list.tmp
}

#===== add scan folder here ====

h_dir="../../../include"

add_h_file "$h_dir/ax/ax_base.h"
add_h_dir  "$h_dir/ax/base"

add_h_file "$h_dir/ax/ax_core.h"
add_h_dir  "$h_dir/ax/core"

add_cpp_dir "../../../src/core"
#====================

#remove last slash
sed -e "$ s/\\\\/ /g" h_file_list.tmp >> h_file_list.mk
sed -e "$ s/\\\\/ /g" cpp_file_list.tmp >> cpp_file_list.mk

rm h_file_list.tmp
rm cpp_file_list.tmp

#echo "====== generate $PWD/cpp_file_list.mk ======"
#cat cpp_file_list.mk
#echo "============="


