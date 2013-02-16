//
//  axDBStmt_MySQL.cpp
//  axDB_MySQL
//
//  Created by Jason on 19/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBStmt_MySQL.h"
#include "axDBConn_MySQL.h"

axDBStmt_MySQL::axDBStmt_MySQL( axDBConn_MySQL* db ) { 
	bind_.setCapacityIncrement( 32 );
	db_.ref( db ); 
	stmt_ = NULL; 
	col_meta_ = NULL;
	columns_ = NULL;
	numColumns_ = 0;
}

axDBStmt_MySQL::~axDBStmt_MySQL() {
	destroy();
}

void axDBStmt_MySQL::destroy() {
	if( col_meta_ ) {
		mysql_free_result( col_meta_ );
		col_meta_ = NULL;
		columns_ = NULL;
		numColumns_ = 0;
	}
	if( stmt_ ) {
		mysql_stmt_close( stmt_ );
		stmt_ = NULL;
	}
}

axStatus axDBStmt_MySQL::create( const char * sql ) {
	axStatus st;
	destroy();

	st = sql_.set( sql );					if( !st ) return st;
	if( db_->echoSQL() ) {
		ax_log("--- CreateStmt SQL: ---\n{?}\n", sql_ );
	}

	stmt_ = mysql_stmt_init( *db_ );		if( !stmt_ ) return axStatus_Std::not_enough_memory;
	
	unsigned long len;
	st = ax_safe_assign( len, ax_strlen( sql ) );
	
	int ret = mysql_stmt_prepare( stmt_, sql, len );
	if( ret != 0 ) {
		ax_log( "MySQL Stmt Error {?}: {?}\nSQL:{?}\n", mysql_stmt_errno(stmt_), mysql_stmt_error(stmt_), sql );
		return axStatus_Std::DB_error_prepare_stmtement;
	}
	
	columns_ = 0;
	numColumns_ = 0;
	
	col_meta_ = mysql_stmt_result_metadata( stmt_ );
	if( col_meta_ ) {
		columns_  = mysql_fetch_fields( col_meta_ );
		numColumns_ = mysql_num_fields  ( col_meta_ );
	}
	return 0;
}

axSize	axDBStmt_MySQL::numParams () {
	if( !stmt_ ) return 0;
	return mysql_stmt_param_count(stmt_);
}

