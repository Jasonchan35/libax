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
	release();
}

void axDBStmt_MySQL::release() {
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

axStatus axDBStmt_MySQL::prepare( const char * sql ) {
	axStatus st;
	release();
	stmt_ = mysql_stmt_init( *db_ );	if( !stmt_ ) return axStatus_Std::not_enough_memory;
	
	unsigned long len;
	st = ax_safe_assign( len, ax_strlen( sql ) );
	
	int ret = mysql_stmt_prepare( stmt_, sql, len );
	if( ret != 0 ) {
		ax_log( "MySQL Stmt Error {?}: {?}", mysql_stmt_errno(stmt_), mysql_stmt_error(stmt_) );
		return axStatus_Std::DB_error;
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



//virtual	
axStatus axDBStmt_MySQL::exec_ParamList( const axDBParamList & list ) {
	if( !stmt_ ) return axStatus_Std::not_initialized;
	axStatus st;	

	axSize n =  mysql_stmt_param_count(stmt_);

	if( list.size() < n ) return axStatus_Std::DB_invalid_param_count;

	st = bind_.resize( n );			if( !st ) return st;
	st = tempStr_.resize( n );		if( !st ) return st;
	st = tempLen_.resize( n );		if( !st ) return st;

	for( axSize i=0; i<n; i++ ) {
		const axDBParam & p = list[i];
		MYSQL_BIND & b = bind_[i];
		axIStringA & str = tempStr_[i];
		unsigned long & len = tempLen_[i];
		memset( &b, 0, sizeof(b) );	
		switch( p.type ) {
			case axDB_c_type_bool: {
				//using str as buffer
				st = str.resize(1);		if( !st ) return st;
				char *buf = str._getInternalBufferPtr();
				buf[0] = p.bool_ ? 1:0;
				b.buffer_type = MYSQL_TYPE_TINY;
				b.buffer      = buf;
			}break;
			case axDB_c_type_int8_t:		{ b.buffer_type = MYSQL_TYPE_TINY;		b.buffer = (void*)&p.int8_;		}break;
			case axDB_c_type_int16_t:		{ b.buffer_type = MYSQL_TYPE_SHORT;		b.buffer = (void*)&p.int16_;	}break;
			case axDB_c_type_int32_t:		{ b.buffer_type = MYSQL_TYPE_LONG;		b.buffer = (void*)&p.int32_;	}break;
			case axDB_c_type_int64_t:		{ b.buffer_type = MYSQL_TYPE_LONGLONG;	b.buffer = (void*)&p.int64_;	}break;
			case axDB_c_type_float:			{ b.buffer_type = MYSQL_TYPE_FLOAT;		b.buffer = (void*)&p.float_;	}break;
			case axDB_c_type_double:		{ b.buffer_type = MYSQL_TYPE_DOUBLE;	b.buffer = (void*)&p.double_;	}break;
			case axDB_c_type_axIStringA:	{
				b.buffer_type = MYSQL_TYPE_STRING;
				b.buffer      = (void*)p.strA;
				len = (long)ax_strlen( p.strA );
				b.buffer_length = len;
			}break;
			case axDB_c_type_axIStringW: {
				st = str.set( p.strW );		if( !st ) return st;
				b.buffer_type = MYSQL_TYPE_STRING;
				b.buffer      = (void*)str.c_str();
				b.buffer_length = (long)str.size();
			}break;
			case axDB_c_type_axDateTime: {
				//using str as buffer
				st = str.resize( sizeof(MYSQL_TIME) );		if( !st ) return st;
				MYSQL_TIME* buf = (MYSQL_TIME*) str._getInternalBufferPtr();

				const axDateTime	& dt = *p.p_dateTime;
				buf->year	= dt.year;
				buf->month	= dt.month;
				buf->day	= dt.day;
				buf->hour	= dt.hour;
				buf->minute	= dt.minute;
				buf->second	= (int)dt.second;
				buf->neg	= 0;
				double int_part;
				buf->second_part = (unsigned long)ax_modf( dt.second, &int_part ) * 1000000; //microsecond

				b.buffer_type = MYSQL_TYPE_TIMESTAMP;
				b.buffer      = buf;
			}break;
			case axDB_c_type_axTimeStamp: {
				//using str as buffer
				st = str.resize( sizeof(MYSQL_TIME) );		if( !st ) return st;
				MYSQL_TIME* buf = (MYSQL_TIME*) str._getInternalBufferPtr();

				axDateTime	dt( *p.p_timeStamp );
				buf->year	= dt.year;
				buf->month	= dt.month;
				buf->day	= dt.day;
				buf->hour	= dt.hour;
				buf->minute	= dt.minute;
				buf->second	= (int)dt.second;
				buf->neg	= 0;
				double int_part;
				buf->second_part = (unsigned long)ax_modf( dt.second, &int_part ) * 1000000; //microsecond

				b.buffer_type = MYSQL_TYPE_DATETIME;
				b.buffer      = buf;
			}break;
				/*
			axDB_c_type_ByteArray,
			axDB_c_type_Date,
			axDB_c_type_Time,
			axDB_c_type_TimeStamp,
			*/
			default:
				assert( false );
				return axStatus_Std::DB_invalid_param_type;
		}
	}

	mysql_stmt_bind_param( stmt_, bind_.ptr() );
	int ret = mysql_stmt_execute(stmt_);
	if( ret != 0 ) {
		ax_log( "MySQL Stmt Error {?}: {?}", mysql_stmt_errno(stmt_), mysql_stmt_error(stmt_) );
		return axStatus_Std::DB_error;
	}
		
	return 0;
}

int			axDBStmt_MySQL::columnType	( axSize col ) {
	if( col >= numColumns_ ) return axDB_c_type_null;
	
	MYSQL_FIELD* f = &columns_[col];
	switch( f->type ) {
		case MYSQL_TYPE_TINY:		return axDB_c_type_int8_t;
		case MYSQL_TYPE_SHORT:		return axDB_c_type_int16_t;
		case MYSQL_TYPE_LONG:		return axDB_c_type_int32_t;
		case MYSQL_TYPE_LONGLONG:	return axDB_c_type_int64_t;
		case MYSQL_TYPE_FLOAT:		return axDB_c_type_float;
		case MYSQL_TYPE_DOUBLE:		return axDB_c_type_double;
		case MYSQL_TYPE_TIME:		return axDB_c_type_axTimeStamp;
		case MYSQL_TYPE_DATE:		return axDB_c_type_axDateTime;
		case MYSQL_TYPE_DATETIME:	return axDB_c_type_axDateTime;
		case MYSQL_TYPE_TIMESTAMP:	return axDB_c_type_axTimeStamp;
		case MYSQL_TYPE_VAR_STRING:	return axDB_c_type_axIStringA;
		case MYSQL_TYPE_STRING:		return axDB_c_type_axIStringA;
		case MYSQL_TYPE_BLOB:		return axDB_c_type_axIByteArray;
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

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int8_t & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_TINY;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}	
	
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int16_t & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_SHORT;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}	
		
axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int32_t & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_LONG;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, int64_t & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_LONGLONG;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}			

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, float & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_FLOAT;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, double & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_DOUBLE;
	b.buffer      = &value;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	return 0;
}	

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

	axIStringA & str = tempStr_[col];
	
	unsigned long len = columns_[col].length + 1;
	st = str.resize( len );	if( !st ) return st;

	b.buffer_type = MYSQL_TYPE_STRING;
	b.buffer	  = str._getInternalBufferPtr();
	b.buffer_length = len;
	b.length	  = &len;

	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	
	st = value.setWithLength( str, len );		if( !st ) return st;	
	return 0;
}	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axIStringW & value ) {
	if( col >= numColumns_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;
	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	
	axIStringA & str = tempStr_[col];

	unsigned long len = columns_[col].length + 1;
	st = str.resize( len );	if( !st ) return st;

	b.buffer_type = MYSQL_TYPE_STRING;
	b.buffer	  = str._getInternalBufferPtr();
	b.buffer_length = len;
	b.length	  = &len;

	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}
	
	st = value.setWithLength( str, len );		if( !st ) return st;	
	return 0;
}	

axStatus axDBStmt_MySQL::getResultAtCol( axSize col, axIByteArray & value ) {
	return 0;
}

axStatus axDBStmt_MySQL::fetch() {
	axStatus st;
	if( !stmt_ ) return axStatus_Std::not_initialized;

	st = bind_.resize   ( numColumns_ );		if( !st ) return st;
	st = tempStr_.resize( numColumns_ );		if( !st ) return st;
	st = tempLen_.resize( numColumns_ );		if( !st ) return st;
	memset( bind_.ptr(), 0, bind_.byteSize() );
	mysql_stmt_bind_result( stmt_, bind_.ptr() );	//bind dummy here
	
	int ret = mysql_stmt_fetch( stmt_ );
	if( ret == MYSQL_NO_DATA        ) return axStatus::kEOF;
	if( ret == MYSQL_DATA_TRUNCATED ) {
//		ax_log("MySQL fetch: data truncated");
		return 0;
		//return axStatus_Std::DB_error;
	}

	if( ret != 0 ) {
		ax_log( "MySQL Stmt Error {?}: {?}", mysql_stmt_errno(stmt_), mysql_stmt_error(stmt_) );
		return axStatus_Std::DB_error;		
	}	
	return 0;
}

