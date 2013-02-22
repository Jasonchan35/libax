#include "axDBConn_Oracle.h"


axStatus	toUTextArray( axIByteArray & out, const char*	  sz ) {
	axStatus st;
	axTempStringW	w;
	st = w.set( sz );		if( !st ) return st;
	return toUTextArray( out, w );
}

axStatus	toUTextArray( axIByteArray & out, const wchar_t* sz ) {
	axStatus st;
	size_t n = ax_strlen( sz )+1;
	st = out.resize( n * sizeof(utext), false );		if( !st ) return st;

	const wchar_t* src = sz;
		  utext*   dst = (utext*)out.ptr();	

	for( size_t i=0; i<n; i++ ) {
		*dst = *src;
		dst++;
		src++;
	}
	return 0;
}

axStatus	fromUTextArray( axIStringW & out, const axIByteArray & utf16 ) {
	axStatus st;
	size_t n = utf16.byteSize() / sizeof( utext );
	st = out.resize(n);			if( !st ) return st;

	const utext* src = (utext*)utf16.ptr();
	wchar_t* dst = out._getInternalBufferPtr();

	for( size_t i=0; i<n; i++ ) {
		*dst = *src;
		dst++;
		src++;
	}

	return 0;
}

axDBStmt_Oracle::axDBStmt_Oracle( axDBConn_Oracle* db ) {
	stmt_ = NULL;
	db_ = db;
}

axDBStmt_Oracle::~axDBStmt_Oracle() {
	destroy();
}

void axDBStmt_Oracle::destroy() {
	if( stmt_ ) {
		OCIHandleFree( stmt_, OCI_HTYPE_STMT );
		stmt_ = NULL;
	}
}


bool axDBStmt_Oracle::hasError( sword status, const char* sql ) {
	return db_->hasError( status, sql );
}


axStatus axDBStmt_Oracle::create( const char* sql ) {
	axStatus st;
	//st = sql_.set( sql );		if( !st ) return st;

	st = convertSQL( sql_, sql );	if( !st ) return st;

	if( db_->echoSQL() ) {
		ax_log("--- CreateStmt SQL: ---\n{?}\n", sql_ );
	}

	sword ret;
	ret = OCIHandleAlloc( db_->envhp, (void**)&stmt_,	OCI_HTYPE_STMT,  0, 0 );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	axByteArray_<8192>	buf;

	st = toUTextArray( buf, sql_ );		if( !st ) return st;

	ret = OCIStmtPrepare( stmt_, db_->errhp, buf.ptr(), buf.byteSize(), OCI_NTV_SYNTAX, OCI_DEFAULT );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	return 0;
}

axStatus axDBStmt_Oracle::fetch() {
	assert(false); return 0;
}


const char* axDBStmt_Oracle::columnName( axSize col ) {
	assert(false); return NULL;
}

int axDBStmt_Oracle::columnType( axSize col ) {
	assert(false); return 0;
}

axSize axDBStmt_Oracle::numParams() {
	assert(false); return 0;
}

