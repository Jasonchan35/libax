#include <ax/ax_core.h>

#include <ax/ax_unit_test.h>

const size_t numRows = 5;



#define myTEST_TYPE_LIST \
	myTEST_TYPE( bool,		bool,			bool   ) \
	myTEST_TYPE( float,		float,			float  ) \
	myTEST_TYPE( double,	double,			double ) \
\
	myTEST_TYPE( int8,		int8_t,			int8_t  ) \
	myTEST_TYPE( int16,		int16_t,		int16_t ) \
	myTEST_TYPE( int32,		int32_t,		int32_t ) \
	myTEST_TYPE( int64,		int64_t,		int64_t ) \
\
	myTEST_TYPE( TimeStamp,	axTimeStamp,	axTimeStamp	 ) \
	myTEST_TYPE( ByteArray,	axByteArray,	axIByteArray ) \
	myTEST_TYPE( StringA,	axStringA,		axIStringA   ) \
	myTEST_TYPE( StringW,	axStringW,		axIStringW   ) \
\
	myTEST_TYPE( vec3f,		axVec3f,		axVec3f   ) \
//---------------

typedef int32_t		TableID;

class Row {
public:
	TableID			id;

	bool			v_bool;
	float			v_float;
	double			v_double;

	int8_t			v_int8;
	int16_t			v_int16;
	int32_t			v_int32;
	int64_t			v_int64;

	axStringA		v_StringA;
	axStringW		v_StringW;

	axByteArray		v_ByteArray;
	axTimeStamp		v_TimeStamp;

	axVec3f			v_vec3f;

	Row() {
		v_bool = false;
		v_int8 = 0;
		v_int16 = 0;
		v_int32 = 0;
		v_int64 = 0;
		v_float = 0;
		v_double = 0;
	}

	void testValue() {
		id = -1;

		v_int8  = 8;
		v_int16 = 16;
		v_int32 = 32;
		v_int64 = -12345689110064;

		v_bool	 = true;

		v_float  = 123.456f;
		v_double = 123456789.1234;

		v_StringA.set( "This is String" );
		v_StringW.set("StringW Test");

		v_TimeStamp.now();

		v_vec3f.set( 77,88,99 );
	}

