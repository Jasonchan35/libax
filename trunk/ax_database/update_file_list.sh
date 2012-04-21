cd `dirname "$0"`
echo === run $0 ====

sh include/update_file_list.sh
sh projects/autotools/database/axSQLite3/update_file_list.sh
sh projects/autotools/database/axMySQL/update_file_list.sh
sh projects/autotools/database/axPostgreSQL/update_file_list.sh
