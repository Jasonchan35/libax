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
	axPtr< axDBConn_PostgreSQL > p(st);		if( !st ) return st;
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
	return p->create( sql );		
}

//virtual	
axStatus axDBConn_PostgreSQL::getSQL_CreateTable ( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
	axStatus st;
	axTempStringA	tableName;

	st = outSQLArray.resize(1);			if( !st ) return st;

	axIStringA & outSQL = outSQLArray[0];
    
	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");		if( !st ) return st;
		}

		st = identifierString( colName, c.name );		if( !st ) return st;

		bool isPKey = (list.pkeyIndex() == i);

		if( isPKey && list.pkeyAutoInc()  ) {
			switch( c.type ) {
				case axDB_c_type_int32: {
					st = outSQL.appendFormat("  {?}\t{?}", colName, "SERIAL" );			if( !st ) return st;
				}break;

				case axDB_c_type_int64: {
					st = outSQL.appendFormat("  {?}\t{?}", colName, "BIGSERIAL" );		if( !st ) return st;
				}break;

				default:
					return axStatus_Std::DB_invalid_primary_key_type;
			}
		}else{
			st = outSQL.appendFormat("  {?}\t{?}", colName, DBTypeName(c.type) );		if( !st ) return st;
		}

		if( isPKey ) {
			st = outSQL.append(" PRIMARY KEY");		if( !st ) return st;
		}

	}

	st = outSQL.appendFormat( "\n);" );		if( !st ) return st;
	return 0;
}

const char*	axDBConn_PostgreSQL::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:		return "BOOLEAN";
		case axDB_c_type_int8:		return "SMALLINT";
		case axDB_c_type_int16:		return "SMALLINT";
		case axDB_c_type_int32:		return "INTEGER";
		case axDB_c_type_int64:		return "BIGINT";

		case axDB_c_type_float:		return "REAL";
		case axDB_c_type_double:	return "DOUBLE PRECISION";

		case axDB_c_type_StringA:	return "VARCHAR";
		case axDB_c_type_StringW:	return "VARCHAR";

		case axDB_c_type_ByteArray:	return "BYTEA";
		case axDB_c_type_TimeStamp:	return "TIMESTAMP"; //support ( from 4713 BC to 294276 AD )	Resolution( 1 microsecond / 14 digits )
	}
	assert( false );
	return "Unknown";
}