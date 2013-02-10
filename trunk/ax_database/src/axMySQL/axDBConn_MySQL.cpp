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
	axDBConn_MySQL* p = new axDBConn_MySQL;
	if( !p ) return axStatus_Std::not_enough_memory;
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

axStatus axDBConn_MySQL::createStmt	( axDBStmt & stmt, const char * sql ) {
	axDBStmt_MySQL* p = new axDBStmt_MySQL( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setImp( p );
	return p->prepare( sql );		
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

		st = identifierString( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}\t{?}", colName, dbTypeName(c.type) );		if( !st ) return st;
	}

	st = outSQL.appendFormat( "\n);" );
	return 0;
}

const char*	axDBConn_MySQL::dbTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:
		case axDB_c_type_int8_t:		return "TINYINT";
		case axDB_c_type_int16_t:		return "SMALLINT";
		case axDB_c_type_int32_t:		return "INT";
		case axDB_c_type_int64_t:		return "BIGINT";

		case axDB_c_type_float:			return "FLOAT";
		case axDB_c_type_double:		return "DOUBLE";

		case axDB_c_type_axIStringA:	return "VARCHAR";
		case axDB_c_type_axIStringW:	return "VARCHAR";

		case axDB_c_type_axIByteArray:	return "BLOB";
		case axDB_c_type_axTimeStamp:	return "TIMESTAMP";
		case axDB_c_type_axDateTime:	return "DATETIME";
	}
	assert( false );
	return "Unknown";
}
