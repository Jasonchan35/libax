#include "axDBConn_ODBC.h"

axStatus	axODBC_connect( axDBConn & db, const char* dsn ) {
	axDBConn_ODBC* p = new axDBConn_ODBC();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( dsn );
}

axDBConn_ODBC::axDBConn_ODBC() {
	env_ = NULL;
	dbc_ = NULL;
}

axDBConn_ODBC::~axDBConn_ODBC() {
	close();
}

void axDBConn_ODBC::close() {
	if( dbc_ ) {
		SQLFreeHandle( SQL_HANDLE_DBC, dbc_ );
		dbc_ = NULL;
	}
	if( env_ ) {
		SQLFreeHandle( SQL_HANDLE_ENV, env_ );
		env_ = NULL;
	}
}

axStatus	axDBConn_ODBC::createStmt ( axDBStmt & stmt, const char * sql ) {
	axStatus st;
	axDBStmt_ODBC* p = new axDBStmt_ODBC( this );
	if( !p ) return axStatus_Std::not_enough_memory;
	stmt._setImp( p );
	return p->create( sql );
}

axStatus	axDBConn_ODBC::getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	tableName;
    
	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");
		}

		st = identifierString( colName, c.name );					if( !st ) return st;
		st = outSQL.appendFormat("  {?}\t{?}", colName, DBTypeName(c.type) );		if( !st ) return st;

		if( list.pkeyIndex() == i ) {
			st = outSQL.append( " PRIMARY KEY" );					if( !st ) return st;
			if( list.pkeyAutoInc() ) {
				st = outSQL.append( " AUTOINCREMENT" );				if( !st ) return st; //different DB might be different
			}
		}
	}

	st = outSQL.appendFormat( "\n);" );
	return 0;

}


axStatus	axDBConn_ODBC::connect	( const char* dsn ) {
	axStatus st;
	axTempStringW	tmp;
	st = tmp.set( dsn );		if( !st ) return st;
	return connect( tmp );
}

axStatus	axDBConn_ODBC::connect	( const wchar_t* dsn ) {
	close();

	axStatus st;

	SQLRETURN	ret = SQL_SUCCESS;
	ret = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_ );
	if( hasError(ret) ) {
		return axStatus_Std::DB_error_connect;
	}
	
	ret = SQLSetEnvAttr( env_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0 );
	if( hasError(ret) ) {
		return axStatus_Std::DB_error_connect;
	}

	ret = SQLAllocHandle( SQL_HANDLE_DBC, env_, &dbc_ );
	if( hasError(ret) ) {
		return axStatus_Std::DB_error_connect;
	}

	SQLSMALLINT dsn_len;
	st = ax_safe_assign( dsn_len, ax_strlen(dsn) );		if( !st ) return st;

	ret = SQLDriverConnect( dbc_, NULL, ax_const_cast(dsn), dsn_len, NULL, 0, NULL, SQL_DRIVER_NOPROMPT );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}
	return 0;
}

bool axDBConn_ODBC::hasError ( RETCODE code, const char* sql ) {
	switch( code ) {
		case SQL_SUCCESS:			return false;
		case SQL_SUCCESS_WITH_INFO:	return false;
	}
	return true;
}

void axDBConn_ODBC::logError() {
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iNativeError;
	const size_t len = 2040;
	WCHAR       wszMessage[len];
    WCHAR       wszState[SQL_SQLSTATE_SIZE+1];

	SQLRETURN ret;
	for(;;) {
		ret = SQLGetDiagRec( SQL_HANDLE_ENV, env_, ++iRec, wszState, &iNativeError, wszMessage, len, NULL );
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) break;
	}

	for(;;) {
		ret = SQLGetDiagRec( SQL_HANDLE_DBC, dbc_, ++iRec, wszState, &iNativeError, wszMessage, len, NULL );
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) break;
    }
}

const char*	axDBConn_ODBC::DBTypeName( int c_type ) {
	// SQL99 Standard ?
	switch( c_type ) {
		case axDB_c_type_bool:		return "BOOLEAN";
		case axDB_c_type_int8:		return "TINYINT";
		case axDB_c_type_int16:		return "SMALLINT";
		case axDB_c_type_int32:		return "INTEGER";
		case axDB_c_type_int64:		return "BIGINT";

		case axDB_c_type_float:		return "REAL";
		case axDB_c_type_double:	return "DOUBLE PRECISION";

		case axDB_c_type_StringA:	return "VARCHAR";
		case axDB_c_type_StringW:	return "VARCHAR";

		case axDB_c_type_ByteArray:	return "BINARY VARYING";
		case axDB_c_type_TimeStamp:	return "DATETIME";

	//	case axDB_c_type_GUID:		return "uniqueidentifier";
	}
	assert( false );
	return "Unknown";
}
