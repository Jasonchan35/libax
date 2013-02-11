#include <ax/ax_core.h>

#include <ax/ax_unit_test.h>

#include <ax/database/axSQLite3.h>
#include <ax/database/axMySQL.h>
#include <ax/database/axPostgreSQL.h>


class Row {
public:
	int64_t			id;

	#define axDB_c_type_list( NAME, TYPE, C_ITYPE )	\
		TYPE		v_##NAME; \
	//----
		#include <ax/core/database/axDB_c_type_list.h>
	#undef axDB_c_type_list

	Row() {
		id = -1;

		v_int8  = 8;
		v_int16 = 16;
		v_int32 = 32;
		v_int64 = 64;

		v_bool	 = true;

		v_float  = 123.456f;
		v_double = 789.1234;

		v_StringA.set( "This is String" );
		v_StringW.set( L"This is WString" );

		v_ByteArray.resize( 10 );
		for( size_t i=0; i<v_ByteArray.size(); i++ ) {
			v_ByteArray[i] = i;
		}

		v_TimeStamp.now();
	}

	axStatus	onTake( Row & src ) {
		axStatus st;

		ax_take_macro( id );

		#define axDB_c_type_list( NAME, TYPE, C_ITYPE )	\
			ax_take_macro( v_##NAME );
		//----
			#include <ax/core/database/axDB_c_type_list.h>
		#undef axDB_c_type_list

		return 0;
	}

	axStatus	toStringFormat( axStringFormat & f ) const {
		axStatus st;
		st = f.format("\n=== Row id={?} ===", id );

		#define axDB_c_type_list( NAME, TYPE, C_ITYPE )	\
			st = f.format("\n  {?} = {?}", "v_"#NAME, v_##NAME );		if( !st ) return st; \
		//----
			#include <ax/core/database/axDB_c_type_list.h>
		#undef axDB_c_type_list

		return 0;
	}

	template<class S>
	axStatus serialize_io( S & s ) {
		axStatus st;
		ax_io( id );

		#define axDB_c_type_list( NAME, TYPE, C_ITYPE )	\
			ax_io( v_##NAME );
		//----
			#include <ax/core/database/axDB_c_type_list.h>
		#undef axDB_c_type_list

		return 0;
	}
};

axStatus test_ax_database_common( axDBConn & db ) {
	axStatus st;

//	db.setEchoSQL( true );
	const size_t numRows = 10000;

	const char* table = "unitTestTable01";

	st = db.dropTableIfExists( table );					if( !st ) return st;
	st = db.createTable<Row>( table, "id", true );		if( !st ) return st;

	axDBTableAccessor<Row>	tbl;
	st = tbl.create( db, table );		if( !st ) return st;

	{	
		Row	row;

		axStopWatch	timer;
		for( size_t i=0; i<numRows; i++ ) {
			row.v_bool = (i % 2 == 0);
			st = tbl.insert( row );			if( !st ) return st;
		}
		ax_log("insert {?} records in {?}s", numRows, timer.get() );
	}

	{	
		Row	row;

		axStopWatch	timer;
		for( size_t i=0; i<numRows; i++ ) {
			row.id = i+1;
			st = tbl.update( row );			if( !st ) return st;
		}
		ax_log("update {?} records in {?}s", numRows, timer.get() );
	}

	{
		axArray< Row >	results;
		results.reserve( numRows );

		axStopWatch	timer;
		st = tbl.selectAll( results );		if( !st ) return st;
		ax_log("select {?} records in {?}s", results.size(), timer.get() );
		//ax_log_var( results );
	}

	return 0;
}

axStatus test_SQLite3() {
	axStatus st;
	axDBConn	db;

	st = axSQLite3_open( db, "test.db" );		if( !st ) return st;
//	st = axSQLite3_openMemory( db );			if( !st ) return st;

	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}

axStatus test_MySQL() {
	axStatus st;
	axDBConn	db;
	st = axMySQL_connect ( db, "test", "test", "1234", "localhost" );	if( !st ) return st;
	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}


axStatus test_PostgreSQL() {
	axStatus st;
	axDBConn	db;
	st = axPostgreSQL_connect ( db, "host=localhost port=5432 dbname=testdb user=test password=1234" );	if( !st ) return st;
	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}



axStatus test_ax_database() {
	axStatus st;

//	axUTestCase( test_SQLite3() );
//	axUTestCase( test_MySQL() );
	axUTestCase( test_PostgreSQL() );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;

	axStopWatch	timer;

	st = test_ax_database();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );

	ax_log_var( timer );
#if axOS_Win
	getchar();
#endif
	
    return 0;
}