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
	const utext* src = (utext*)utf16.ptr();

	for( size_t i=0; i<n; i++ ) {
		if( src[i] == 0 ) { //find mull terminate
			n = i;
			break;
		}
	}

	st = out.resize(n);			if( !st ) return st;
	wchar_t* dst = out._getInternalBufferPtr();
	for( size_t i=0; i<n; i++ ) {
		*dst = *src;
		dst++;
		src++;
	}

//	ax_log_hex( utf16 );
//	ax_log_hex( out );

	return 0;
}

static
sword FetchDefineCallback(	dvoid		*octxp, 
							OCIDefine	*defnp, 
							ub4			iter, 
							dvoid		**bufpp,
							ub4			**alenpp,
							ub1			*piecep,
							dvoid		**indpp,
							ub2			**rcodep ) 
{
	axDBStmt_Oracle::ColumnInfo* info = (axDBStmt_Oracle::ColumnInfo*)octxp;

	ax_log("CLOB_CB {?} {?} {?}", iter, *piecep, octxp );

	axStatus st;

	switch( *piecep ) {
		case OCI_FIRST_PIECE: {
			size_t n = info->utf16_buf.capacity();
			if( n % 2 == 1 ) n--; //must be even number for utf16
			if( n == 0 ) n = info->utf16_buf_chunk_size;

			st = info->utf16_buf.resize( n );		if( !st ) return OCI_ERROR;

			*piecep = OCI_NEXT_PIECE;

			*bufpp	= info->utf16_buf.ptr();
			info->utf16_buf_ret_len  = info->utf16_buf.byteSize();
		}break;

		case OCI_NEXT_PIECE: {
			size_t n =info->utf16_buf.size();
			st = info->utf16_buf.resize( n + info->utf16_buf_chunk_size );	if( !st ) return OCI_ERROR;

			*piecep = OCI_NEXT_PIECE;

			*bufpp	= &info->utf16_buf[n];
			info->utf16_buf_ret_len = info->utf16_buf_chunk_size;

			ax_log_var( info->utf16_buf_ret_len );

		}break;

		default:
			assert(false);
			*bufpp = NULL;
	}

	info->utf16_buf_last_len = info->utf16_buf_ret_len;
	ax_log_var( info->utf16_buf_last_len );
	
	*alenpp = &info->utf16_buf_ret_len;

	info->oci_rcode = 0;
	*rcodep = &info->oci_rcode;

	info->oci_indicator = 0;
	*indpp  = &info->oci_indicator;

	return OCI_CONTINUE;
}

axDBStmt_Oracle::axDBStmt_Oracle( axDBConn_Oracle* db ) {
	stmt_ = NULL;
	db_ = db;
	stmt_type_ = OCI_STMT_UNKNOWN;
}

axDBStmt_Oracle::~axDBStmt_Oracle() {
	destroy();
}