template<class T> inline
sword axDBStmt_Oracle::_BindPos_number( ub4 col, T & value, ub2 dty ) {
	OCIBind*	bindp = NULL;
	return OCIBindByPos( stmt_, &bindp, db_->errhp, col, ax_const_cast(&value), sizeof(value), dty, 
						 NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 
}

axStatus axDBStmt_Oracle::exec_ArgList( const axDBInParamList & list ) {
	axStatus st;

	echoExecSQL( db_, list );

	sword ret;

	st = tmpDataArray.resize( list.size(), false );			if( !st ) return st;

	OCIBind*	bindp = NULL;

	for( size_t i=0; i<list.size(); i++ ) {
		ub4 col = i+1;
		const axDBInParam &param = list[i];
		TmpData			&tmpData = tmpDataArray[i];

		switch( param.type ) {
			case axDB_c_type_int8:		{ ret = _BindPos_number( col, param.v_int8,   SQLT_INT ); }break;
			case axDB_c_type_int16:		{ ret = _BindPos_number( col, param.v_int16,  SQLT_INT ); }break;
			case axDB_c_type_int32:		{ ret = _BindPos_number( col, param.v_int32,  SQLT_INT ); }break;
			case axDB_c_type_int64:		{ ret = _BindPos_number( col, param.v_int64,  SQLT_INT ); }break;

			case axDB_c_type_uint8:		{ ret = _BindPos_number( col, param.v_int8,   SQLT_UIN ); }break;
			case axDB_c_type_uint16:	{ ret = _BindPos_number( col, param.v_int16,  SQLT_UIN ); }break;
			case axDB_c_type_uint32:	{ ret = _BindPos_number( col, param.v_int32,  SQLT_UIN ); }break;
			case axDB_c_type_uint64:	{ ret = _BindPos_number( col, param.v_int64,  SQLT_UIN ); }break;

			case axDB_c_type_float:		{ ret = _BindPos_number( col, param.v_float,  SQLT_BFLOAT  );  }break;
			case axDB_c_type_double:	{ ret = _BindPos_number( col, param.v_double, SQLT_BDOUBLE );  }break;

			case axDB_c_type_bool: { 
				int8_t v = param.v_bool ? 1 : 0;
				ret = _BindPos_number( col, v, SQLT_INT ); 
			}break;

			case axDB_c_type_StringA: {
				st = toUTextArray( tmpData, param.v_strA );		if( !st ) return st;
				ret = OCIBindByPos( stmt_, &bindp, db_->errhp, col, tmpData.ptr(), tmpData.byteSize(), SQLT_STR, 
									 NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 

				ub2 cform = SQLCS_NCHAR;
				ret = OCIAttrSet( bindp, OCI_HTYPE_BIND, &cform, 0, OCI_ATTR_CHARSET_FORM, db_->errhp );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
			}break;

			case axDB_c_type_StringW: {
				st = toUTextArray( tmpData, param.v_strW );		if( !st ) return st;
				ret = OCIBindByPos( stmt_, &bindp, db_->errhp, col, tmpData.ptr(), tmpData.byteSize(), SQLT_STR, 
									NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;

				ub2 cform = SQLCS_NCHAR;
				ret = OCIAttrSet( bindp, OCI_HTYPE_BIND, &cform, 0, OCI_ATTR_CHARSET_FORM, db_->errhp );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
			}break;

			default:
				ret = OCI_ERROR;
		}

		if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
	}

	ret = OCIStmtExecute( db_->svchp, stmt_, db_->errhp, 1, 0, 0, 0, OCI_DEFAULT | OCI_COMMIT_ON_SUCCESS ); 
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	return 0;
}


axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int8_t			&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int16_t			&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int32_t			&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int64_t			&value ) { assert(false); return 0; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint8_t			&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint16_t			&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint32_t			&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint64_t			&value ) { assert(false); return 0; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, float				&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, double			&value ) { assert(false); return 0; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, bool				&value ) { assert(false); return 0; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axIStringA		&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axIStringW		&value ) { assert(false); return 0; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axIByteArray		&value ) { assert(false); return 0; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axTimeStamp		&value ) { assert(false); return 0; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axDateTime		&value ) { assert(false); return 0; }


axStatus axDBStmt_Oracle::convertSQL( axIStringA &out, const char* inSQL ) {
	out.clear();
	axStatus st;

	numParams_ = 0;
		
	if( ! inSQL ) return axStatus_Std::invalid_parameter;
	//find '{' and '}'
	const char* s = NULL; //start
	const char* c = inSQL;
	const char* raw = inSQL;
	
	int cur_index = 0;	
	char  in_quote = 0;
	
	for( ; *c; c++ ) {
		if( in_quote ) {
			switch( in_quote ) {
				case '\'': {
					//flush out
					if( *c == '\'' ) {
						axSize len = c-raw;
						st = out.appendWithLength( raw, len );	if( !st ) return st;
						raw = c+1;
						st = out.append( '\'');			if( !st ) return st;
						in_quote = false;
					}
				}break;
				case '"': {
					//flush out
					if( *c == '"' ) {
						axSize len = c-raw;
						st = out.appendWithLength( raw, len );	if( !st ) return st;
						raw = c+1;
						st = out.append( '"');			if( !st ) return st;
						in_quote = 0;
					}
				}break;
			}
		}else{
			switch( *c ) {
				case '\'': {
					//flush out
					axSize len = c-raw;
					st = out.appendWithLength( raw, len );	if( !st ) return st;
					raw = c+1;
					st = out.append( '\'');			if( !st ) return st;
					in_quote = '\'';
				}break;
					
				case '"': {
					//flush out
					axSize len = c-raw;
					st = out.appendWithLength( raw, len );	if( !st ) return st;
					raw = c+1;
					st = out.append( '"');			if( !st ) return st;
					in_quote = '"';
				}break;
													
				case '?': {
					//flush out
					axSize len = c-raw;
					st = out.appendWithLength( raw, len );	if( !st ) return st;
					raw = c+1;
					
					cur_index++;
					out.appendFormat( ":{?}", cur_index );
					s = NULL;		

					numParams_++;
				}break;
			}
		}
	}
	
	if( !s ) {
		axSize len = c-raw;
		return out.appendWithLength( raw, len );
	}
	assert( false );
	return axStatus_Std::invalid_parameter;
}