	axStatus	onTake( Row & src ) {
		axStatus st;

		ax_take_macro( id );

		#define myTEST_TYPE( NAME, TYPE, C_ITYPE )	\
			ax_take_macro( v_##NAME );
		//----
			myTEST_TYPE_LIST
		#undef myTEST_TYPE

		return 0;
	}

	axStatus	toStringFormat( axStringFormat & f ) const {
		axStatus st;
		st = f.format("\n=== Row id={?} ===", id );

		#define myTEST_TYPE( NAME, TYPE, C_ITYPE )	\
			st = f.format("\n  {?} = {?}", "v_"#NAME, v_##NAME );		if( !st ) return st; \
		//----
			myTEST_TYPE_LIST
		#undef myTEST_TYPE

		return 0;
	}

	template<class S>
	axStatus serialize_io( S & s ) {
		axStatus st;
		ax_io( id );

		#define myTEST_TYPE( NAME, TYPE, C_ITYPE )	\
			ax_io( v_##NAME );
		//----
			myTEST_TYPE_LIST
		#undef myTEST_TYPE

		return 0;
	}
};

axStatus test_ax_database_common( axDBConn & db ) {
	axStatus st;

	db.setEchoSQL( true );

//	const char* table = "unit Test's \"Table\" 01";
//	const char* table = "Test's Table";
	const char* table = "TestTable";

	st = db.dropTableIfExists( table );								if( !st ) return st;
	axDBTableAccessor<Row, TableID, &Row::id>	tbl;

	st = tbl.createTable( db, table );			if( !st ) return st;


	st = tbl.create( db, table );				if( !st ) return st;


	{	ax_log("===== insert ======");
		Row	row;
		row.testValue();

		axStopWatch	timer;
		for( size_t i=0; i<numRows; i++ ) {
			st = tbl.insert( row );				if( !st ) return st;
		}
		ax_log("insert {?} records in {?}s", numRows, timer.get() );
	}

	{	ax_log("===== update ======");
		Row	row;
		row.testValue();

		axStopWatch	timer;
		for( size_t i=0; i<numRows; i++ ) {
			row.id = (TableID)i+1;
			row.v_bool = (i % 2 == 1);
			st = tbl.update( row );							if( !st ) return st;
			st = row.v_ByteArray.append( (uint8_t) i);		if( !st ) return st;
		}
		ax_log("update {?} records in {?}s", numRows, timer.get() );
	}

	{	ax_log("===== select all ======");
		axArray< Row >	results;
		results.reserve( numRows );

		axStopWatch	timer;
		st = tbl.selectAll( results );		if( !st ) return st;
		ax_log("select {?} records in {?}s", results.size(), timer.get() );

		//ax_log_var( results );
		#if 1 // dump last only
			if( results.size() ) {
				ax_log_var( results.last() );
			}
		#endif
	}

	return 0;
}

#include <ax/database/axSQLite3.h>
axStatus test_SQLite3() {
	axStatus st;
	axDBConn	db;

	st = axSQLite3_open( db, "test.db" );		if( !st ) return st;
//	st = axSQLite3_openMemory( db );			if( !st ) return st;

	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}

#if 0 //=============== MySQL ====================
#include <ax/database/axMySQL.h>
axStatus test_MySQL() {
	axStatus st;
	axDBConn	db;
	st = axMySQL_connect ( db, "test", "test", "1234", "localhost" );	if( !st ) return st;
	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}

#include <ax/database/axPostgreSQL.h>
axStatus test_PostgreSQL() {
	axStatus st;
	axDBConn	db;
	st = axPostgreSQL_connect ( db, "host=localhost port=5432 dbname=testdb user=test password=1234" );	if( !st ) return st;
	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}
#endif

#if 0 // =============== ODBC ====================
#include <ax/database/axODBC.h>
//axStatus test_ODBC() {
//	axStatus st;
//	axDBConn	db;
//	st = axODBC_connect ( db, "DRIVER={PostgreSQL ANSI}; DATABASE=testdb; SERVER=localhost;  PORT=5432;UID=test; PWD=1234;" );	if( !st ) return st;
//	st = test_ax_database_common(db);			if( !st ) return st;
//	return 0;
//}

axStatus test_ODBC_MSSQL() {
	axStatus st;
	axDBConn	db;
	st = axODBC_MSSQL_connect ( db, "MSSQL_DSN", "test", "1234");		if( !st ) return st;

	//st = axODBC_MSSQL_connect ( db, "DRIVER={SQL Server Native Client 10.0}; "
	//								"DATABASE=testdb; SERVER=192.168.1.56; UID=test; PWD=1234;");	if( !st ) return st;
	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}
axStatus test_ODBC_Oracle() {
	axStatus st;
	axDBConn	db;

//	st = axODBC_Oracle_connect ( db, "MyOracleDSN", "test", "1234" );	if( !st ) return st;
	st = axODBC_Oracle_connect ( db, "TonyOracle", "test", "1234" );	if( !st ) return st;

//	st = axODBC_Oracle_connectDSN ( db, "DSN=MyOracleDSN; UID=test; PWD=1234;" ); if( !st ) return st;
//	st = axODBC_Oracle_connect ( db, "DRIVER={Oracle in OraDb11g_home1}; UID=test; PWD=1234;" ); if( !st ) return st;
//	st = axODBC_Oracle_connect ( db, "DSN={TonyOracle}; UID=testdb; PWD=1234;" ); if( !st ) return st;
	st = test_ax_database_common(db);			if( !st ) return st;
	return 0;
}
#endif

axStatus test_ax_database() {
	axStatus st;
	axLog::instance->addFile( "test.log", false );

	ax_log("test {?} records\n", numRows );

	axUTestCase( test_SQLite3() );
//	axUTestCase( test_MySQL() );
//	axUTestCase( test_PostgreSQL() );
//	axUTestCase( test_ODBC_MSSQL() );
//	axUTestCase( test_ODBC_Oracle() );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;

	axStopWatch	timer;

	st = test_ax_database();
    printf("\n\n==== END of PROGRAM  return %d %s ====\n", st.code(), st.c_str() );

#if axOS_Win
	getchar();
#endif
	
    return 0;
}