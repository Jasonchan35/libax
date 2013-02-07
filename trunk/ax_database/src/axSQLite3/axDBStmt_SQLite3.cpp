//
//  axDBStmt_SQLite3.cpp
//  axDB_SQLite3
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBStmt_SQLite3.h"
#include "axDBConn_SQLite3.h"

axDBStmt_SQLite3::axDBStmt_SQLite3( axDBConn_SQLite3* db ) { 
	db_.ref( db ); 
	paramCount_ = 0;
	colCount_ = 0;
	stmt_ = NULL;
}

void axDBStmt_SQLite3::releaseStmt() {
	if( stmt_ ) {
		sqlite3_finalize( stmt_ );
		stmt_ = NULL;
	}
}

const char* axDBStmt_SQLite3 :: getColumnName( axSize col ) {
	if( !stmt_ ) return NULL;
	axStatus st;
	int i;
	st = ax_safe_assign( i, col );	if( !st ) return NULL;
	return sqlite3_column_name( stmt_, i );
}

int	axDBStmt_SQLite3 :: getValueType( axSize col ) {
	if( !stmt_ ) return axDB_c_type_null;
	axStatus st;
	int i;
	st = ax_safe_assign( i, col );	if( !st ) return axDB_c_type_null;
	int t = sqlite3_column_type( stmt_, i );
	switch( t ) {
		case SQLITE_INTEGER: return axDB_c_type_int64;
		case SQLITE_FLOAT:	 return axDB_c_type_double;
		case SQLITE3_TEXT:	 return axDB_c_type_StringA;
		case SQLITE_BLOB:	 return axDB_c_type_ByteArray;
	}
	return axDB_c_type_null;
}

axStatus axDBStmt_SQLite3::exec_ParamList( const axDB_ParamList & list ) {
	if( !stmt_ ) return axStatus_Std::DB_error;

	sqlite3_reset( stmt_ );

	axStatus st;
			
	int ret;	
	if( list.size() < paramCount_ ) return axStatus_Std::DB_invalid_param_count;

	int n = (int)list.size();	
	
	//convert param value	
	st = tmpData.resize( list.size() );		if( !st ) return st;
	st = strData.resize( list.size() );		if( !st ) return st;

	int c = sqlite3_bind_parameter_count( stmt_ );
	if( c > n ) return axStatus_Std::DB_invalid_param_count;
			
	for( int i=0; i<c; i++ ) {
		const axDB_Param &param = list[i];
		switch( param.type ) {
		
			case axDB_c_type_int8:		ret = sqlite3_bind_int   ( stmt_, i+1, param.int8_  );	break;
			case axDB_c_type_int16: 	ret = sqlite3_bind_int   ( stmt_, i+1, param.int16_ );	break;
			case axDB_c_type_int32: 	ret = sqlite3_bind_int   ( stmt_, i+1, param.int32_ );	break;
			case axDB_c_type_int64: 	ret = sqlite3_bind_int64 ( stmt_, i+1, param.int64_ );	break;
			
			case axDB_c_type_bool: {
				tmpData[i] = param.bool_ ? 1 : 0;
				ret = sqlite3_bind_int( stmt_, i+1, tmpData[i] );
			}break;
								
			case axDB_c_type_float: 	ret = sqlite3_bind_double( stmt_, i+1, (double) param.float_  ); break;
			case axDB_c_type_double: 	ret = sqlite3_bind_double( stmt_, i+1,          param.double_ ); break;
			
			case axDB_c_type_StringA: 	ret = sqlite3_bind_text  ( stmt_, i+1, param.strA, -1, NULL ); break;				
			case axDB_c_type_StringW: {
				st = strData[i].set( param.strW );	if( !st ) return st;
				ret = sqlite3_bind_text( stmt_, i+1, strData[i], -1, NULL );
			}break;
			
			case axDB_c_type_TimeStamp:{
				axDateTime	dt( param.timestamp_ );
				st = strData[i].convert( dt );	if( !st ) return st;
				ret = sqlite3_bind_text( stmt_, i+1, strData[i], -1, NULL );
			}break;
			
			default: {
				assert( false );
				return axStatus_Std::DB_invalid_param_type;
			}
		}
		
		if( db_->hasError(ret) ) return axStatus_Std::DB_error;
	}
	
	ret = sqlite3_step( stmt_ );
	if( db_->hasError(ret) ) return axStatus_Std::DB_error;
	sqlite3_reset( stmt_ );
				
	colCount_ = sqlite3_column_count( stmt_ );
	
//	ax_log( "SQLite: {?} row changes ", sqlite3_changes(*db_) );
	return 0;
}

