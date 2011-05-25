#ifdef axUSE_PGSQL

#include "axDBO_pgsql.h"

axDBO_pgsql::axDBO_pgsql() {
	conn_ = NULL;
}

axDBO_pgsql::~axDBO_pgsql() {
	close();
}

axStatus axDBO_pgsql::connect( const char* dsn ){
    axStatus st;

    conn_ = PQconnectdb( dsn );
    if( PQstatus( conn_ ) != CONNECTION_OK ) {
		close();
		return -1;
    }
	return 0;
}

void axDBO_pgsql::close() {
	if( conn_ ) {
		PQfinish( conn_ );
		conn_ = NULL;
	}
}

axStatus	axDBO_pgsql_DataSet::status() const {
	if( ! res_ ) return axStatus::code_not_initialized;
	ExecStatusType e = PQresultStatus( res_ );
	switch( e ) {
		case PGRES_TUPLES_OK:	return 0;
		case PGRES_COMMAND_OK:	return 0;
	}

#ifdef _DEBUG
	ax_print( "psql Error: {?}\n", PQresultErrorMessage( res_ ) );
#endif
	return -1;
}

axSize	axDBO_pgsql_DataSet::rowCount() const {
	if( ! res_ ) return 0;
	return PQntuples( res_ );
}

axSize	axDBO_pgsql_DataSet::colCount() const {
	if( ! res_ ) return 0;
	return PQnfields( res_ );
}

int	axDBO_pgsql_DataSet::getColumnType( axSize col ) const {
	if( !res_) return axDBO_c_type_null;
	Oid oid = PQftype( res_, col );
	switch( oid ) {
		case BOOLOID:	return axDBO_c_type_bool;
		case BYTEAOID:	return axDBO_c_type_ByteArray;
		case CHAROID:	return axDBO_c_type_char;
		case INT8OID:	return axDBO_c_type_int64;
		case INT2OID:	return axDBO_c_type_int16;
		case INT4OID:	return axDBO_c_type_int32;
		case FLOAT4OID: return axDBO_c_type_float;
		case FLOAT8OID: return axDBO_c_type_double;
		//string
		case BPCHAROID:
		case VARCHAROID:
		case TEXTOID: {
			return axDBO_c_type_StringA;
		}break;
		case DATEOID:	return axDBO_c_type_Date;
		case TIMEOID:	return axDBO_c_type_Time;
		case TIMETZOID:	return axDBO_c_type_Time;
		case TIMESTAMPOID:		return axDBO_c_type_TimeStamp;
		case TIMESTAMPTZOID:	return axDBO_c_type_TimeStamp;
	}
	assert(false);
	return axDBO_c_type_null;
}

axStatus	axDBO_pgsql_DataSet::getValue( axIByteArray & value, axSize row, axSize col ) const {
	value.clear();
	if( !res_) return axStatus::code_not_initialized;
	Oid oid = PQftype( res_, col );
	if( oid != BYTEAOID ) return -1;
	char* p = PQgetvalue( res_, row, col );
	if( !p ) { return 0; } //is null

	axStatus st;
	size_t n;
	st = ax_safe_assign( n, PQgetlength( res_, row, col ) );	if( !st ) return st;
	st = value.resize( n, false );								if( !st ) return st;
	memcpy( value.ptr(), p, n );
	return 1;
}

axStatus	axDBO_pgsql_DataSet::getValue( axIStringA & value, axSize row, axSize col ) const {
	value.clear();
	if( !res_) return axStatus::code_not_initialized;
	Oid oid = PQftype( res_, col );
	if( oid != VARCHAROID && oid != BPCHAROID && oid != TEXTOID ) return -1;
	char* p = PQgetvalue( res_, row, col );
	if( !p ) { return 0; } //is null
	value.set( p );
	return 1;
}

axStatus	axDBO_pgsql_DataSet::getValue( axIStringW & value, axSize row, axSize col ) const {
	value.clear();
	if( !res_) return axStatus::code_not_initialized;
	Oid oid = PQftype( res_, col );
	if( oid != VARCHAROID && oid != BPCHAROID && oid != TEXTOID ) return -1;
	char* p = PQgetvalue( res_, row, col );
	if( !p ) { return 0; } //is null
	value.set( p );
	return 1;
}

