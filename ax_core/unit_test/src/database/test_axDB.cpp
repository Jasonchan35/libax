#include <ax/axSQLite3.h>
#include <ax/axPostgreSQL.h>
#include <ax/axMySQL.h>

class axTest_SQLite : public axCommandLineApp {
public:
	axStatus	onRun() {
		axStatus st;		
//		st = test_sqlite();		ax_log("=== test_sqlite () return {?} ====\n\n", st );
//		st = test_postgre();	ax_log("=== test_postgre() return {?} ====\n\n", st );
		st = test_mysql();		ax_log("=== test_mysql  () return {?} ====\n\n", st );

		#if axOS_WIN
			_getch();
		#endif

		return st;
	}
	
	axStatus test_sqlite() {
		axPRINT_FUNC_NAME
		axStatus st;
		axDBConn	db;
		st = axSQLite3_open( db, ":memory:" );		if( !st ) return st;

		axDBStmt	stmt;
		st = db.exec(stmt,"drop table if exists table1;");		
		if( !st ) return st;
		
		st = db.exec(stmt,"create table table1 ( col_int integer primary key, col_string varchar, col_timestamp TIMESTAMP )" );	
		if( !st ) return st;

		return test_db( db );
	}
		
	axStatus test_postgre() {
		axPRINT_FUNC_NAME
		axStatus st;
		axDBConn	db;
		st = axPostgreSQL_connect( db, "host=127.0.0.1 dbname=testdb user=test password=1234" );		if( !st ) return st;		

		axDBStmt	stmt;
		st = db.exec(stmt,"drop table if exists table1;");		
		if( !st ) return st;
		
		st = db.exec(stmt,"create table table1 ( col_int integer, col_string varchar, col_timestamp timestamp without time zone, PRIMARY KEY(col_int) )" );	
		if( !st ) return st;	

		return test_db( db );
	}	

	axStatus test_mysql() {
		axPRINT_FUNC_NAME
		axStatus st;
		axDBConn	db;
		st = axMySQL_connect( db, "test", "test", "1234", NULL );		if( !st ) return st;		

		axDBStmt	stmt;
		st = db.exec(stmt,"drop table if exists table1;");		
		if( !st ) return st;
		
		st = db.exec(stmt,"create table table1 ( col_int integer, col_string varchar(255), col_timestamp timestamp, PRIMARY KEY(col_int) )" );	
		if( !st ) return st;	

		return test_db( db );
	}	
	
	axStatus test_db( axDBConn &db ) {
		axStatus st;
		axDBStmt	stmt;

		st = db.prepareStmt( stmt, "insert into table1 (col_int,col_string,col_timestamp) values(?,?,?);" );		
		if( !st ) return st;
		
		axDateTime	dt;
		dt.setDate( 2010, 4, 5 );
		//dt.UTC = true;

		st = stmt.exec( 123456, "apple", dt );		if( !st ) return st;

		st = db.exec( stmt, "select col_int, col_string, col_timestamp from table1" );		
		if( !st ) return st;		
		
		axStringA	col_string;
		int			col_int;
		axDateTime	col_timestamp;
		
		for(;;) {
			st = stmt.getRow( col_int, col_string, col_timestamp );	if( !st ) return st;
			ax_print( "{?}, {?}, {?}\n", col_int, col_string, col_timestamp );
		}
		return 0;
	}
	
};

axCommandLineApp_program_main("axTest_SQLite", axTest_SQLite )