//virtual	
axStatus axDBStmt_MySQL::exec_ArgList( const axDBInParamList & list ) {
	if( !stmt_ ) return axStatus_Std::not_initialized;

	axStatus st;	
	echoExecSQL( db_, list );

	if( list.size() != numParams() ) return axStatus_Std::DB_invalid_param_count;
	size_t n = numParams();

	st = bind_.resize( n );			if( !st ) return st;
	st = tempStr_.resize( n );		if( !st ) return st;
	st = tempLen_.resize( n );		if( !st ) return st;

	for( axSize i=0; i<n; i++ ) {
		const axDBInParam & p = list[i];
		MYSQL_BIND & b = bind_[i];
		axIStringA & tmpStr = tempStr_[i];

		memset( &b, 0, sizeof(b) );	
		switch( p.type ) {
			case axDB_c_type_bool: {
				//using tmpStr as buffer
				st = tmpStr.resize(1);		if( !st ) return st;
				char *buf = tmpStr._getInternalBufferPtr();
				buf[0] = p.v_bool ? 1:0;
				b.buffer_type = MYSQL_TYPE_TINY;
				b.buffer      = buf;
			}break;

			case axDB_c_type_float:		{ b.buffer_type = MYSQL_TYPE_FLOAT;		b.buffer = (void*)&p.v_float;	}break;
			case axDB_c_type_double:	{ b.buffer_type = MYSQL_TYPE_DOUBLE;	b.buffer = (void*)&p.v_double;	}break;

			case axDB_c_type_int8:		{ b.buffer_type = MYSQL_TYPE_TINY;		b.buffer = (void*)&p.v_int8;	}break;
			case axDB_c_type_int16:		{ b.buffer_type = MYSQL_TYPE_SHORT;		b.buffer = (void*)&p.v_int16;	}break;
			case axDB_c_type_int32:		{ b.buffer_type = MYSQL_TYPE_LONG;		b.buffer = (void*)&p.v_int32;	}break;
			case axDB_c_type_int64:		{ b.buffer_type = MYSQL_TYPE_LONGLONG;	b.buffer = (void*)&p.v_int64;	}break;

			case axDB_c_type_uint8:		{ b.buffer_type = MYSQL_TYPE_TINY;		b.is_unsigned=true; b.buffer = (void*)&p.v_uint8;	}break;
			case axDB_c_type_uint16:	{ b.buffer_type = MYSQL_TYPE_SHORT;		b.is_unsigned=true; b.buffer = (void*)&p.v_uint16;	}break;
			case axDB_c_type_uint32:	{ b.buffer_type = MYSQL_TYPE_LONG;		b.is_unsigned=true; b.buffer = (void*)&p.v_uint32;	}break;
			case axDB_c_type_uint64:	{ b.buffer_type = MYSQL_TYPE_LONGLONG;	b.is_unsigned=true; b.buffer = (void*)&p.v_uint64;	}break;

			case axDB_c_type_ByteArray: {
				b.buffer_type	= MYSQL_TYPE_BLOB;
				b.buffer		= (void*) p.v_ByteArray->ptr();
				st = ax_safe_assign( b.buffer_length, p.v_ByteArray->byteSize() );	if( !st ) return st;
			}break;

			case axDB_c_type_StringA:	{
				b.buffer_type = MYSQL_TYPE_STRING;
				b.buffer      = (void*)p.v_strA;
				st = ax_safe_assign( b.buffer_length, ax_strlen( p.v_strA ) );		if( !st ) return st;
			}break;

			case axDB_c_type_StringW: {
				st = tmpStr.set( p.v_strW );										if( !st ) return st;
				b.buffer_type	= MYSQL_TYPE_STRING;
				b.buffer		= (void*)tmpStr.c_str();
				st = ax_safe_assign( b.buffer_length, tmpStr.size() );				if( !st ) return st;
			}break;

			case axDB_c_type_TimeStamp: {
				//using tmpStr as buffer
				st = tmpStr.resize( sizeof(MYSQL_TIME) );							if( !st ) return st;
				MYSQL_TIME* mysqlTime = (MYSQL_TIME*) tmpStr._getInternalBufferPtr();

				axDateTime dt( p.v_TimeStamp );

				mysqlTime->year		= dt.year;
				mysqlTime->month	= dt.month;
				mysqlTime->day		= dt.day;
				mysqlTime->hour		= dt.hour;
				mysqlTime->minute	= dt.minute;
				mysqlTime->second	= (int)dt.second;
				mysqlTime->neg		= 0;
				double int_part;
				mysqlTime->second_part = (unsigned long)( ax_modf( dt.second, &int_part ) * 1000000 ); //microsecond

				b.buffer_type = MYSQL_TYPE_DATETIME;
				b.buffer      = mysqlTime;
			}break;

			default:
				assert( false );
				return axStatus_Std::DB_invalid_param_type;
		}
	}

	mysql_stmt_bind_param( stmt_, bind_.ptr() );
	int ret = mysql_stmt_execute(stmt_);
	if( ret != 0 ) {
		ax_log( "MySQL Stmt Error {?}: {?}\nSQL:{?}\n", mysql_stmt_errno(stmt_), mysql_stmt_error(stmt_), sql_ );
		return axStatus_Std::DB_error;
	}
		
	return 0;
}



int			axDBStmt_MySQL::columnType	( axSize col ) {
	if( col >= numColumns_ ) return axDB_c_type_null;
	
	MYSQL_FIELD* f = &columns_[col];
	switch( f->type ) {
		case MYSQL_TYPE_TINY:		return axDB_c_type_int8;
		case MYSQL_TYPE_SHORT:		return axDB_c_type_int16;
		case MYSQL_TYPE_LONG:		return axDB_c_type_int32;
		case MYSQL_TYPE_LONGLONG:	return axDB_c_type_int64;
		case MYSQL_TYPE_FLOAT:		return axDB_c_type_float;
		case MYSQL_TYPE_DOUBLE:		return axDB_c_type_double;
		case MYSQL_TYPE_TIME:		return axDB_c_type_TimeStamp;
		case MYSQL_TYPE_DATE:		return axDB_c_type_TimeStamp;
		case MYSQL_TYPE_DATETIME:	return axDB_c_type_TimeStamp;
		case MYSQL_TYPE_TIMESTAMP:	return axDB_c_type_TimeStamp;
		case MYSQL_TYPE_VAR_STRING:	return axDB_c_type_StringA;
		case MYSQL_TYPE_STRING:		return axDB_c_type_StringA;
		case MYSQL_TYPE_BLOB:		return axDB_c_type_ByteArray;
	}
	return axDB_c_type_null;
}

