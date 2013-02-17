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

	return 0;
}

axSize axDBStmt_ODBC::numParams() {
	if( !stmt_ ) return 0;

	SQLSMALLINT	paramCount = 0;
	SQLRETURN	ret;
	ret = SQLNumParams( stmt_, &paramCount );
	if( hasError( ret ) ) {
		logError();
		return axStatus_Std::DB_error_prepare_stmtement;
	}
	return (size_t) paramCount;
}

void axDBStmt_ODBC::destroy() {
	if( stmt_ ) {
		if( db_->lastExecStmt_ == stmt_ ) {
			SQLCloseCursor( db_->lastExecStmt_ );
			db_->lastExecStmt_ = NULL;
		}

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
		if( ! wcsncmp(wszState, L"01004", 5) ) continue;
		ax_log("ODBC Error [{?}] [{?}]: {?}\nSQL:\n{?}\n", wszState, (int)iNativeError, wszMessage, sql_ );
    }
}

SQLRETURN axDBStmt_ODBC::_OnSQLBindParameter( SQLUSMALLINT col, const int64_t & value, axIStringW & tmpStr, SQLLEN & len ) {
	return SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,		0, 0, ax_const_cast(&value), 0, &len );
}

SQLRETURN axDBStmt_ODBC::_OnSQLBindParameter( SQLUSMALLINT col, const uint64_t & value, axIStringW & tmpStr, SQLLEN & len ) {
	return _OnBindParamByString( col, value, tmpStr, len );
}

axStatus	axDBStmt_ODBC::exec_ArgList	( const axDBInParamList & list ) { 
	axStatus st;
	echoExecSQL( db_, list );

	if( db_->lastExecStmt_ ) {
		SQLCloseCursor( db_->lastExecStmt_ );
	}
	db_->lastExecStmt_ = stmt_;


	SQLRETURN	ret = SQL_ERROR;
	if( list.size() != numParams() )	return axStatus_Std::DB_invalid_param_count;

	st = tmpStrData.resize	( list.size() );			if( !st ) return st;
	st = cbLen.resize		( list.size() );			if( !st ) return st;
	cbLen.setAll(0);

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBInParam & param = list[i];
		SQLLEN			&len	= cbLen[i];
		SQLUSMALLINT	col		= (SQLUSMALLINT) i+1;

		switch( param.type ) {
			case axDB_c_type_int8: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_STINYINT, SQL_INTEGER,	0, 0, ax_const_cast(&param.v_int8), 0, &len );
			}break;
			case axDB_c_type_int16: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_SSHORT,	SQL_INTEGER,	0, 0, ax_const_cast(&param.v_int16), 0, &len );
			}break;
			case axDB_c_type_int32: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_SLONG,	SQL_INTEGER,	0, 0, ax_const_cast(&param.v_int32), 0, &len );
			}break;
			case axDB_c_type_int64: {
				ret = _OnSQLBindParameter( col, param.v_int64, tmpStrData[i], len );
			}break;

			case axDB_c_type_uint8: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_UTINYINT, SQL_INTEGER,	0, 0, ax_const_cast(&param.v_uint8), 0, &len );
			}break;
			case axDB_c_type_uint16: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_USHORT,	SQL_INTEGER,	0, 0, ax_const_cast(&param.v_uint16), 0, &len );
			}break;
			case axDB_c_type_uint32: {
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_ULONG,	SQL_BIGINT,		0, 0, ax_const_cast(&param.v_uint32), 0, &len );
			}break;
			case axDB_c_type_uint64: {
				ret = _OnSQLBindParameter( col, param.v_uint64, tmpStrData[i], len );
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

			case axDB_c_type_StringW: {
				len = SQL_NTS;
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_WCHAR,	SQL_WVARCHAR,	0, 0, ax_const_cast(param.v_strW), 0, &len );
			}break;

			case axDB_c_type_StringA: {
				st = tmpStrData[i].set( param.v_strA );		if( !st ) return st;
				const wchar_t* sz = tmpStrData[i].c_str();
				len = SQL_NTS;
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_WCHAR,	SQL_WVARCHAR,	0, 0, ax_const_cast(sz), 0, &len );
			}break;

			case axDB_c_type_ByteArray: {
				const axIByteArray*	data = param.v_ByteArray;
				len = data->byteSize();
				const uint8_t* ptr = data->ptr();
				if( len == 0 ) {
					st = tmpStrData[i].resize(32);		if( !st ) return st;
					// ptr cannot be NULL anyway even len = 0, so just give it a dummy pointer
					ptr = (uint8_t*)tmpStrData[i].c_str(); 
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

				double int_part; 
				int micro_second = (int)(ax_modf( dt.second, &int_part ) * 1000000); //round to micro-second
				ts->fraction = (SQLUINTEGER)( micro_second * 1000 ); //nano-second
				ts->second	 = (SQLUSMALLINT) int_part;

				len = sizeof(SQL_TIMESTAMP_STRUCT);
				ret = SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, 
										SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 27, 7, ts, 0, &len );
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
		ResultCol & resultCol = columnInfo[i];
		SQLUSMALLINT col = (SQLUSMALLINT)i+1;
		SQLSMALLINT colNameLen;
		//get the name length
		ret = SQLDescribeCol( stmt_, col, NULL, 0, &colNameLen, NULL, NULL, NULL, NULL );
		if( hasError( ret ) ) {
			logError();
			return axStatus_Std::DB_error;
		}

		st = wstr.resize( colNameLen, false );		if( !st ) return st;
		ret = SQLDescribeCol( stmt_, col, 
								wstr._getInternalBufferPtr(), colNameLen, NULL, 
								&resultCol.type, 
								&resultCol.sizeInDB, 
								&resultCol.decimalDigits, 
								&resultCol.nullable );
		if( hasError( ret ) ) {
			logError();
			return axStatus_Std::DB_error;
		}

		st = resultCol.name.set( wstr );		if( !st ) return st;
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

	if( ret == SQL_NO_DATA ) {
		SQLCloseCursor( stmt_ );
		return axStatus::kEOF;
	}

	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_error;
	}
	return 0; 
}

