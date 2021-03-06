//
//  axDBStmt_PostgreSQL.cpp
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_PostgreSQL.h"
#include "axDBStmt_PostgreSQL.h"


axDBStmt_PostgreSQL::Result::Result()	{ 
	stmt_ = NULL; 
	p_=NULL; 
	curRow_=-1; 
	colCount_=0; 
	rowCount_=0; 
}


void	axDBStmt_PostgreSQL::Result::release()	{ 
	if( p_ ) { 
		PQclear( p_ ); 
		stmt_ = NULL; 
		p_ = NULL; 
		curRow_ = -1;
		colCount_ = 0;
		rowCount_ = 0;
	} 
}


axStatus	axDBStmt_PostgreSQL::Result::status() {
	if( ! p_ ) return axStatus_Std::not_initialized;
	ExecStatusType e = PQresultStatus( p_ );
	switch( e ) {
		case PGRES_TUPLES_OK:	return 0;
		case PGRES_COMMAND_OK:	return 0;
		default: {
			ax_log( "PostgreSQL: {?}SQL:\n{?}\n", PQresultErrorMessage( p_ ), stmt_->sql_ );
			return axStatus_Std::DB_error;
		}
	}
}

//========
axStatus	axDBStmt_PostgreSQL::fetch() {
	if( ! res_ ) return 0;
	res_.curRow_++;
	if( res_.curRow_ >= res_.rowCount_ ) return axStatus::kEOF;
	return 0;
}

int axDBStmt_PostgreSQL::columnType( axSize col ) {
	Oid oid = PQftype( res_, (int)col );
	switch( oid ) {
		case BOOLOID:		return axDB_c_type_bool;
		case BYTEAOID:		return axDB_c_type_blob;
		case CHAROID:		return axDB_c_type_StringA;
		case INT8OID:		return axDB_c_type_int64;
		case INT2OID:		return axDB_c_type_int16;
		case INT4OID:		return axDB_c_type_int32;
		case TEXTOID:		return axDB_c_type_StringA;
		case FLOAT4OID:		return axDB_c_type_float;
		case FLOAT8OID:		return axDB_c_type_double;
		case BPCHAROID:		return axDB_c_type_StringA;
		case VARCHAROID:	return axDB_c_type_StringA;
		case DATEOID:		return axDB_c_type_datetime;
		case TIMEOID:		return axDB_c_type_datetime;
		case TIMESTAMPOID:	return axDB_c_type_datetime;
		case TIMESTAMPTZOID:return axDB_c_type_datetime;
		case TIMETZOID:		return axDB_c_type_datetime;
	}

	return axDB_c_type_null;
}

const char* axDBStmt_PostgreSQL::columnName	( axSize col ) {
	if( ! res_ ) return NULL;
	return PQfname( res_, (int)col );
}

template<class T> inline
axStatus	axDBStmt_PostgreSQL::getValue_number( axSize col, T &value ) {
	value = 0;
	if( !res_ ) return axStatus_Std::not_initialized;

	axStatus st;

	int c = (int) col;
	int r = (int) res_.curRow_;
	
	switch( PQftype( res_, c ) ) {
		case INT2OID: {
			int16_t* tmp = (int16_t*)PQgetvalue( res_, r, c );
			if( !tmp ) return 0; //is null
			st = ax_safe_assign( value, ax_be_to_host(*tmp) );		if( !st ) return st;
		}break;

		case INT4OID: {
			int32_t* tmp = (int32_t*)PQgetvalue( res_, r, c );
			if( !tmp ) return 0; //is null
			st = ax_safe_assign( value, ax_be_to_host(*tmp) );		if( !st ) return st;
		}break;

		case INT8OID: {
			int64_t* tmp = (int64_t*)PQgetvalue( res_, r, c );
			if( !tmp ) return 0; //is null
			st = ax_safe_assign( value, ax_be_to_host(*tmp) );		if( !st ) return st;
		}break;

		case FLOAT4OID: {
			float* tmp = (float*)PQgetvalue( res_, r, c );
			if( !tmp ) return 0; //is null
			st = ax_safe_assign( value, ax_be_to_host(*tmp) );		if( !st ) return st;
		}break;

		case FLOAT8OID: {
			double* tmp = (double*)PQgetvalue( res_, r, c );
			if( !tmp ) return 0; //is null
			st = ax_safe_assign( value, ax_be_to_host(*tmp) );		if( !st ) return st;
		}break;

		default: {
			assert( false );
			return axStatus_Std::DB_invalid_param_type;
		}break;
	}

	return 1;
}

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int8_t  &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int16_t &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int32_t &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, int64_t &	value ) { return getValue_number( col, value ); }

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, uint8_t  &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, uint16_t &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, uint32_t &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, uint64_t &	value ) { return getValue_number( col, value ); }

axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, float   &	value ) { return getValue_number( col, value ); }
axStatus	axDBStmt_PostgreSQL::getResultAtCol( axSize col, double  &	value ) { return getValue_number( col, value ); }


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
axStatus axDBStmt_PostgreSQL::exec_ArgList ( const axDBInParamList & list ) {
	axStatus st;

	echoExecSQL( db_, list );

	if( db_->lastExecResult_ ) {
		db_->lastExecResult_->release();
	}
	db_->lastExecResult_ = NULL;


	if( list.size() != numParams() ) return axStatus_Std::DB_invalid_param_count;


	res_.colCount_ = 0;
	res_.rowCount_ = 0;

	if( ( stmtName_.isEmpty() ) || ( !paramSet_.isSameTypes( list ) ) ) {
		st = doPrepare( list );		if( !st ) return st;
	}
		
	st = paramSet_.bindList( list );	if( !st ) return st;
		
	res_.set( this, PQexecPrepared( *db_, stmtName_.c_str(), 
									(int)paramSet_.size(), 
									(const char* const*)paramSet_.pData.ptr(), 
									paramSet_.lengths.ptr(), 
									paramSet_.formats.ptr(), BINARY_FORMAT ) );
							
	db_->lastExecResult_ = &res_;
	st = res_.status();		if( !st ) return st;


	res_.colCount_ = PQnfields( res_ );
	res_.rowCount_ = PQntuples( res_ );
	return 0;

}

axStatus axDBStmt_PostgreSQL::doPrepare( const axDBInParamList & list ) {
	destroy();
	
	axStatus st;
	Result rs;

	st = paramSet_.setTypes( list );					if( !st ) return st;

	//using pointer as unique stmt name
	st = stmtName_.format( "{?}", (void*)this );		if( !st ) return st;
	rs.set( this, PQprepare( *db_, stmtName_, sql_, (int)paramSet_.size(), paramSet_.types.ptr() ) );
	st = rs.status();	
	if( !st ) {
		stmtName_.clear();
		return axStatus_Std::DB_error_prepare_stmtement;
	}

	//require PostgreSQL 8.2 
//	rs.set( PQdescribePrepared ( *db_, stmtName_ ) );
//	st = rs.status();	if( !st ) return st;
	return 0;	
}

axStatus axDBStmt_PostgreSQL::create( const char * sql ) {
	destroy();
	axStatus st;
	st = convertSQL( sql_, sql );	if( !st ) return st;
	if( db_->echoSQL() ) {
		ax_log("--- CreateStmt SQL: ---\n{?}\n", sql_ );
	}
	return 0;
}

axDBStmt_PostgreSQL::axDBStmt_PostgreSQL( axDBConn_PostgreSQL* db ) { 
	db_.ref( db ); 
}
			  
axDBStmt_PostgreSQL::~axDBStmt_PostgreSQL() {
	destroy();
}
			
void axDBStmt_PostgreSQL::destroy() {
	if( stmtName_.isEmpty() ) return;

	if( db_->lastExecResult_ == &res_ ) {
		db_->lastExecResult_ = NULL;
		res_.release();
	}

	axStatus st;
	axTempStringA	sql;
	st = sql.format( "DEALLOCATE \"{?}\" ", stmtName_ );
	stmtName_.clear();
	
	Result rs;
	rs.set( this, PQexec( *db_, sql.c_str() ) );
	st = rs.status();	if( !st ) { assert(false); return; }
}
			  
axSize axDBStmt_PostgreSQL::numParams() {
	return numParams_;
}

axStatus axDBStmt_PostgreSQL::convertSQL( axIStringA &out, const char* inSQL ) {
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
