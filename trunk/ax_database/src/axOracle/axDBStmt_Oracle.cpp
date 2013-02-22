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

axStatus	fromUTextArray( axIStringA & out, const axIByteArray & utf16 ) {
	axTempStringW	tmp;
	axStatus st = fromUTextArray( tmp, utf16 );		if( !st ) return st;
	return out.set( tmp );
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
	curRow_ = 0;
}

axDBStmt_Oracle::~axDBStmt_Oracle() {
	destroy();
}

void axDBStmt_Oracle::destroy() {
	sword ret;
	if( stmt_ ) {
		ret = OCIStmtRelease( stmt_, db_->errhp, NULL, 0, OCI_DEFAULT );
		if( hasError(ret,sql_) ) { assert(false); }

		stmt_ = NULL;
	}
}


bool axDBStmt_Oracle::hasError( sword status, const char* sql ) {
	return db_->hasError( status, sql );
}


axStatus axDBStmt_Oracle::create( const char* sql ) {
	axStatus st;

	st = convertSQL( sql_, sql );	if( !st ) return st;

	if( db_->echoSQL() ) {
		ax_log("--- CreateStmt SQL: ---\n{?}\n", sql_ );
	}

	sword ret;
	axByteArray_<8192>	buf;

	st = toUTextArray( buf, sql_ );		if( !st ) return st;

	ret = OCIStmtPrepare2( db_->svchp, &stmt_, db_->errhp, buf.ptr(), buf.byteSize(), NULL, 0, OCI_NTV_SYNTAX, OCI_DEFAULT );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	columnInfos.resize(0);

	ub2 stmt_type;
	ret = OCIAttrGet( stmt_, OCI_HTYPE_STMT, &stmt_type, 0, OCI_ATTR_STMT_TYPE, db_->errhp );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

//	ax_log_var( stmt_type );

	if( stmt_type == OCI_STMT_SELECT ) {

		ret = OCIStmtExecute( db_->svchp, stmt_, db_->errhp, 1, 0, 0, 0, OCI_DESCRIBE_ONLY ); 
		if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

		ub4 parmcnt; 
		ret = OCIAttrGet( stmt_, OCI_HTYPE_STMT, &parmcnt, 0, OCI_ATTR_PARAM_COUNT, db_->errhp );
		if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

		st = columnInfos.resize( parmcnt );		if( !st ) return st;

		axByteArray_<128>	colName;
		/* get describe information for each column */ 
		for ( ub4 i = 0; i < parmcnt; i++ ) { 
			ColumnInfo & info = columnInfos[i];

			ub4 col = i+1;
			OCIParam* param;
			ret = OCIParamGet( stmt_, OCI_HTYPE_STMT, db_->errhp, (void**)&param, col );
			if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

			ret = OCIAttrGet( param, OCI_DTYPE_PARAM, &info.dbType, 0, OCI_ATTR_DATA_TYPE, db_->errhp );
			if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

			sb2 num_precision;
			ret = OCIAttrGet( param, OCI_DTYPE_PARAM, &num_precision, 0, OCI_ATTR_PRECISION, db_->errhp );

			sb1 num_scale;
			ret = OCIAttrGet( param, OCI_DTYPE_PARAM, &num_scale, 0, OCI_ATTR_SCALE, db_->errhp );
/*
			st = colName.resizeToCapacity();		if( !st ) return st;
			ub4 len = colName.byteSize();

			ret = OCIAttrGet( param, OCI_DTYPE_PARAM, colName.ptr(), &len, OCI_ATTR_NAME, db_->errhp );
			if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

			colName.resize( len );
			ax_log_hex( colName );
			st = fromUTextArray( info.name, colName );		if( !st ) return st;

			ax_log("{?} ({?},{?}) {?}", info.dbType, num_precision, num_scale, info.name );
			*/
		}
	}

	return 0;
}

axStatus axDBStmt_Oracle::fetch() {
	sword ret;
	curRow_++;
	if( curRow_ == 1 ) {
		ub4 fetched;
		ret = OCIAttrGet( stmt_, OCI_HTYPE_STMT, &fetched, 0, OCI_ATTR_ROWS_FETCHED, db_->errhp );
		if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
		if( fetched < 1 ) return axStatus::kEOF;

		//no data if first row is not fetched
		return 0;
	}

	ret = OCIStmtFetch2( stmt_, db_->errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT );
	if( ret == OCI_NO_DATA ) return axStatus::kEOF;
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	return 0;
}


const char* axDBStmt_Oracle::columnName( axSize col ) {
	assert(false); return NULL;
}

