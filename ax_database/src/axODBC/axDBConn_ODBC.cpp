#include "axDBConn_ODBC.h"

axStatus	axODBC_connect( axDBConn & db, const char* server, const char* username, const char* password ) {
	axDBConn_ODBC* p = new axDBConn_ODBC();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( server, username, password );
}

axDBConn_ODBC::axDBConn_ODBC() {
	env_ = NULL;
	dbc_ = NULL;
	lastExecStmt_ = NULL;
	directExecStmt_ = NULL;
}

axDBConn_ODBC::~axDBConn_ODBC() {
	close();
}

void axDBConn_ODBC::close() {
	if( directExecStmt_ ) {
		SQLFreeHandle( SQL_HANDLE_STMT, directExecStmt_ );
		directExecStmt_ = NULL;
	}
	if( dbc_ ) {
		SQLDisconnect( dbc_ );
		SQLFreeHandle( SQL_HANDLE_DBC, dbc_ );
		dbc_ = NULL;
	}
	if( env_ ) {
		SQLFreeHandle( SQL_HANDLE_ENV, env_ );
		env_ = NULL;
	}
}

axStatus	axDBConn_ODBC::_directExec( const char* sql ) {
	if( echoSQL_ ) {
		ax_log("--- ExecSQL: ---\n{?}\n", sql );
	}	

	if( lastExecStmt_ ) {
		SQLCloseCursor( lastExecStmt_ );
		lastExecStmt_ = NULL;
	}

	SQLRETURN ret;
	ret = SQLExecDirectA( directExecStmt_, (SQLCHAR*)sql, SQL_NTS );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error;
	}
	return 0;
}

axStatus	axDBConn_ODBC::savePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );			if( !st ) return st;
	st = tmp.format("SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_ODBC::rollBackToSavePoint	( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );						if( !st ) return st;
	st = tmp.format("ROLLBACK TO SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_ODBC::releaseSavePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );					if( !st ) return st;
	st = tmp.format("RELEASE SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}


axStatus	axDBConn_ODBC::beginTran	() { 		
	SQLRETURN ret;
	ret = SQLSetConnectAttr( dbc_, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, 0 );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_transaction;
	}
	return 0; 
}

axStatus	axDBConn_ODBC::rollBackTran	() {
	SQLRETURN ret;
	ret = SQLEndTran( SQL_HANDLE_DBC , dbc_, SQL_ROLLBACK );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_transaction;
	}
	ret = SQLSetConnectAttr( dbc_, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0  );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_transaction;
	}
	return 0; 
}

axStatus	axDBConn_ODBC::commitTran	() { 		
	SQLRETURN ret;
	ret = SQLEndTran( SQL_HANDLE_DBC , dbc_, SQL_COMMIT );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_transaction;
	}
	ret = SQLSetConnectAttr( dbc_, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0  );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_transaction;
	}
	return 0; 
}

axStatus	axDBConn_ODBC::createStmt ( axDBStmt & stmt, const char * sql ) {
	axStatus st;
	axDBStmt_ODBC* p = new axDBStmt_ODBC( this );
	if( !p ) return axStatus_Std::not_enough_memory;
	stmt._setImp( p );
	return p->create( sql );
}

axStatus	axDBConn_ODBC::getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
	axStatus st;
	axTempStringA	tableName;
    
	st = outSQLArray.resize(1);			if( !st ) return st;
	axIStringA & outSQL = outSQLArray[0];

	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");			if( !st ) return st;
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

axStatus	axDBConn_ODBC::_preConnect() {
	close();

	axStatus st;

	SQLRETURN	ret;
	ret = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_ );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}
	
	ret = SQLSetEnvAttr( env_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0 );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}

	ret = SQLAllocHandle( SQL_HANDLE_DBC, env_, &dbc_ );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}

	return 0;
}

axStatus axDBConn_ODBC::_postConnect() {
	SQLRETURN ret;
	ret = SQLAllocHandle( SQL_HANDLE_STMT, dbc_, & directExecStmt_ ); 
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}
	return 0;
}

axStatus	axDBConn_ODBC::connectDSN	( const char* dsn ) {
	axStatus st;
	axTempStringW	w_dsn;
	st = w_dsn.set( dsn );		if( !st ) return st;
	return connectDSN( w_dsn );
}

axStatus	axDBConn_ODBC::connectDSN	( const wchar_t* dsn ) {
	if( ! dsn   ) return axStatus_Std::DB_error_connect;

	axStatus st;
	st = _preConnect();		if( !st ) return st;

	SQLSMALLINT dsn_len;
	st = ax_safe_assign( dsn_len, ax_strlen(dsn) );		if( !st ) return st;

	SQLRETURN	ret;
	ret = SQLDriverConnect( dbc_, NULL, ax_const_cast(dsn), dsn_len, NULL, 0, NULL, SQL_DRIVER_NOPROMPT );

	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}

	return _postConnect();
}

axStatus	axDBConn_ODBC::connect	( const char* server, const char* username, const char* password ) {
	axStatus st;
	axStringW_<128>	w_server;		st = w_server.set  ( server );			if( !st ) return st;
	axStringW_<128>	w_username;		st = w_username.set( username );		if( !st ) return st;
	axStringW_<128>	w_password;		st = w_password.set( password );		if( !st ) return st;

	return connect( w_server, w_username, w_password );
}

axStatus	axDBConn_ODBC::connect	( const wchar_t* server, const wchar_t* username, const wchar_t* password ) {
	if( ! server   ) return axStatus_Std::DB_error_connect;
	if( ! username ) return axStatus_Std::DB_error_connect;
	if( ! password ) return axStatus_Std::DB_error_connect;

	axStatus st;
	st = _preConnect();		if( !st ) return st;

	SQLRETURN	ret;
	ret = SQLConnect( dbc_, ax_const_cast(server),		SQL_NTS,
							ax_const_cast(username),	SQL_NTS,
							ax_const_cast(password),	SQL_NTS );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error_connect;
	}

	return _postConnect();
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

	iRec = 0;
	for(;;) {
		ret = SQLGetDiagRec( SQL_HANDLE_ENV, env_, ++iRec, wszState, &iNativeError, wszMessage, len, NULL );
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) break;
		ax_log("ODBC Error [{?}] [{?}]: {?}", wszState, (int)iNativeError, wszMessage );
	}

	iRec = 0;
	for(;;) {
		ret = SQLGetDiagRec( SQL_HANDLE_DBC, dbc_, ++iRec, wszState, &iNativeError, wszMessage, len, NULL );
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) break;
		ax_log("ODBC Error [{?}] [{?}]: {?}", wszState, (int)iNativeError, wszMessage );
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

		case axDB_c_type_uint8:		return "TINYINT UNSIGNED";
		case axDB_c_type_uint16:	return "SMALLINT UNSIGNED";
		case axDB_c_type_uint32:	return "INTEGER UNSIGNED";
		case axDB_c_type_uint64:	return "BIGINT UNSIGNED";

		case axDB_c_type_float:		return "REAL";
		case axDB_c_type_double:	return "DOUBLE PRECISION";

		case axDB_c_type_StringA:	return "VARCHAR";
		case axDB_c_type_StringW:	return "VARCHAR";

		case axDB_c_type_blob:	return "BINARY VARYING";
		case axDB_c_type_datetime:	return "DATETIME";

	//	case axDB_c_type_GUID:		return "uniqueidentifier";
	}
	assert( false );
	return "Unknown";
}
