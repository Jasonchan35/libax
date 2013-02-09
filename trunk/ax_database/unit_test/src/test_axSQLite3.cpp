

#include <ax/ax_core.h>
#include <ax/database/axSQLite3.h>

#include <ax/ax_unit_test.h>

axStatus test_axSQLite3_case1() {
	axStatus st;
	axDBConn	db;
	st = axSQLite3_open( db, "test.db" );		if( !st ) return st;

	db.setEchoSQL( true );

	axDBStmt	stmt;

	st = stmt.create( db, "drop table if exists table001;" );			if( !st ) return st;
	st = stmt.exec();													if( !st ) return st;

	st = stmt.create( db, "create table table001 ( id int, name text );" );		if( !st ) return st;
	st = stmt.exec();															if( !st ) return st;

	st = stmt.createExec( db, "insert into table001 (id,name) values(?,?)", 5, "string ' testing" );		if( !st ) return st;

	st = stmt.create( db, "select id, name from table001" );		if( !st ) return st;
	st = stmt.exec();												if( !st ) return st;

	int			recId;
	axStringA	name;

	for(;;) {
		st = stmt.getRow( recId, name );	
		if( st.isEOF() ) break;
		if( !st ) return st;

		ax_log_var( recId );
		ax_log_var( name );
	}

	return 0;
}


class Row {
public:
	int64_t		my_id;
	axVec3f		vec3;
	float		float1;

	Row() {
		my_id = 0;
		vec3.set( 0,0,0 );
		float1 = 0;
	}

	template<class S>
	axStatus	serialize_io( S &s ) {
		axStatus st;
		ax_io( my_id );
		ax_io( vec3 );
		ax_io( float1 );
		return 0;
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?}, {?}, {?}", my_id, vec3, float1 );
	}
};


class Row2 {
public:
	int64_t		my_id;
	axVec3f		vec3;
    
	Row2() {
		my_id = 0;
		vec3.set( 0,0,0 );
	}
    
	template<class S>
	axStatus	serialize_io( S &s ) {
		axStatus st;
		ax_io( my_id );
		ax_io( vec3 );
		return 0;
	}
    
	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?}, {?}", my_id, vec3 );
	}
};


axStatus test_axSQLite3_case2() {
	axStatus st;
	axDBConn	db;
	st = axSQLite3_open( db, "test.db" );		if( !st ) return st;

	db.setEchoSQL( true );


	axStringA	table;
	st = table.set( "table002" );	if( !st ) return st;

	st = db.dropTableIfExists( table );					if( !st ) return st;
	st = db.createTable<Row>( table );					if( !st ) return st;

	axDBStmt	stmt;

	{//insert
		Row	row;
		axTempStringA	sql;
		st = db.createSQL_Insert( sql, table, row );		if( !st ) return st;
		st = stmt.create( db, sql );						if( !st ) return st;

		row.my_id = 10;
		row.float1 = 2.4f;
		row.vec3.set( 5,6,7 );
		st = stmt.exec( row );	if( !st ) return st;
	}

	{
		Row row;
		row.my_id = 100;
		row.vec3.set( 9, 10, 11 );
		axDBStmt_Insert<Row>	insertRow;
		st = insertRow.create( db, table );		if( !st ) return st;
		insertRow.exec( row );
	}

	{
		Row row;
		row.my_id = 100;
		row.vec3.set( 90, 100, 110 );
		axDBStmt_Update<Row, int64_t, &Row::my_id >	updateRow;
		//axDBStmt_Update<Row>	updateRow;

		st = updateRow.create( db, table );			if( !st ) return st;
		updateRow.exec( row );
	}

    
	{
		Row	row;
        
		axTempStringA	sql;
		st = db.createSQL_Select( sql, table, row, NULL );		if( !st ) return st;
		st = stmt.createExec( db, sql );						if( !st ) return st;
        
		for(;;) {
			st = stmt.getRow( row );	if( st.isEOF() ) break;
			if( !st ) return st;
            
			ax_log_var( row );
		}
	}
	{
	
		axDBStmt_Select< Row, int64_t, &Row::my_id > stmt;
		st = stmt.create( db, table );		if( !st ) return st;
		
		Row single_row;
		st = stmt.exec( single_row, 10 );		if( !st ) return st;
		ax_log_var( single_row );
		
	}

	
	{
		axDBTableAccessor< Row, int64_t, &Row::my_id > ta;
		
		st = ta.create( db, table ); if(!st ) return st;
		
		
		Row row;
		
		row.my_id = 20;
		row.float1 = 7.4f;
		row.vec3.set( 1,7,3 );
		st = ta.insert( row );		if( !st ) return st;

		row.float1 = 9999.4f;
		st = ta.update( row );		if( !st ) return st;
		
		ax_log_var( row );
		
		Row row_select;
		st = ta.select( row_select, 20 );		if( !st ) return st;
		
		ax_log_var( row_select );
	}
	//axDBStmt_Single<Row>	table1;

	//table1.create( "table1" );

	//table1.select( row, 10 );
	//table1.insert( row );
	//table1.update( row );
	//table1.deleteRow( 10 );
	//table1.createTable();


	return 0;
}


axStatus test_axSQLite3() {
	axStatus st;
//	axUTestCase( test_axSQLite3_case1 );
	axUTestCase( test_axSQLite3_case2 );

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