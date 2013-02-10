//
//  axDBStmt_PostgreSQL.cpp
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_PostgreSQL.h"
#include "axDBStmt_PostgreSQL.h"

axStatus	axDBStmt_PostgreSQL::fetch() {
	if( ! res_ ) return 0;
	res_.curRow_++;
	if( res_.curRow_ >= res_.rowCount_ ) return axStatus::kEOF;
	return 0;
}

axStatus	axDBStmt_PostgreSQL::Result::status() {
	if( ! p_ ) return axStatus_Std::not_initialized;
	ExecStatusType e = PQresultStatus( p_ );
	switch( e ) {
		case PGRES_TUPLES_OK:	return 0;
		case PGRES_COMMAND_OK:	return 0;
		default: {
			ax_log( "PostgreSQL: {?}\n", PQresultErrorMessage( p_ ) );
			return axStatus_Std::DB_error;
		}
	}
}

int axDBStmt_PostgreSQL::columnType( axSize col ) {
	Oid oid = PQftype( res_, col );
	switch( oid ) {
		case BOOLOID:		return axDB_c_type_bool;
		case BYTEAOID:		return axDB_c_type_axIByteArray;
		case CHAROID:		return axDB_c_type_axIStringA;
		case INT8OID:		return axDB_c_type_int64_t;
		case INT2OID:		return axDB_c_type_int16_t;
		case INT4OID:		return axDB_c_type_int32_t;
		case TEXTOID:		return axDB_c_type_axIStringA;
		case FLOAT4OID:		return axDB_c_type_float;
		case FLOAT8OID:		return axDB_c_type_double;
		case BPCHAROID:		return axDB_c_type_axIStringA;
		case VARCHAROID:	return axDB_c_type_axIStringA;
		case DATEOID:		return axDB_c_type_axTimeStamp;
		case TIMEOID:		return axDB_c_type_axTimeStamp;
		case TIMESTAMPOID:	return axDB_c_type_axTimeStamp;
		case TIMESTAMPTZOID:return axDB_c_type_axTimeStamp;
		case TIMETZOID:		return axDB_c_type_axTimeStamp;
	}

	return axDB_c_type_null;
}

const char* axDBStmt_PostgreSQL::columnName	( axSize col ) {
	if( ! res_ ) return NULL;
	return PQfname( res_, col );
}

template<class T> inline
axStatus	axDBStmt_PostgreSQL::getValue_number( axSize col, T &value, Oid oid ) {
	if( !res_ ) return axStatus_Std::not_initialized;
	int c = (int) col;
	int r = (int) res_.curRow_;
	
	if( PQftype( res_, c ) != oid ) return axStatus_Std::DB_invalid_value_type;
	T* p = (T*)PQgetvalue( res_, r, c );
	if( !p ) { value = 0; return 0; } //is null
	value = ax_be_to_host( *p );
	return 1;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int8_t &	value ) { 
	int16_t tmp;
	axStatus st;
	st = getValue_number( col, tmp, INT2OID );	if( !st ) return st;
	st = ax_safe_assign( value, tmp );			if( !st ) return st;
	return 0;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int16_t &	value ) { return getValue_number( col, value, INT2OID ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int32_t &	value ) { return getValue_number( col, value, INT4OID ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int64_t &	value ) { return getValue_number( col, value, INT8OID ); }

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, float   &	value ) { return getValue_number( col, value, FLOAT4OID ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, double  &	value ) { return getValue_number( col, value, FLOAT8OID ); }

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, bool    &	value ) {
	if( !res_) return axStatus_Std::not_initialized;
	int c = (int) col;
	int r = (int) res_.curRow_;
	
	if( PQftype( res_, c ) != BOOLOID ) return axStatus_Std::DB_invalid_value_type;
	char* p = PQgetvalue( res_, r, c );
	if( !p ) { value = 0; return 0; } //is null
	value = (*p) ? true: false;
	return 1;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, axIStringA    &value ) {
	value.clear();
	if( !res_) return axStatus_Std::not_initialized;
	int c = (int) col;
	int r = (int) res_.curRow_;
	
	Oid oid = PQftype( res_, c );
	if( oid != VARCHAROID && oid != BPCHAROID && oid != TEXTOID ) return axStatus_Std::DB_invalid_value_type;
	char* p = PQgetvalue( res_, r, c );
	if( !p ) { return 0; } //is null
	value.set( p );
	return 1;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, axIStringW    &value ) {
	value.clear();
	if( !res_) return axStatus_Std::not_initialized;
	int c = (int) col;
	int r = (int) res_.curRow_;
	
	Oid oid = PQftype( res_, c );
	if( oid != VARCHAROID && oid != BPCHAROID && oid != TEXTOID ) return axStatus_Std::DB_invalid_value_type;
	char* p = PQgetvalue( res_, r, c );
	if( !p ) { return 0; } //is null
	value.set( p );
	return 1;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, axIByteArray	&value ) {
	value.clear();
	if( !res_) return axStatus_Std::not_initialized;
	int c = (int) col;
	int r = (int) res_.curRow_;
	
	Oid oid = PQftype( res_, c );
	if( oid != BYTEAOID ) return -1;
	char* p = PQgetvalue( res_, r, c );
	if( !p ) { return 0; } //is null
	
	axStatus st;
	size_t n;
	st = ax_safe_assign( n, PQgetlength( res_, r, c ) );	if( !st ) return st;
	st = value.resize( n, false );							if( !st ) return st;
	memcpy( value.ptr(), p, n );
	return 1;
}
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, axDateTime	&value ) {
	axTimeStamp	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	value.set( tmp );
	return 0;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, axTimeStamp	&value ) {
	if( !res_) return axStatus_Std::not_initialized;

	int c = (int) col;
	int r = (int) res_.curRow_;

	Oid oid = PQftype( res_, c );
	switch( oid ) {
		case TIMESTAMPTZOID: 
		case TIMESTAMPOID: {
			double d;
			if( PQgetisnull( res_, r, c ) ) { value = 0; return 0; }
#if USE_INTEGER_DATETIMES
			int64_t* p = (int64_t*)PQgetvalue( res_, r, c );
			int64_t  t = ax_be_to_host(*p);
			d = (long double)t / 1000000 + date_1970_to_2000;
#else
			double *p = (double *)PQgetvalue( res_, r, c );
			d = ax_be_to_host(*p) + date_1970_to_2000;
#endif
			if( oid == TIMESTAMPOID ) {
				d -= axDateTime::getTimeZone();
			}
			value = d;
			return 0;
		}break;
	}
	assert( false );
	return axStatus_Std::DB_invalid_value_type;
}

