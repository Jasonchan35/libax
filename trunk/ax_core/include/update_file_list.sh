cd `dirname "$0"`
echo === run $0 ====

echo "h_file_list=\\" > h_file_list.mk

add_h_dir() {
	find $1 -name "*.h" | sed -e "s/$/ \\\\/" -e"s/^/ /" >> h_file_list.tmp
}

add_h_file() {
	echo $1 | sed -e "s/$/ \\\\/" -e"s/^/ /" >> h_file_list.tmp
}

#===== add scan folder here ====

add_h_dir  "ax"

#====================

#remove last slash
sed -e "$ s/\\\\/ /g" h_file_list.tmp >> h_file_list.mk

rm h_file_list.tmp

#echo "====== generate $PWD/h_file_list.mk ======"
#cat h_file_list.mk
#echo "============="


