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
	colCount_ = 0;
}

axDBStmt_MySQL::~axDBStmt_MySQL() {
	release();
}

void axDBStmt_MySQL::release() {
	if( col_meta_ ) {
		mysql_free_result( col_meta_ );
		col_meta_ = NULL;
		columns_ = NULL;
		colCount_ = 0;
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
	colCount_ = 0;
	
	col_meta_ = mysql_stmt_result_metadata( stmt_ );
	if( col_meta_ ) {
		columns_  = mysql_fetch_fields( col_meta_ );
		colCount_ = mysql_num_fields  ( col_meta_ );
	}
	return 0;
}



//virtual	
axStatus axDBStmt_MySQL::exec_ParamList( const axDB_ParamList & list ) {
	if( !stmt_ ) return axStatus_Std::not_initialized;
	axStatus st;	

	axSize n =  mysql_stmt_param_count(stmt_);

	if( list.size() < n ) return axStatus_Std::DB_invalid_param_count;

	st = bind_.resize( n );			if( !st ) return st;
	st = tempStr_.resize( n );		if( !st ) return st;
	st = tempLen_.resize( n );		if( !st ) return st;

	for( axSize i=0; i<n; i++ ) {
		const axDB_Param & p = list[i];
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
			case axDB_c_type_int8: {
				b.buffer_type = MYSQL_TYPE_TINY;
				b.buffer      = (void*)&p.int8_;
			}break;
			case axDB_c_type_int16: {
				b.buffer_type = MYSQL_TYPE_SHORT;
				b.buffer      = (void*)&p.int16_;
			}break;
			case axDB_c_type_int32: {
				b.buffer_type = MYSQL_TYPE_LONG;
				b.buffer      = (void*)&p.int32_;
			}break;
			case axDB_c_type_int64: {
				b.buffer_type = MYSQL_TYPE_LONGLONG;
				b.buffer      = (void*)&p.int64_;
			}break;
			case axDB_c_type_float: {
				b.buffer_type = MYSQL_TYPE_FLOAT;
				b.buffer      = (void*)&p.float_;
			}break;
			case axDB_c_type_double: {
				b.buffer_type = MYSQL_TYPE_DOUBLE;
				b.buffer      = (void*)&p.double_;
			}break;
			case axDB_c_type_StringA: {
				b.buffer_type = MYSQL_TYPE_STRING;
				b.buffer      = (void*)p.strA;
				len = (long)ax_strlen( p.strA );
				b.buffer_length = len;
			}break;
			case axDB_c_type_StringW: {
				st = str.set( p.strW );		if( !st ) return st;
				b.buffer_type = MYSQL_TYPE_STRING;
				b.buffer      = (void*)str.c_str();
				b.buffer_length = (long)str.size();
			}break;
			case axDB_c_type_TimeStamp: {
				//using str as buffer
				st = str.resize( sizeof(MYSQL_TIME) );		if( !st ) return st;
				MYSQL_TIME* buf = (MYSQL_TIME*) str._getInternalBufferPtr();

				axDateTime	dt( p.timestamp_ );
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

int			axDBStmt_MySQL::getValueType	( axSize col ) {
	if( col >= colCount_ ) return axDB_c_type_null;
	
	MYSQL_FIELD* f = &columns_[col];
	switch( f->type ) {
		case MYSQL_TYPE_TINY:		return axDB_c_type_int8;
		case MYSQL_TYPE_SHORT:		return axDB_c_type_int16;
		case MYSQL_TYPE_LONG:		return axDB_c_type_int32;
		case MYSQL_TYPE_LONGLONG:	return axDB_c_type_int64;
		case MYSQL_TYPE_FLOAT:		return axDB_c_type_float;
		case MYSQL_TYPE_DOUBLE:		return axDB_c_type_double;
		case MYSQL_TYPE_TIME:		return axDB_c_type_TimeStamp;
		case MYSQL_TYPE_DATE:		return axDB_c_type_DateTime;
		case MYSQL_TYPE_DATETIME:	return axDB_c_type_DateTime;
		case MYSQL_TYPE_TIMESTAMP:	return axDB_c_type_TimeStamp;
		case MYSQL_TYPE_VAR_STRING:	return axDB_c_type_StringA;
		case MYSQL_TYPE_STRING:		return axDB_c_type_StringA;
		case MYSQL_TYPE_BLOB:		return axDB_c_type_ByteArray;
	}
	return axDB_c_type_null;
}

const char* axDBStmt_MySQL::getColumnName	( axSize col ) {
	if( col >= colCount_ ) return NULL;
	return columns_[col].name;
}

axStatus axDBStmt_MySQL::getValue( axSize col, bool & value ) {
	axStatus st;
	int8_t tmp;
	st = getValue( col, tmp );	if( !st ) return st;
	value = ( tmp != 0 );
	return 0;
}	

axStatus axDBStmt_MySQL::getValue( axSize col, int8_t & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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
	
axStatus axDBStmt_MySQL::getValue( axSize col, int16_t & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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
		
axStatus axDBStmt_MySQL::getValue( axSize col, int32_t & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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

axStatus axDBStmt_MySQL::getValue( axSize col, int64_t & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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

axStatus axDBStmt_MySQL::getValue( axSize col, float & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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

axStatus axDBStmt_MySQL::getValue( axSize col, double & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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

axStatus axDBStmt_MySQL::getValue( axSize col, axTimeStamp & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
	axStatus st;

	MYSQL_TIME	tmp;

	MYSQL_BIND  b;
	memset( &b, 0, sizeof(b) );	
	b.buffer_type = MYSQL_TYPE_DATETIME;
	b.buffer      = &tmp;
	if( 0 != mysql_stmt_fetch_column( stmt_, &b, (unsigned)col, 0 ) ) {
		return axStatus_Std::DB_invalid_param_type;
	}

	axDateTime	dt;
	dt.year		= tmp.year;
	dt.month	= tmp.month;
	dt.day		= tmp.day;
	dt.hour		= tmp.hour;
	dt.minute	= tmp.minute;
	dt.second	= tmp.second;
	dt.second	+= (double)tmp.second_part / 1000000;

	value = dt.toTimeStamp();
	return 0;
}

axStatus axDBStmt_MySQL::getValue( axSize col, axIStringA & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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

axStatus axDBStmt_MySQL::getValue( axSize col, axIStringW & value ) {
	if( col >= colCount_ ) return axStatus_Std::DB_no_such_column;
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

axStatus axDBStmt_MySQL::getValue( axSize col, axIByteArray & value ) {
	return 0;
}

axStatus axDBStmt_MySQL::fetch() {
	axStatus st;
	if( !stmt_ ) return axStatus_Std::not_initialized;

	st = bind_.resize   ( colCount_ );		if( !st ) return st;
	st = tempStr_.resize( colCount_ );		if( !st ) return st;
	st = tempLen_.resize( colCount_ );		if( !st ) return st;
	memset( bind_.ptr(), 0, bind_.byteSize() );
	mysql_stmt_bind_result( stmt_, bind_.ptr() );	//bind dummy here
	
	int ret = mysql_stmt_fetch( stmt_ );
	if( ret == MYSQL_NO_DATA        ) return axStatus_Std::DB_no_more_row;
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

