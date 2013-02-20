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
	axPtr< axDBConn_SQLite3 > p(st);	if( !st ) return st;
	conn._setImp( p );
	return p->openFile( filename );
}

axStatus axSQLite3_openMemory( axDBConn & conn ) {
	axStatus st;
	axPtr< axDBConn_SQLite3 > p(st);	if( !st ) return st;
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
	lastExecStmt_ = NULL;
}

axDBConn_SQLite3::~axDBConn_SQLite3() {
	if( p_ ) {
		if( lastExecStmt_ ) {
			sqlite3_reset( lastExecStmt_);
			lastExecStmt_ = NULL;
		}
		sqlite3_close( p_ );
		p_ = NULL;
	}
}

axStatus	axDBConn_SQLite3::_directExec( const char* sql ) {
	if( echoSQL_ ) {
		ax_log("--- ExecSQL: ---\n{?}\n", sql );
	}

	if( lastExecStmt_ ) {
		sqlite3_reset( lastExecStmt_ );
		lastExecStmt_ = NULL;
	}

	if( !p_ ) return axStatus_Std::not_initialized;
	int ret = sqlite3_exec( p_, sql, NULL, NULL, NULL );
	if( hasError(ret, sql) ) {
		return axStatus_Std::DB_error;
	}
	return 0;
}

bool axDBConn_SQLite3::hasError( int code, const char* sql ) {
	switch( code ) {
		case SQLITE_OK:
		case SQLITE_DONE:
		case SQLITE_ROW:
			return false;
	}

	const char* msg = p_ ? sqlite3_errmsg( p_ ) : "";
	if( sql ) {
		ax_log( "SQLite3 Error({?}): {?}\n SQL: {?}", code, msg, sql );
	}else{
		ax_log( "SQLite3 Error({?}): {?}", code, msg );
	}
	return true;
}

//virtual 
axStatus axDBConn_SQLite3::getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	axStatus	st;
	axStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;

	st = outSQL.format("SELECT ID FROM {?} WHERE _ROWID_ = LAST_INSERT_ROWID();", tableName );		if( !st ) return st;
	return 0;
}


axStatus	axDBConn_SQLite3::beginTran			() { return _directExec( "BEGIN TRANSACTION;" ); }

axStatus	axDBConn_SQLite3::rollBackTran		() { return _directExec( "ROLLBACK TRANSACTION;" ); }
axStatus	axDBConn_SQLite3::commitTran		() { return _directExec( "COMMIT TRANSACTION;" ); }

axStatus	axDBConn_SQLite3::savePoint				( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );			if( !st ) return st;
	st = tmp.format("SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_SQLite3::rollBackToSavePoint	( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );						if( !st ) return st;
	st = tmp.format("ROLLBACK TO SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}
axStatus	axDBConn_SQLite3::releaseSavePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );					if( !st ) return st;
	st = tmp.format("RELEASE SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}



//virtual 
axStatus axDBConn_SQLite3::createStmt( axDBStmt & stmt, const char* sql ) {
	axDBStmt_SQLite3* p = new axDBStmt_SQLite3( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setImp( p );
	return p->create( sql );	
}

//virtual	
axStatus axDBConn_SQLite3::getSQL_CreateTable ( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
	axStatus st;
	axTempStringA	tableName;

	st = outSQLArray.resize(1);		if( !st ) return st;
	axIStringA & outSQL = outSQLArray[0];
    
	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axTempStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");		if( !st ) return st;
		}

		st = identifierString( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}\t{?}", colName, DBTypeName(c.type) );		if( !st ) return st;

		if( list.pkeyIndex() == i ) {
			st = outSQL.append( " PRIMARY KEY" );					if( !st ) return st;
			if( list.pkeyAutoInc() ) {
				st = outSQL.append( " AUTOINCREMENT" );				if( !st ) return st;
			}			
		}
	}

	st = outSQL.appendFormat( "\n);" );		if( !st ) return st;
	return 0;
}

const char*	axDBConn_SQLite3::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:
		case axDB_c_type_int8:
		case axDB_c_type_int16:
		case axDB_c_type_int32:
		case axDB_c_type_int64:
		case axDB_c_type_uint8:
		case axDB_c_type_uint16:
		case axDB_c_type_uint32:
		case axDB_c_type_uint64:
					return "INTEGER"; //in SQLite, AUTO INCREMENT must primary key and INTEGER (even INT/SMALLINT are the same but it is not working)

		case axDB_c_type_float:		return "FLOAT";
		case axDB_c_type_double:	return "DOUBLE";
		case axDB_c_type_StringA:	return "TEXT";
		case axDB_c_type_StringW:	return "TEXT";
		case axDB_c_type_blob:	return "BLOB";
		case axDB_c_type_datetime:	return "DATETIME";
	}
	assert( false );
	return "Unknown";
}
