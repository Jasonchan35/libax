#include "axDBStmt_ODBC.h"
#include "axDBConn_ODBC.h"

axDBStmt_ODBC::axDBStmt_ODBC( axDBConn_ODBC* db ) {
	db_.ref( db );
	stmt_ = NULL;
}

axDBStmt_ODBC::~axDBStmt_ODBC() {
	destroy();
}

axStatus	axDBStmt_ODBC::create ( const char * sql ) {
	destroy();

	axStatus st;

	st = sql_.set( sql );		if( !st ) return st;
	if( db_->echoSQL() ) {
		ax_log("--- CreateStmt SQL: ---\n{?}\n", sql_ );
	}

	SQLRETURN	ret;

	ret = SQLAllocHandle( SQL_HANDLE_STMT, db_->dbc_, & stmt_ ); 
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error_prepare_stmtement;
	}

	axTempStringW	w_sql;
	st = w_sql.set( sql );		if( !st ) return st;
	ret = SQLPrepare( stmt_, ax_const_cast( w_sql.c_str() ), (SQLINTEGER) w_sql.size() );
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error_prepare_stmtement;
	}

	SQLSMALLINT	paramCount = 0;
	ret = SQLNumParams( stmt_, &paramCount );
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error_prepare_stmtement;
	}

	paramCount_ = paramCount;
	return 0;
}

void axDBStmt_ODBC::destroy() {
	if( stmt_ ) {
		SQLFreeHandle( SQL_HANDLE_STMT, stmt_ );
		stmt_ = NULL;
	}	
}

bool axDBStmt_ODBC::hasError ( RETCODE code ) {
	switch( code ) {
		case SQL_SUCCESS:			return false;
		case SQL_SUCCESS_WITH_INFO:	return false;
	}
	return true;
}

void axDBStmt_ODBC::logError() {
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iNativeError;
	const size_t len = 2040;
	WCHAR       wszMessage[len];
    WCHAR       wszState[SQL_SQLSTATE_SIZE+1];

	SQLRETURN ret;
	for(;;)
    {
		ret = SQLGetDiagRec( SQL_HANDLE_STMT, stmt_, ++iRec, wszState, &iNativeError, wszMessage, len, NULL );
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) break;

		// Hide data truncated..
		if( wcsncmp(wszState, L"01004", 5) ) {
			ax_log("ODBC Error [{?}] [{?}]: {?}\nSQL:\n{?}\n", wszState, (int)iNativeError, wszMessage, sql_ );
		}
    }
}