const char* axDBStmt_MySQL::columnName	( axSize col ) {
	if( col >= numColumns_ ) return NULL;
	return columns_[col].name;
}

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, bool & value ) {
	axStatus st;
	int8_t tmp;
	st = getResultAtCol( col, tmp );	if( !st ) return st;
	value = ( tmp != 0 );
	return 0;
}	

template<class T> inline
axStatus axDBStmt_MySQL::_getResultAtCol_number( axSize col, T & value, enum enum_field_types buffer_type, my_bool is_unsigned ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = buffer_type;
	b.is_unsigned = is_unsigned;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int8_t   & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_TINY,		false ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int16_t  & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_SHORT,		false ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int32_t  & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_LONG,		false ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int64_t  & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_LONGLONG,	false ); }	
	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, uint8_t  & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_TINY,		true ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, uint16_t & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_SHORT,		true ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, uint32_t & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_LONG,		true ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, uint64_t & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_LONGLONG,	true ); }	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, float    & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_FLOAT,		false ); }	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, double   & value ) { return _getResultAtCol_number( col, value, MYSQL_TYPE_DOUBLE,		false ); }	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axTimeStamp & value ) {
	axDateTime	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	value = tmp.toTimeStamp();
	return 0;
}

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axDateTime & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;

	MYSQL_TIME	tmp;

	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_DATETIME;
	b.buffer      = &tmp;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}

	value.year		= tmp.year;
	value.month		= tmp.month;
	value.day		= tmp.day;
	value.hour		= tmp.hour;
	value.minute	= tmp.minute;
	value.second	= tmp.second;
	value.second	+= (double)tmp.second_part / 1000000;

	return 0;
}

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axIStringA & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;

	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	

	unsigned long len = *bind_[col].length + 1;
	st = value.resize( len );	if( !st ) return st;

	b.buffer_type	= MYSQL_TYPE_STRING;
	b.buffer		= value._getInternalBufferPtr();
	b.buffer_length = len;
	b.length		= &len;

	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	
	return 0;
}	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axIStringW & value ) {
	axTempStringA	tmp;
	axStatus st = getResultAtCol( col, tmp );		if( !st ) return st;
	return value.set( tmp );
}	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axIByteArray & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;

	unsigned long len = *bind_[col].length;

	st = value.resize( len );		if( !st ) return st;

	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type	= MYSQL_TYPE_BLOB;
	b.buffer		= value.ptr();

	st = ax_safe_assign( b.buffer_length, value.byteSize() );		if( !st ) return st;
	b.length		= &len;

	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}

axStatus axDBStmt_MySQL::fetch() {
	axStatus st;
	if( !stmt_ ) return axStatus_Std::not_initialized;

	st = bind_.resize   ( numColumns_ );		if( !st ) return st;
	st = tempStr_.resize( numColumns_ );		if( !st ) return st;
	st = tempLen_.resize( numColumns_ );		if( !st ) return st;
	memset( bind_.ptr(), 0, bind_.byteSize() );

	for( size_t i=0; i<numColumns_; i++ ) {
		bind_[i].length = &tempLen_[i];
	}

	mysql_stmt_bind_result( stmt_, bind_.ptr() );	//bind dummy here
	
	int ret = mysql_stmt_fetch( stmt_ );
	if( ret == MYSQL_NO_DATA        ) return axStatus::kEOF;
	if( ret == MYSQL_DATA_TRUNCATED ) {
//		ax_log("MySQL fetch: data truncated");
		return 0;
		//return axStatus_Std::DB_error;
	}

	if( ret != 0 ) {
		ax_log( "MySQL Stmt Error {?}: {?}\nSQL:{?}\n", mysql_stmt_errno(stmt_), mysql_stmt_error(stmt_), sql_ );
		return axStatus_Std::DB_error;		
	}	
	return 0;
}