//virtual	
axStatus axDBStmt_PostgreSQL::exec_ParamList ( const axDBParamList & list ) {
	axStatus st;
	res_.colCount_ = 0;
	res_.rowCount_ = 0;

	if( ( stmtName_.isEmpty() ) || ( !paramSet_.isSameTypes( list ) ) ) {
		st = doPrepare( list );		if( !st ) return st;
	}
		
	st = paramSet_.bindList( list );	if( !st ) return st;
		
	res_.set( PQexecPrepared( *db_, stmtName_.c_str(), (int)paramSet_.size(), 
							(const char* const*)paramSet_.pData.ptr(), 
							paramSet_.lengths.ptr(), 
							paramSet_.formats.ptr(), BINARY_FORMAT ) );
							
	st = res_.status();		if( !st ) return st;
	res_.colCount_ = PQnfields( res_ );
	res_.rowCount_ = PQntuples( res_ );
	return 0;

}

axStatus axDBStmt_PostgreSQL::doPrepare( const axDBParamList & list ) {
	release();
	
	axStatus st;
	Result rs;

	st = paramSet_.setTypes( list );					if( !st ) return st;

	//using pointer as unique stmt name
	st = stmtName_.format( "{?}", (void*)this );		if( !st ) return st;
	rs.set( PQprepare( *db_, stmtName_, sql_, (int)paramSet_.size(), paramSet_.types.ptr() ) );
	st = rs.status();	
	if( !st ) {
		stmtName_.clear();
		return st;
	}


	//require PostgreSQL 8.2 
//	rs.set( PQdescribePrepared ( *db_, stmtName_ ) );
//	st = rs.status();	if( !st ) return st;
	return 0;	
}

axStatus axDBStmt_PostgreSQL::prepare( const char * sql ) {
	release();
	axStatus st;
	st = convertSQL( sql_, sql );	if( !st ) return st;
//	ax_log( "converted SQL: {?}", sql_ );
	return 0;
}
			  
axDBStmt_PostgreSQL::~axDBStmt_PostgreSQL() {
	release();
}
			
void axDBStmt_PostgreSQL::release() {
	if( stmtName_.isEmpty() ) return;
	axStatus st;
	axTempStringA	sql;
	st = sql.format( "DEALLOCATE \"{?}\" ", stmtName_ );
	stmtName_.clear();
	
	Result rs;
	rs.set( PQexec( *db_, sql.c_str() ) );
	st = rs.status();	if( !st ) { assert(false); return; }
}
			  
axStatus axDBStmt_PostgreSQL::convertSQL( axIStringA &out, const char* inSQL ) {
	out.clear();
	axStatus st;
		
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
					
				case '$': {
					//flush out
					axSize len = c-raw;
					st = out.appendWithLength( raw, len );	if( !st ) return st;
					raw = c+1;
					st = out.append( "$$" );			if( !st ) return st;
				}break;
										
				case '?': {
					//flush out
					axSize len = c-raw;
					st = out.appendWithLength( raw, len );	if( !st ) return st;
					raw = c+1;
					
					cur_index++;
					out.appendFormat( "${?}", cur_index );
					s = NULL;					
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
