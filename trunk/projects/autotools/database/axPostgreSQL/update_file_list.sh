cd `dirname "$0"`

echo "cpp_file_list=\\" > cpp_file_list.mk

add_cpp_dir() {
	find $1 -name "*.cpp" | sed -e "s/$/ \\\\/" -e"s/^/ /" >> cpp_file_list.tmp
}

#===== add scan folder here ====
add_cpp_dir "../../../../src/database/axPostgreSQL/"

#====================

#remove last slash
sed -e "$ s/\\\\/ /g" cpp_file_list.tmp >> cpp_file_list.mk

rm cpp_file_list.tmp

#echo "====== generate $PWD/cpp_file_list.mk ======"
#cat cpp_file_list.mk
#echo "============="