template<class T> inline static
axStatus	_getNumberValue( PGresult* res_, T &value, Oid oid, axSize row, axSize col ) {
	if( !res_) return axStatus::code_not_initialized;
	if( PQftype( res_, col ) != oid ) return -1;
	char* p = PQgetvalue( res_, row, col );
	if( !p ) { value = 0; return 0; } //is null
	value = ax_be_to_host( *(T*)p );
	return 1;
}

axStatus	axDBO_pgsql_DataSet::getValue( char    &	value, axSize row, axSize col ) const { return _getNumberValue( res_, value, CHAROID,	row, col ); }
axStatus	axDBO_pgsql_DataSet::getValue( int16_t &	value, axSize row, axSize col ) const { return _getNumberValue( res_, value, INT2OID,   row, col ); }
axStatus	axDBO_pgsql_DataSet::getValue( int32_t &	value, axSize row, axSize col ) const { return _getNumberValue( res_, value, INT4OID,   row, col ); }
axStatus	axDBO_pgsql_DataSet::getValue( int64_t &	value, axSize row, axSize col ) const { return _getNumberValue( res_, value, INT8OID,   row, col ); }

axStatus	axDBO_pgsql_DataSet::getValue( float   &	value, axSize row, axSize col ) const { return _getNumberValue( res_, value, FLOAT4OID, row, col ); }
axStatus	axDBO_pgsql_DataSet::getValue( double  &	value, axSize row, axSize col ) const { return _getNumberValue( res_, value, FLOAT8OID, row, col ); }

axStatus	axDBO_pgsql_DataSet::getValue( int8_t & value, axSize row, axSize col ) const {
	axStatus st;
	int16_t	v;
	st = getValue( v, row, col );		if( !st ) return st;
	return ax_safe_assign( value, v );
}


axStatus	axDBO_pgsql_DataSet::getValue( bool    &	value, axSize row, axSize col ) const {
	if( !res_) return axStatus::code_not_initialized;
	if( PQftype( res_, col ) != BOOLOID ) return -1;
	char* p = PQgetvalue( res_, row, col );
	if( !p ) { value = 0; return 0; } //is null
	value = (*p) ? true: false;
	return 1;
}


