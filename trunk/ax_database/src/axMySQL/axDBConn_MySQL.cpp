//
//  axDBConn_MySQL.cpp
//  axDB_MySQL
//
//  Created by Jason on 19/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_MySQL.h"
#include "axDBStmt_MySQL.h"

axStatus axMySQL_connect ( axDBConn & conn, const char* dbname, const char* user, const char* password, const char* host, uint32_t port ) {
	axStatus st;
	axPtr< axDBConn_MySQL > p(st);		if( !st ) return st;
	conn._setImp( p );
	return p->connect( dbname, user, password, host, port );
}

axDBConn_MySQL::axDBConn_MySQL() {
}

axDBConn_MySQL::~axDBConn_MySQL() {
	close();
}

void axDBConn_MySQL::close() {
	mysql_close( &p_ );
}

axStatus axDBConn_MySQL::connect( const char* dbname, const char* user, const char* password, const char* host, uint32_t port ) {
	mysql_init( &p_ );
	if( ! mysql_real_connect( &p_, host, user, password, dbname, port, NULL, 0 ) ) {
		ax_log( "MySQL Error {?}: {?}", mysql_errno(&p_), mysql_error(&p_) );
		return axStatus_Std::DB_error_connect;
	}
	
	return 0;
}

axStatus	axDBConn_MySQL::identifierString( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );						if( !st ) return st;
	st = tmp.replaceString("`","``");		if( !st ) return st;
	return out.format("`{?}`", tmp );
}

axStatus axDBConn_MySQL::createStmt	( axDBStmt & stmt, const char * sql ) {
	axDBStmt_MySQL* p = new axDBStmt_MySQL( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setImp( p );
	return p->create( sql );		
}


//virtual	
axStatus axDBConn_MySQL::getSQL_CreateTable ( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	tableName;
    
	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axTempStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");
		}

		st = identifierString( colName, c.name );					if( !st ) return st;
		st = outSQL.appendFormat( "  {?}\t{?}", colName, DBTypeName(c.type) );		if( !st ) return st;

		if( list.pkeyIndex() == i ) {
			st = outSQL.append( " PRIMARY KEY" );					if( !st ) return st;
			if( list.pkeyAutoInc() ) {
				st = outSQL.append( " AUTO_INCREMENT" );			if( !st ) return st;
			}
		}
	}

	st = outSQL.appendFormat( "\n);" );					if( !st ) return st;
	return 0;
}

const char*	axDBConn_MySQL::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:			return "TINYINT";
		case axDB_c_type_int8:			return "TINYINT";
		case axDB_c_type_int16:			return "SMALLINT";
		case axDB_c_type_int32:			return "INT";
		case axDB_c_type_int64:			return "BIGINT";

		case axDB_c_type_float:			return "FLOAT";
		case axDB_c_type_double:		return "DOUBLE";

		case axDB_c_type_StringA:		return "TEXT";
		case axDB_c_type_StringW:		return "TEXT";

		case axDB_c_type_ByteArray:		return "BLOB";

		// TIMESTAMP in MySQL is 32bit, so only support ( '1970-01-01 00:00:01' UTC to '2038-01-19 03:14:07' UTC )
		// therefore we using DATETIME instead ( '1000-01-01 00:00:00' to '9999-12-31 23:59:59'.  )
		case axDB_c_type_TimeStamp:		return "DATETIME";	
	}
	assert( false );
	return "Unknown";
}