template<class T> inline
axStatus	axDBStmt_ODBC::_getResultAtCol_number ( axSize col, T &value, SQLSMALLINT C_Type ) {
	value = 0;

	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	SQLLEN cbLen = 0;	
	SQLRETURN ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, C_Type, &value, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int8_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_STINYINT ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int16_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_SSHORT ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int32_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_SLONG ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, int64_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_SBIGINT ); }

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, uint8_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_UTINYINT ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, uint16_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_USHORT ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, uint32_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_ULONG ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, uint64_t	&value ) {  return _getResultAtCol_number( col, value, SQL_C_UBIGINT ); }

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, float		&value ) {  return _getResultAtCol_number( col, value, SQL_C_FLOAT ); }
axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, double	&value ) {  return _getResultAtCol_number( col, value, SQL_C_DOUBLE ); }

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, bool		&value ) { 
	value = false;
	axStatus st;
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;
	int8_t	tmp = 0;
	st = getResultAtCol( col, tmp );		if( !st ) return st;
	value = tmp ? true : false;
	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axTimeStamp		&value ) { 
	value = 0;

	axDateTime	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	value = tmp.toTimeStamp();
	return 0;
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axDateTime		&value ) {
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	value.reset();

	TIMESTAMP_STRUCT	ts;

	SQLLEN cbLen;	
	SQLRETURN ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, SQL_C_TYPE_TIMESTAMP, &ts, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	value.year		= ts.year;
	value.month		= (int8_t)ts.month;
	value.day		= (int8_t)ts.day;
	value.hour		= (int8_t)ts.hour;
	value.minute	= (int8_t)ts.minute;
	value.second	= ts.second + (double) ts.fraction / 1000000000.0f; ////nano-second
	return 0; 
}


axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axIStringW 	&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	value.resize(0);

	axStatus st;
	char tmp;
	SQLLEN cbLen;	
	SQLRETURN ret;

	//get the data length first
	ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, SQL_C_WCHAR, &tmp, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	if( cbLen == 0 || cbLen == SQL_NULL_DATA ) return 0;
	if( cbLen < 0 ) {
		assert(false);
		return axStatus_Std::DB_invalid_param_count;
	}

	size_t n = cbLen / sizeof(wchar_t);

	st = value.resize( n, false );		if( !st ) return st;

	ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, SQL_C_WCHAR, value._getInternalBufferPtr(), (n+1)*sizeof(wchar_t), &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	return 0; 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axIStringA		&value ) {
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	value.resize(0);

	axTempStringW	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	return value.set( tmp ); 
}

axStatus	axDBStmt_ODBC::getResultAtCol	( axSize col, axIByteArray		&value ) { 
	if( ! columnInfo.inBound(col) ) return axStatus_Std::DB_invalid_param_count;

	value.resize(0);

	axStatus st;
	char tmp;
	SQLLEN cbLen;	
	SQLRETURN ret;

	int t = columnInfo[col].type;

	//get the data length first
	ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, SQL_C_BINARY, &tmp, 0, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	if( cbLen == 0 || cbLen == SQL_NULL_DATA )  {
		return 0; // BLOB size = 0 or NULL
	}

	if( cbLen < 0 ) {
		assert(false);
		return axStatus_Std::DB_invalid_param_count;
	}

	st = value.resize( cbLen, false );		if( !st ) return st;
	ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, SQL_C_BINARY, value.ptr(), cbLen, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0; 
}