axStatus	axDBO_pgsql::execSQL_ParamList ( axDBO_Driver_ResultSP &out, const char* sql, const axDBO_ParamList &list ) {
	if( ! conn_ ) return axStatus::code_not_initialized;
	axStatus st;

	axDBO_pgsql_DataSet* res = new axDBO_pgsql_DataSet;
	if( ! res ) return axStatus::code_not_enough_memory;
	out.ref( res );
	res->dbo_ = this;

	axSize i;
	for( i=0; i<list.size(); i++ ) {
		const axDBO_Param &p = list[i];
		switch( p.type() ) {
			case axDBO_c_type_int8: { //pgSQL doesn't support int8. so we convert to int16
				_param_tmp	  [i].int16_ = (int16_t)( *(int8_t*) p.data() );
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( int16_t );
				_param_types  [i] = INT2OID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_int16: {
				_param_tmp	  [i].int16_ = ax_host_to_be( *(int16_t*) p.data() );
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( int16_t );
				_param_types  [i] = INT2OID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_int32: {
				_param_tmp	  [i].int32_ = ax_host_to_be( *(int32_t*) p.data() );
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( int32_t );
				_param_types  [i] = INT4OID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_int64: {
				_param_tmp	  [i].int64_ = ax_host_to_be( *(int64_t*) p.data() );
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( int64_t );
				_param_types  [i] = INT8OID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_bool: {
				_param_tmp	  [i].char_ = (*(bool*)p.data()) ? 1: 0;
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( int8_t );
				_param_types  [i] = BOOLOID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_float: {
				_param_tmp	  [i].float_ = ax_host_to_be( *(float*) p.data() );
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( float );
				_param_types  [i] = FLOAT4OID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_double: {
				_param_tmp	  [i].double_ = ax_host_to_be( *(double*) p.data() );
				_param_pvalues[i] = (const char*)&_param_tmp[i];
				_param_lengths[i] = sizeof( double );
				_param_types  [i] = FLOAT8OID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_char: {
				_param_pvalues[i] = (const char*) p.data();
				_param_lengths[i] = sizeof(char);
				_param_types  [i] = CHAROID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_char_str: {
				const char* sz = (const char*)p.data();
				_param_pvalues[i] = sz;
				_param_lengths[i] = ax_strlen( sz );
				_param_types  [i] = VARCHAROID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_wchar_str: {
				//convert to StringA
				_param_tmp_str[i].set( (const wchar_t*) p.data() );
				_param_pvalues[i] = _param_tmp_str[i].c_str();
				_param_lengths[i] = _param_tmp_str[i].size();
				_param_types  [i] = VARCHAROID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_StringA: {
				const axIStringA* v = (const axIStringA*)p.data();
				_param_pvalues[i] = v->c_str();
				_param_lengths[i] = v->size();
				_param_types  [i] = VARCHAROID;
				_param_formats[i] = BINARY_FORMAT;
			}break;
			case axDBO_c_type_StringW: {
				const axIStringW* v = (const axIStringW*)p.data();
				//convert to StringA
				_param_tmp_str[i].set( v->c_str() );
				_param_pvalues[i] = _param_tmp_str[i].c_str();
				_param_lengths[i] = _param_tmp_str[i].size();
				_param_types  [i] = VARCHAROID;
				_param_formats[i] = BINARY_FORMAT;
			}break;

			case axDBO_c_type_ByteArray: {
				const axIByteArray* v = (const axIByteArray*)p.data();
				_param_pvalues[i] = (const char*) v->ptr();
				_param_types  [i] = BYTEAOID;
				_param_lengths[i] = v->size();
				_param_formats[i] = BINARY_FORMAT;
			}break;

			default: {
				assert( false );
				return -100;
			}break;
		}
	}

	axTempStringA &_sql = _param_tmp_str[0];
	st = _convertPrepareSQL( _sql, sql );				if( !st ) return st;
//	ax_print( "SQL: {?}\n TO: {?} \n", sql, _sql );

	*res = PQexecParams( conn_, _sql, (int)list.size(),
						 _param_types, _param_pvalues, _param_lengths, _param_formats, BINARY_FORMAT );

	st = res->status();		if( !st ) return st;

	/*
	int row_count = PQntuples( _res );
	int col_count = PQnfields( _res );

	ax_print( "=== column === \n" );
	for( int c=0; c < col_count; c++ ) {
		ax_print( "{?} ", PQfname( _res,c ) );
	}

	ax_print( L"\n--- rows --- \n" );
	for( int r=0; r<row_count; r++ ) {
		for( int c=0; c < col_count; c++ ) {
			ax_print( L"{?} ", PQgetvalue( _res, r, c ) );
		}
		ax_print(L"\n");
	}
*/
	return 0;
}


axStatus axDBO_pgsql::_convertPrepareSQL( axIStringA &out, const char* inSQL ) {
	out.clear();
	axStatus st;

	if( ! inSQL ) return axStatus::code_invalid_parameter;
	//find '{' and '}'
	const char *s = NULL; //start
	const char *e = NULL; //end
	const char* c = inSQL;
	const char *raw = inSQL;

	int cur_index = 0;
	int last_index = 0;

	char  in_quote = 0;

	for( ; *c; c++ ) {
		if( in_quote ) {
			switch( in_quote ) {
				case '\'': {
					//flush out
					if( *c == '\'' ) {
						axSize len = c-raw;
						st = out.append( raw, len );	if( !st ) return st;
						raw = c+1;
						st = out.append( '\'');			if( !st ) return st;
						in_quote = false;
					}
				}break;
				case '"': {
					//flush out
					if( *c == '"' ) {
						axSize len = c-raw;
						st = out.append( raw, len );	if( !st ) return st;
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
					st = out.append( raw, len );	if( !st ) return st;
					raw = c+1;
					st = out.append( '\'');			if( !st ) return st;
					in_quote = '\'';
				}break;

				case '"': {
					//flush out
					axSize len = c-raw;
					st = out.append( raw, len );	if( !st ) return st;
					raw = c+1;
					st = out.append( '"');			if( !st ) return st;
					in_quote = '"';
				}break;

				case '$': {
					//flush out
					axSize len = c-raw;
					st = out.append( raw, len );	if( !st ) return st;
					raw = c+1;
					st = out.append( "$$" );			if( !st ) return st;
				}break;

				case '{': {
					if( c[1] == '{' ) {
						c++;
					}else{
						s=c;
					}
					//flush out
					axSize len = c-raw;
					st = out.append( raw, len );	if( !st ) return st;
					raw = c+1;
				}break;

				case '}': {
					raw = c+1;
					if( !s ) { //found '}' before '{'
						st = out.append( c, 1 );	if( !st ) return st;
						continue;
					}
					e=c;

					int index = 0;
					s++;

					axStringA_<64>	param, part1, part2;
					param.set( s, e-s );
					if( param.splitByChar( ':', part1, part2 ) ) {
						st = part1.decSize( 1 );			if( !st ) return st;
					//	st = this->opt.set( part2 );		if( !st ) return st;
					}

					//wprintf(L"arg = [%s] [%s] [%s]\n", param.c_str(), part1.c_str(), this->opt.c_str() );

					switch( *s ) {
						case '?':	index = cur_index;			break;
						case '+':	index = last_index + 1;		break;
						default:	{
							st = str_to( s, index );
							if( !st ) index = -1;
						}break;
					}

					cur_index++;
					last_index = index;
					out.appendFormat( " ${?} ", index+1 );
					s = NULL;

				}break;
			}
		}
	}

	if( !s ) {
		axSize len = c-raw;
		return out.append( raw, len );
	}
	assert( false ); //not end of '}'
	return axStatus::code_invalid_parameter;
}

//virtual
axStatus axDBO_pgsql::prepareSQL_ParamList ( axDBO_Driver_StmtSP &out, const char* sql, const axDBO_ParamList &list ) {
	if( ! conn_ ) return axStatus::code_not_initialized;
	axStatus st;

	axDBO_pgsql_Stmt* stmt = new axDBO_pgsql_Stmt;
	if( ! stmt ) return axStatus::code_not_enough_memory;
	out.ref( stmt );

	axStringA_<64>	stmt_name;

	//using pointer as stmt name
	stmt->dbo_.ref( this );
	st = stmt_name.format( "{?}", stmt );		if( !st ) return st;

	axDBO_pgsql_DataSet res;

	axTempStringA &_sql = _param_tmp_str[0];
	st = _convertPrepareSQL( _sql, sql );		if( !st ) return st;

	res = PQprepare( conn_, stmt_name, _sql, (int)list.size(), NULL );
	st = res.status();		if( !st ) return st;

#if axOS_WIN
	res = PQdescribePrepared ( conn_, stmt_name );
	st = res.status();		if( !st ) return st;

	axSize n_param = (axSize) PQnparams( res );
#else
    #warning need pgsql 8.2
#endif


	st = stmt->paramList_.copy( list );		if( !st ) return st;
/*
	axSize i;
	for( i=0; i<n_param; i++ ) {
		Oid t = PQparamtype ( res, (int)i );
		ax_print( "param type {?}\n", t );

		st = stmt->bindParam( list[i] );
		if( !st ) return st;
	}
*/
//	PQexecPrepared( conn_, name, nParam, &val

	st = stmt->stmtName_.set( stmt_name );		if( !st ) return st;
	return 0;
}


void axDBO_pgsql_Stmt::release() {
	axStatus st;
	axStringA_<128> sql;

	if( ! stmtName_ ) return;

	st = sql.format( "DEALLOCATE \"{?}\" ", stmtName_ );
	stmtName_.clear();

	if( ! dbo_ ) { assert(false); return; }
	if( ! dbo_->conn_ ) { assert(false); return; }

	if( !st ) { assert(false); return; }

	PGresult *res = PQexec( dbo_->conn_, sql );
	if ( !res ) { assert( false ); return; }

	ExecStatusType s = PQresultStatus( res );
//	if( s != PGRES_TUPLES_OK  ) assert( false );
}

axStatus axDBO_pgsql_Stmt::exec() {
	if( ! stmtName_ ) return axStatus::code_not_initialized;
	if( ! dbo_ ) return axStatus::code_not_initialized;
	if( ! dbo_->conn_ ) return axStatus::code_not_initialized;

	axDBO_pgsql_DataSet	res;
//	res = PQexecPrepared( dbo_->conn_, stmtName_, paramList_.size(),
//							paramValues_, paramLengths_, paramFormats_, 1 );
	return 0;
}

#endif //axUSE_PGSQL