int axDBStmt_Oracle::columnType( axSize col ) {
	assert(false); return 0;
}

axSize axDBStmt_Oracle::numParams() {
	return numParams_;
}

template<class T> inline
sword axDBStmt_Oracle::_BindPos_number( ub4 col, T & value, ub2 dty ) {
	OCIBind*	bindp = NULL;
	return OCIBindByPos( stmt_, &bindp, db_->errhp, col, ax_const_cast(&value), sizeof(value), dty, 
						 NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 
}

static
sword StmtDefineCB(	dvoid          *octxp,
					OCIDefine      *defnp,
					ub4            iter, 
					dvoid          **bufpp,
					ub4            **alenpp,
					ub1            *piecep,
					dvoid          **indpp,
					ub2            **rcodep ) 
{

	axDBStmt_Oracle* p = (axDBStmt_Oracle*) octxp;

	ax_log( "StmtDefineCB {?} {?} {?}", (void*)p, iter, *piecep );

	static char buf [2048];
	static ub4  len = 0;
	static ub2  retcode = 0;

	*bufpp = buf;
	*alenpp = &len;
	*piecep = OCI_LAST_PIECE;
	*rcodep = &retcode;

	return 0;
}

axStatus axDBStmt_Oracle::exec_ArgList( const axDBInParamList & list ) {
	axStatus st;

	echoExecSQL( db_, list );

	curRow_ = 0;
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

	for( size_t i=0; i<columnInfos.size(); i++ ) {
		ColumnInfo & info = columnInfos[i];
		info.reset();

		OCIDefine* defnp;
		ub4 col = i+1;

		switch( info.dbType ) {
			case SQLT_BFLOAT:
			case SQLT_BDOUBLE:
			case SQLT_IBFLOAT:
			case SQLT_IBDOUBLE:
			case SQLT_NUM: {
				info.is_number = true;
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, &info.number, sizeof(info.number), SQLT_VNU, 0, 0, 0, OCI_DEFAULT  );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			default:
				ax_log_var( info.dbType );
				assert(false);
				return axStatus_Std::DB_invalid_param_type;
		}
	}

	ub4 exec_mode = OCI_DEFAULT;
	if( ! db_->inTrans_ ) exec_mode |= OCI_COMMIT_ON_SUCCESS;

	ret = OCIStmtExecute( db_->svchp, stmt_, db_->errhp, 1, 0, 0, 0, exec_mode ); 
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	return 0;
}

template<class T>
axStatus	axDBStmt_Oracle::_getResultAtCol_int	( axSize col, T	&value ) {
	value = 0;
	ColumnInfo & info = columnInfos[col];
	if( ! info.is_number ) return axStatus_Std::DB_invalid_value_type;

	sword ret = OCINumberToInt( db_->errhp, &info.number, sizeof( value ), OCI_NUMBER_SIGNED, &value );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
	return 0; 
}

template<class T>
axStatus	axDBStmt_Oracle::_getResultAtCol_uint ( axSize col, T	&value ) { 
	value = 0;
	ColumnInfo & info = columnInfos[col];
	if( ! info.is_number ) return axStatus_Std::DB_invalid_value_type;

	sword ret = OCINumberToInt( db_->errhp, &info.number, sizeof( value ), OCI_NUMBER_UNSIGNED, &value );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
	return 0; 
}

template<class T>
axStatus	axDBStmt_Oracle::_getResultAtCol_real ( axSize col, T	&value ) { 
	value = 0;
	ColumnInfo & info = columnInfos[col];
	if( ! info.is_number ) return axStatus_Std::DB_invalid_value_type;

	sword ret = OCINumberToReal( db_->errhp, &info.number, sizeof( value ), &value );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
	return 0; 
}


axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int8_t			&value ) { return _getResultAtCol_int(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int16_t			&value ) { return _getResultAtCol_int(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int32_t			&value ) { return _getResultAtCol_int(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int64_t			&value ) { return _getResultAtCol_int(col,value); }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint8_t			&value ) { return _getResultAtCol_uint(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint16_t			&value ) { return _getResultAtCol_uint(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint32_t			&value ) { return _getResultAtCol_uint(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint64_t			&value ) { return _getResultAtCol_uint(col,value); }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, float				&value ) { return _getResultAtCol_real(col,value); }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, double			&value ) { return _getResultAtCol_real(col,value); }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, bool				&value ) { 
	value = false;
	int8_t tmp;
	axStatus st = _getResultAtCol_int(col,tmp);	if( !st ) return st;
	value = tmp ? true : false;
	return 0; 
}

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