axStatus	axDBStmt_ODBC::exec_ParamList	( const axDBParamList & list ) { 
	axStatus st;
	SQLRETURN	ret = SQL_ERROR;

	if( list.size() < paramCount_ )	return axStatus_Std::DB_invalid_param_count;

	st = tmpStrData.resize	( list.size() );			if( !st ) return st;
	st = cbLen.resize		( list.size() );			if( !st ) return st;
	cbLen.setAll(0);

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBParam & param = list[i];
		SQLLEN			&len	= cbLen[i];
		SQLUSMALLINT	col		= (SQLUSMALLINT) i+1;

		switch( param.type ) {
			case axDB_c_type_int8: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_STINYINT, SQL_TINYINT,	0, 0, ax_const_cast(&param.v_int8), 0, &len );
			}break;
			case axDB_c_type_int16: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_SSHORT,	SQL_SMALLINT,	0, 0, ax_const_cast(&param.v_int16), 0, &len );
			}break;
			case axDB_c_type_int32: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_SLONG,	SQL_INTEGER,	0, 0, ax_const_cast(&param.v_int32), 0, &len );
			}break;
			case axDB_c_type_int64: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_SBIGINT,	SQL_BIGINT,		0, 0, ax_const_cast(&param.v_int64), 0, &len );
			}break;

			case axDB_c_type_bool: {
				//using tmpStrData as int8 for bool
				st = tmpStrData[i].resize(1);		if( !st ) return st;
				int8_t* tmp = (int8_t*)tmpStrData[i]._getInternalBufferPtr();
				*tmp = param.v_bool ? 1 : 0;
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_STINYINT,	SQL_TINYINT,	0, 0, tmp, 0, &len );
			}break;

			case axDB_c_type_float: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_FLOAT,	SQL_REAL,		0, 0, ax_const_cast(&param.v_float),  0, &len );
			}break;

			case axDB_c_type_double: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_DOUBLE,	SQL_DOUBLE,		0, 0, ax_const_cast(&param.v_double), 0, &len );
			}break;

			case axDB_c_type_StringA: {
				len = SQL_NTS;
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_CHAR,		SQL_VARCHAR,	0, 0, ax_const_cast(param.v_strA), 0, &len );
			}break;

			case axDB_c_type_StringW: {
				st = tmpStrData[i].set( param.v_strW );		if( !st ) return st;
				len = SQL_NTS;
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_CHAR,		SQL_VARCHAR,	0, 0, ax_const_cast(tmpStrData[i].c_str()), 0, &len );
			}break;

			case axDB_c_type_ByteArray: {
				const axIByteArray*	data = param.v_ByteArray;
				len = data->byteSize();
				const uint8_t* ptr = data->ptr();
				if( len == 0 ) {
					ptr = (uint8_t*)1; // ptr cannot be NULL anyway even size = 0
				}
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_BINARY,	SQL_VARBINARY,	0, 0, ax_const_cast(ptr), 0, &len );
			}break;

			case axDB_c_type_TimeStamp: {
				//using tmpStrData as buffer

				st = tmpStrData[i].resize( sizeof( TIMESTAMP_STRUCT ) );		if( !st ) return st;
				SQL_TIMESTAMP_STRUCT *ts = (TIMESTAMP_STRUCT*) tmpStrData[i]._getInternalBufferPtr();

				axDateTime	dt( param.v_TimeStamp );

				ts->year		= dt.year;
				ts->month		= dt.month;
				ts->day			= dt.day;
				ts->hour		= dt.hour;
				ts->minute		= dt.minute;
				ts->second		= (SQLUSMALLINT)dt.second;

				//seems not working on MSSQL
				//double int_part; 
				//ts->fraction = (SQLUINTEGER)( ax_modf( dt.second, &int_part ) * 1000000000 ); //nano-second
				ts->fraction = 0;

				SQLSMALLINT paramType = 0;
				SQLULEN		paramSize = 0;
				SQLSMALLINT paramDecimalDigits = 0;
				SQLSMALLINT paramNullable = 0;

				len = sizeof(SQL_TIMESTAMP_STRUCT);
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
					SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 27, 7, ts, len, &len );
			}break;

			default: {
				assert( false );
				len = SQL_NULL_DATA;
				return axStatus_Std::DB_invalid_param_type;
			}break;
		}

		if( hasError(ret) ) {
			logError();
			return axStatus_Std::DB_error;
		}
	}


	ret = SQLExecute( stmt_ );
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error;
	}

//=====
	SQLSMALLINT	numCols;
	ret = SQLNumResultCols( stmt_, &numCols );
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error;
	}

	st = columnInfo.resize( numCols );		if( !st ) return st;

	axTempStringW	wstr;
	for( size_t i=0; i<columnInfo.size(); i++ ) {
		ResultCol & col = columnInfo[i];
		SQLSMALLINT colNameLen;
		//get the name length
		ret = SQLDescribeCol( stmt_, i+1, NULL, 0, &colNameLen, NULL, NULL, NULL, NULL );
		if( hasError( ret ) ) {
			logError();
			return axStatus_Std::DB_error;
		}

		st = wstr.resize( colNameLen, false );		if( !st ) return st;
		ret = SQLDescribeCol( stmt_, i+1, 
								wstr._getInternalBufferPtr(), colNameLen, NULL, 
								&col.type, 
								&col.sizeInDB, 
								&col.decimalDigits, 
								&col.nullable );
		if( hasError( ret ) ) {
			logError();
			return axStatus_Std::DB_error;
		}

		st = col.name.set( wstr );		if( !st ) return st;
	}

	return 0; 
}


