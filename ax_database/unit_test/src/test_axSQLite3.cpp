

#include <ax/ax_core.h>
#include <ax/database/axSQLite3.h>

#include <ax/ax_unit_test.h>

axStatus test_ax_SQLite3_case1() {
	axStatus st;
	axDBConn	db;
	st = axSQLite3_open( db, "test.db" );		if( !st ) return st;

	axDBStmt	stmt;

	st = stmt.create( db, "drop table if exists table001;" );			if( !st ) return st;
	st = stmt.exec();		if( !st ) return st;

	st = stmt.create( db, "create table table001 ( id int, name text );" );		if( !st ) return st;
	st = stmt.exec();		if( !st ) return st;

	st = stmt.createExec( db, "insert into table001 (id,name) values(?,?)", 5, "string ' testing" );		if( !st ) return st;

	st = stmt.create( db, "select id, name from table001" );		if( !st ) return st;
	st = stmt.exec();		if( !st ) return st;

	for(;;) {
		int			recId;
		axStringA	name;

		st = stmt.getRow( recId, name );	
		if( st.isEOF() ) break;
		if( !st ) return st;

		ax_log_var( recId );
		ax_log_var( name );
	}


	return 0;
}


axStatus test_axSQLite3() {
	axStatus st;
	axUTestCase( test_ax_SQLite3_case1 );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = test_axSQLite3();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}