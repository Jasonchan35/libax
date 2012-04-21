cd `dirname "$0"`
echo === run $0 ====

sh include/update_file_list.sh
sh projects/autotools/axSQLite3/update_file_list.sh
sh projects/autotools/axMySQL/update_file_list.sh
sh projects/autotools/axPostgreSQL/update_file_list.sh
