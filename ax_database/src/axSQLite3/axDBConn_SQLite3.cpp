//
//  axDBConn_SQLite3.cpp
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_SQLite3.h"
#include "axDBStmt_SQLite3.h"



axStatus axSQLite3_open( axDBConn & conn, const char* filename ) {
	axStatus st;
	axDBConn_SQLite3* p = new axDBConn_SQLite3;
	if( !p ) return axStatus_Std::not_enough_memory;
	conn._setImp( p );
	return p->openFile( filename );
}

axStatus axSQLite3_openMemory( axDBConn & conn ) {
	axStatus st;
	axDBConn_SQLite3* p = new axDBConn_SQLite3;
	if( !p ) return axStatus_Std::not_enough_memory;
	conn._setImp( p );
	return p->openMemory();
}

	
axStatus axDBConn_SQLite3::openFile( const char * filename ) {
#if axOS_WIN
	axStatus st;
	axTempStringW	w_filename;
	st = w_filename.set( filename );	if( !st ) return st;
	int ret = sqlite3_open16( w_filename, &p_ );
#else
	int ret = sqlite3_open( filename, &p_ );
#endif
	if( hasError(ret) ) {
		return axStatus_Std::DB_error_connect;
	}
	return 0;
}

axStatus axDBConn_SQLite3::openMemory() {	
	int ret = sqlite3_open( ":memory:", &p_ );

	if( hasError(ret) ) {
		ax_log( "sqlite error" );
		return axStatus_Std::DB_error_connect;
	}
	return 0;
}

axDBConn_SQLite3::axDBConn_SQLite3() {
	p_ = NULL;
}

axDBConn_SQLite3::~axDBConn_SQLite3() {
	if( p_ ) {
		sqlite3_close( p_ );
		p_ = NULL;
	}
}

bool axDBConn_SQLite3::hasError( int code, const char* sql ) {
	switch( code ) {
		case SQLITE_OK:
		case SQLITE_DONE:
		case SQLITE_ROW:
			return false;
	}

	if( p_ ) {
		if( sql ) {
			ax_log( "SQLite3 Error({?}): {?}\n SQL: {?}", code, sqlite3_errmsg( p_ ), sql );
		}else{
			ax_log( "SQLite3 Error({?}): {?}", code, sqlite3_errmsg( p_ ) );
		}
	}else{
		if( sql ) {
			ax_log( "SQLite3 Error({?})", code  );
		}else{
			ax_log( "SQLite3 Error({?})\n  SQL: {?}", code, sql  );
		}
	}
	return true;
}

//virtual 
axStatus axDBConn_SQLite3::createStmt( axDBStmt & stmt, const char* sql ) {
	axDBStmt_SQLite3* p = new axDBStmt_SQLite3( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setImp( p );
	
	return p->create( sql );	
}

//virtual
axStatus axDBConn_SQLite3::createSQL_DropTableIfExists( axIStringA & outSQL, const char* table ) {
	axTempStringA	tableName;
	axStatus st;
	st = ax_sql_escape_str( tableName, table );						if( !st ) return st;
	st = outSQL.format("DROP TABLE IF EXISTS {?};", tableName );		if( !st ) return st;

	ax_log( "SQL:\n{?}",outSQL );
	return 0;
}

//virtual	
axStatus axDBConn_SQLite3::createSQL_CreateTable ( axIStringA & outSQL, const char* table, const axDB_ColumnList & list ) {
	axStatus st;
	axTempStringA	tableName;
	st = ax_sql_escape_str( tableName, table );						if( !st ) return st;

	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	if( list.size() < 1 ) return axStatus_Std::DB_invalid_param_count;

	if( ! list[0].name.equals("id") )		return axStatus_Std::DB_id_column_is_not_first_one;
	if( list[0].type != axDB_kRowIdType )	return axStatus_Std::DB_id_column_type_error;


	axTempStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDB_Column & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");
		}

		st = ax_sql_escape_str( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}\t{?}", colName, dbTypeName(c.type) );		if( !st ) return st;
	}

	st = outSQL.appendFormat( "\n);" );

	ax_log( "SQL:\n{?}",outSQL );

	return 0;
}

const char*	axDBConn_SQLite3::dbTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:
		case axDB_c_type_int8:
		case axDB_c_type_int16:
		case axDB_c_type_int32:
					return "INTEGER";

		case axDB_c_type_int64:		return "BIGINT";
		case axDB_c_type_float:		return "FLOAT";
		case axDB_c_type_double:	return "DOUBLE";
		case axDB_c_type_StringA:	return "TEXT";
		case axDB_c_type_StringW:	return "TEXT";
		case axDB_c_type_ByteArray:	return "BLOB";
		case axDB_c_type_TimeStamp:	return "DATETIME";
		case axDB_c_type_DateTime:	return "DATETIME";
	}
	assert( false );
	return "Unknown";
}