int	axDBStmt_ODBC::columnType	( axSize col ) { 
	if( ! columnInfo.inBound(col) ) return axDB_c_type_null;

	switch( columnInfo[col].type ) {
	//== int ==
		case SQL_TINYINT:			return axDB_c_type_int8;
		case SQL_SMALLINT:			return axDB_c_type_int16;
		case SQL_INTEGER:			return axDB_c_type_int32;
		case SQL_BIGINT:			return axDB_c_type_int64;
	//== float ==
		case SQL_FLOAT:				return axDB_c_type_float;
	//== double ==
		case SQL_DOUBLE:			return axDB_c_type_double;
		case SQL_DECIMAL:			return axDB_c_type_double;
		case SQL_REAL:				return axDB_c_type_double;
	//== StringA ==
		case SQL_CHAR:				return axDB_c_type_StringA;
		case SQL_VARCHAR:			return axDB_c_type_StringA;
		case SQL_LONGVARCHAR:		return axDB_c_type_StringA;
	//== StringW ==
		case SQL_WCHAR:				return axDB_c_type_StringW;
		case SQL_WVARCHAR:			return axDB_c_type_StringW;
		case SQL_WLONGVARCHAR:		return axDB_c_type_StringW;
	//== ByteArray ==
		case SQL_BINARY:			return axDB_c_type_ByteArray;
		case SQL_VARBINARY:			return axDB_c_type_ByteArray;
		case SQL_LONGVARBINARY:		return axDB_c_type_ByteArray;
	//==
		case SQL_TYPE_DATE:			return axDB_c_type_TimeStamp;
		case SQL_TYPE_TIME:			return axDB_c_type_TimeStamp;
		case SQL_TYPE_TIMESTAMP:	return axDB_c_type_TimeStamp;
	}
	return axDB_c_type_null;
}

const char* axDBStmt_ODBC::columnName	( axSize col ) { 
	if( ! columnInfo.inBound(col) ) return NULL;
	return columnInfo[col].name;
}

axStatus	axDBStmt_ODBC::fetch () { 
	SQLRETURN ret = SQLFetch( stmt_ );

	if( ret == SQL_NO_DATA ) return axStatus::kEOF;

	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int8_t			&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_STINYINT, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int16_t			&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_SSHORT, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int32_t			&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_SLONG, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int64_t			&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_SBIGINT, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, float				&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_FLOAT, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, double			&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_DOUBLE, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, bool				&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	int8_t	tmp;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_TINYINT, &tmp, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	value = tmp ? true : false;
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axTimeStamp		&value ) { 
	axDateTime	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	value = tmp.toTimeStamp();
	return 0;
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axDateTime		&value ) {
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	TIMESTAMP_STRUCT	ts;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, col+1, SQL_C_TYPE_TIMESTAMP, &ts, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	value.year		= ts.year;
	value.month		= ts.month;
	value.day		= ts.day;
	value.hour		= ts.hour;
	value.minute	= ts.minute;
	value.second	= ts.second + (double) ts.fraction / 1000000000.0f;
	return 0; 
}


axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axIStringA		&value ) { 
	axStatus st;
	char tmp;
	SQLLEN cbLen;	
	SQLRETURN ret;

	//get the data length first
	ret = SQLGetData( stmt_, col+1, SQL_C_CHAR, &tmp, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	if( cbLen < 0 ) {
		assert(false);
		return axStatus_Std::DB_invalid_param_count;
	}

	st = value.resize( cbLen, false );		if( !st ) return st;
	if( cbLen == 0 ) return 0;

	ret = SQLGetData( stmt_, col+1, SQL_C_CHAR, value._getInternalBufferPtr(), cbLen+1, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axIStringW		&value ) {
	axTempStringA	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	return value.set( tmp ); 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axIByteArray		&value ) { 
	axStatus st;
	char tmp;
	SQLLEN cbLen;	
	SQLRETURN ret;

	int t = columnInfo[col].type;

	//get the data length first
	ret = SQLGetData( stmt_, col+1, SQL_C_BINARY, &tmp, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	if( cbLen < 0 ) {
		assert(false);
		return axStatus_Std::DB_invalid_param_count;
	}

	st = value.resize( cbLen, false );		if( !st ) return st;
	if( cbLen == 0 ) return 0;

	ret = SQLGetData( stmt_, col+1, SQL_C_BINARY, value.ptr(), cbLen, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