axStatus axDBStmt_SQLite3::fetch () {
	if( ! stmt_ ) return axStatus_Std::not_initialized;
	int ret = sqlite3_step( stmt_ );
	if( ret == SQLITE_DONE ) return axStatus_Std::DB_no_more_row;	
	if( db_->hasError(ret) ) return axStatus_Std::DB_error;
	return 0;
}

axStatus axDBStmt_SQLite3::prepare ( const char * sql ) {
	releaseStmt();	
	const char* remainSQL = NULL;
	int ret = sqlite3_prepare( *db_, sql, -1, &stmt_, &remainSQL );
	if( db_->hasError(ret) ) return axStatus_Std::DB_error;
	
	if( ax_strchrs( remainSQL, "\t\r\n " ) ) {
		ax_log("cannot contain multiple commands into a prepared statement");
		return axStatus_Std::DB_error;
	}
	return 0;
}

template< class T >
axStatus axDBStmt_SQLite3::getValue_int( axSize col, T & value ) {
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	int c = (int) col;
	int type = sqlite3_column_type( stmt_, c );
	
	axStatus st;
	switch( type ) {
		case SQLITE_NULL:
		case SQLITE_INTEGER: {
			// SQLite using int64 internal in memory
			st = ax_safe_assign( value, sqlite3_column_int64( stmt_, c ) );
			if( !st ) return st;
			return 0;
		}break;
	}
	
	value = 0;
	return axStatus_Std::DB_invalid_value_type;
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, int8_t  & value ) {  return getValue_int( col, value ); }
axStatus	axDBStmt_SQLite3::getValue( axSize col, int16_t & value ) {  return getValue_int( col, value ); }
axStatus	axDBStmt_SQLite3::getValue( axSize col, int32_t & value ) {  return getValue_int( col, value ); }
axStatus	axDBStmt_SQLite3::getValue( axSize col, int64_t & value ) {  return getValue_int( col, value ); }

axStatus	axDBStmt_SQLite3::getValue( axSize col, float		&value ) { 
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	int c = (int) col;
	int type = sqlite3_column_type( stmt_, c );
	
	switch( type ) {
		case SQLITE_NULL:
		case SQLITE_FLOAT: {
			value = (float)sqlite3_column_double( stmt_, c );
			return 0;
		}break;
	}
	
	value = 0;
	return axStatus_Std::DB_invalid_value_type;
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, double		&value ) { 
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	int c = (int) col;
	int type = sqlite3_column_type( stmt_, c );
	
	switch( type ) {
		case SQLITE_NULL:
		case SQLITE_FLOAT: {
			value = sqlite3_column_double( stmt_, c );
			return 0;
		}break;
	}
	
	value = 0;
	return axStatus_Std::DB_invalid_value_type;
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, bool		&value ) {
	axStatus st;
	int v;
	st = getValue_int( col, v );	if( !st ) return st;
	value = (v != 0);
	return 0;
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, axIStringA    &value ) { 
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	int c = (int) col;

	return value.set( (const char*)sqlite3_column_text( stmt_, c ) ); 
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, axIStringW    &value ) { 
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	int c = (int) col;

	return value.set( (const char*)sqlite3_column_text( stmt_, c ) ); 
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, axIByteArray	&value ) {
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	int c = (int) col;
	
	int type = sqlite3_column_type( stmt_, c );
	axStatus st;
	switch( type ) {
		case SQLITE_NULL: 	value.clear(); return 0;
		case SQLITE_BLOB: {
			axSize size;
			st = ax_safe_assign( size, sqlite3_column_bytes(stmt_, c ) );	if( !st ) return st;
			st = value.resize( size );
			memcpy( value.ptr(), sqlite3_column_blob(stmt_,c), size );
			return 0;
		}break;
	}
	
	value.clear();
	return axStatus_Std::DB_invalid_value_type;
}

axStatus	axDBStmt_SQLite3::getValue( axSize col, axTimeStamp	&value ) {
	if( !stmt_ ) return axStatus_Std::not_initialized;
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;

	axStatus st;

	int c = (int) col;
	int type = sqlite3_column_type( stmt_, c );
	switch( type ) {
		case SQLITE_TEXT: {
			axDateTime	dt;
			st = dt.setByString( (const char*)sqlite3_column_text( stmt_, c ) );	if( !st ) return st;
			value = dt.toTimeStamp();
		}break;
	}
	return 0;
}

