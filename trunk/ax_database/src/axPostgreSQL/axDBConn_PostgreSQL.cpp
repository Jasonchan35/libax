//
//  axDBConn_PostgreSQL.cpp
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_PostgreSQL.h"
#include "axDBStmt_PostgreSQL.h"

axStatus axPostgreSQL_connect ( axDBConn & conn, const char * dsn ) {
	axStatus st;
	axDBConn_PostgreSQL* p = new axDBConn_PostgreSQL;
	if( !p ) return axStatus_Std::not_enough_memory;
	conn._setImp( p );
	return p->connect( dsn );
}

axDBConn_PostgreSQL::axDBConn_PostgreSQL() {
	p_ = NULL;
}

axDBConn_PostgreSQL::~axDBConn_PostgreSQL() {
	close();
}

void axDBConn_PostgreSQL::close() {
	if( p_ ) {
		PQfinish( p_ );
		p_ = NULL;
	}
}

axStatus axDBConn_PostgreSQL::connect( const char * dsn ) {
	p_ = PQconnectdb( dsn );
	if( PQstatus( p_ ) != CONNECTION_OK ) {
		close();
		return axStatus_Std::DB_error_connect;
	}
	return 0;
}

axStatus axDBConn_PostgreSQL::createStmt( axDBStmt & stmt, const char * sql ) {
	axDBStmt_PostgreSQL* p = new axDBStmt_PostgreSQL( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setImp( p );
	return p->prepare( sql );		
}

//virtual	
axStatus axDBConn_PostgreSQL::getSQL_CreateTable ( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
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

const char*	axDBConn_PostgreSQL::dbTypeName( int c_type ) {
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