void axDBStmt_Oracle::destroy() {
	sword ret;
	if( stmt_ ) {
		ret = OCIStmtRelease( stmt_, db_->errhp, NULL, 0, OCI_DEFAULT );
		if( hasError(ret,sql_) ) { assert(false); }

		stmt_type_ = OCI_STMT_UNKNOWN;
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

	ret = OCIAttrGet( stmt_, OCI_HTYPE_STMT, &stmt_type_, 0, OCI_ATTR_STMT_TYPE, db_->errhp );
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

//	ax_log_var( stmt_type );

	return 0;
}

//virtual	
axStatus	axDBStmt_Oracle::getRow_ArgList	( axDBOutParamList & list ) {
	axStatus st;
	sword ret;

	if( list.size() != columnInfos.size() ) return axStatus_Std::DB_invalid_param_count;

	for( size_t i=0; i<list.size(); i++ ) {
		axDBOutParam & out = list[i];
		ColumnInfo & info  = columnInfos[i];
		ub4 col = i+1;
		OCIDefine *defnp;

		switch( out.type ) {
		//==== int ====
			case axDB_c_type_int8: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int8_t), SQLT_INT, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_int16: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int16_t), SQLT_INT, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_int32: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int32_t), SQLT_INT, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_int64: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int64_t), SQLT_INT, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
		//===== uint8 ====
			case axDB_c_type_uint8: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int8_t), SQLT_UIN, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_uint16: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int16_t), SQLT_UIN, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_uint32: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int32_t), SQLT_UIN, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_uint64: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(int64_t), SQLT_UIN, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
		//==== real ===
			case axDB_c_type_float: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(float), SQLT_BFLOAT, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
			case axDB_c_type_double: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, out.data, sizeof(double), SQLT_BDOUBLE, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
		//==== bool ===
			case axDB_c_type_bool: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, &info.bool_as_int32, sizeof(int8_t), SQLT_INT, 0, 0, 0, OCI_DEFAULT );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
		//===== string ====
			case axDB_c_type_StringW: 
			case axDB_c_type_StringA: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, NULL, ax_type_max<sb4>(), 
										SQLT_STR, 0, 0, 0, OCI_DEFAULT | OCI_DYNAMIC_FETCH );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

				ret = OCIDefineDynamic( defnp, db_->errhp, &info, FetchDefineCallback );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

			// set UTF16
				ub2 cform = SQLCS_NCHAR;
				ret = OCIAttrSet( defnp, OCI_HTYPE_DEFINE, &cform, 0, OCI_ATTR_CHARSET_FORM, db_->errhp );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;
		//====== BLOB ======
			case axDB_c_type_blob: {
				ret = OCIDefineByPos( stmt_, &defnp, db_->errhp, col, NULL, ax_type_max<sb4>(), 
										SQLT_BIN, 0, 0, 0, OCI_DEFAULT | OCI_DYNAMIC_FETCH );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

				ret = OCIDefineDynamic( defnp, db_->errhp, &info, FetchDefineCallback );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_error;
			}break;

		//======= unknown =======
			default: 
				assert(false);
				return axStatus_Std::DB_error;
		}
	}

	ax_log( "====fetch===");
	ret = OCIStmtFetch2( stmt_, db_->errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT );
	if( ret == OCI_NO_DATA ) return axStatus::kEOF;
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;

	for( size_t i=0; i<list.size(); i++ ) {
		axDBOutParam & out = list[i];
		ColumnInfo & info  = columnInfos[i];
		ub4 col = i+1;

		switch( out.type ) {
			case axDB_c_type_bool: {
				*(bool*)out.data = info.bool_as_int32 ? true : false;
			}break;

			case axDB_c_type_StringA: {
				st = info.utf16_buf.decSize( info.utf16_buf_last_len - info.utf16_buf_ret_len );		if( !st ) return st;
				st = fromUTextArray( *(axIStringA*)out.data, info.utf16_buf );							if( !st ) return st;
			}break;

			case axDB_c_type_StringW: {
				st = info.utf16_buf.decSize( info.utf16_buf_last_len - info.utf16_buf_ret_len );		if( !st ) return st;
				st = fromUTextArray( *(axIStringW*)out.data, info.utf16_buf );							if( !st ) return st;
			}break;

			case axDB_c_type_blob: {
				st = info.utf16_buf.decSize( info.utf16_buf_last_len - info.utf16_buf_ret_len );		if( !st ) return st;
				st = ((axIByteArray*)out.data)->copy( info.utf16_buf );									if( !st ) return st;
			}break;
		}
	}

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
		//====== int =========
			case axDB_c_type_int8:		{ 
				ret = _BindPos_number( col, param.v_int8,   SQLT_INT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_int16:		{ 
				ret = _BindPos_number( col, param.v_int16,  SQLT_INT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_int32:		{ 
				ret = _BindPos_number( col, param.v_int32,  SQLT_INT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_int64:		{ 
				ret = _BindPos_number( col, param.v_int64,  SQLT_INT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
		//======= uint ========
			case axDB_c_type_uint8:		{ 
				ret = _BindPos_number( col, param.v_int8,   SQLT_UIN ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_uint16:	{ 
				ret = _BindPos_number( col, param.v_int16,  SQLT_UIN ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_uint32:	{ 
				ret = _BindPos_number( col, param.v_int32,  SQLT_UIN ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_uint64:	{ 
				ret = _BindPos_number( col, param.v_int64,  SQLT_UIN ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
		//========= real ====
			case axDB_c_type_float:		{ 
				ret = _BindPos_number( col, param.v_float,  SQLT_BFLOAT  );  
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
			case axDB_c_type_double:	{ 
				ret = _BindPos_number( col, param.v_double, SQLT_BDOUBLE );  
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type; 
			}break;
		//======= bool =====
			case axDB_c_type_bool: { 
				int8_t v = param.v_bool ? 1 : 0;
				ret = _BindPos_number( col, v, SQLT_INT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
			}break;
		//===== string ====
			case axDB_c_type_StringA: {
				st = toUTextArray( tmpData, param.v_strA );		if( !st ) return st;
				ret = OCIBindByPos( stmt_, &bindp, db_->errhp, col, tmpData.ptr(), tmpData.byteSize(), SQLT_STR, 
									 NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 

			// set UTF16
				ub2 cform = SQLCS_NCHAR;
				ret = OCIAttrSet( bindp, OCI_HTYPE_BIND, &cform, 0, OCI_ATTR_CHARSET_FORM, db_->errhp );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
			}break;

			case axDB_c_type_StringW: {
				st = toUTextArray( tmpData, param.v_strW );		if( !st ) return st;
				ret = OCIBindByPos( stmt_, &bindp, db_->errhp, col, tmpData.ptr(), tmpData.byteSize(), SQLT_STR, 
									NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;

			// set UTF16
				ub2 cform = SQLCS_NCHAR;
				ret = OCIAttrSet( bindp, OCI_HTYPE_BIND, &cform, 0, OCI_ATTR_CHARSET_FORM, db_->errhp );
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
			}break;

		//===== blob ====
			case axDB_c_type_blob: {
				const axIByteArray* arr = param.v_ByteArray;
				ax_log_hex( *arr );
				ret = OCIBindByPos( stmt_, &bindp, db_->errhp, col, ax_const_cast(arr->ptr()), arr->byteSize(), SQLT_BIN, 
									NULL, NULL, NULL, NULL, NULL, OCI_DEFAULT ); 
				if( hasError(ret,sql_) ) return axStatus_Std::DB_invalid_param_type;
			}break;

			default:
				assert(false);
				return axStatus_Std::DB_invalid_param_type;
		}
	}

	ub4 exec_mode = OCI_DEFAULT;
	if( ! db_->inTrans_ ) exec_mode |= OCI_COMMIT_ON_SUCCESS;

	ub4 iter = 1;
	if( stmt_type_ == OCI_STMT_SELECT ) {
		iter = 0;
	}

	ret = OCIStmtExecute( db_->svchp, stmt_, db_->errhp, iter, 0, 0, 0, exec_mode ); 
	if( hasError(ret,sql_) ) return axStatus_Std::DB_error;


	if( stmt_type_ == OCI_STMT_SELECT ) {
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

			//sb2 num_precision;
			//ret = OCIAttrGet( param, OCI_DTYPE_PARAM, &num_precision, 0, OCI_ATTR_PRECISION, db_->errhp );

			//sb1 num_scale;
			//ret = OCIAttrGet( param, OCI_DTYPE_PARAM, &num_scale, 0, OCI_ATTR_SCALE, db_->errhp );
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

axStatus	axDBStmt_Oracle::fetch() { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int8_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int16_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int32_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int64_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint8_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint16_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint32_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, uint64_t			&value ) { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, float				&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, double			&value ) { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, bool				&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axIStringA		&value ) { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axIStringW		&value ) { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axIByteArray		&value ) { assert(false); return axStatus_Std::should_not_be_here; }

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axTimeStamp		&value ) { assert(false); return axStatus_Std::should_not_be_here; }
axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, axDateTime		&value ) { assert(false); return axStatus_Std::should_not_be_here; }


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
