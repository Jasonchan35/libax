cd `dirname "$0"`
echo === run $0 ====

echo "src_file_list=\\" > src_file_list.mk

add_src_dir() {
	find $1 -name "*.cpp" | sed -e "s/$/ \\\\/" -e"s/^/ /" >> src_file_list.tmp
	find $1 -name "*.h" | sed -e "s/$/ \\\\/" -e"s/^/ /" >> src_file_list.tmp
}

#===== add scan folder here ====
add_src_dir "../../../src/database/axMySQL/"

#====================

#remove last slash
sed -e "$ s/\\\\/ /g" src_file_list.tmp >> src_file_list.mk

rm src_file_list.tmp

#echo "====== generate $PWD/src_file_list.mk ======"
#cat src_file_list.mk
#echo "============="


