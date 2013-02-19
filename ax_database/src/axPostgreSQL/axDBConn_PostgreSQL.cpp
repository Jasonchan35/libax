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
	lastExecResult_ = NULL;
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
axStatus axDBConn_PostgreSQL::getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	axStatus	st;
	st = outSQL.format("SELECT LASTVAL();" );		if( !st ) return st;
	return 0;
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
		case axDB_c_type_int8:		return "SMALLINT";
		case axDB_c_type_int16:		return "SMALLINT";
		case axDB_c_type_int32:		return "INTEGER";
		case axDB_c_type_int64:		return "BIGINT";

		case axDB_c_type_uint8:		return "SMALLINT";
		case axDB_c_type_uint16:	return "INTEGER";
		case axDB_c_type_uint32:	return "BIGINT";
		case axDB_c_type_uint64:	return "BIGINT";

		case axDB_c_type_bool:		return "BOOLEAN";
		case axDB_c_type_float:		return "REAL";
		case axDB_c_type_double:	return "DOUBLE PRECISION";

		case axDB_c_type_StringA:	return "VARCHAR";
		case axDB_c_type_StringW:	return "VARCHAR";

		case axDB_c_type_blob:	return "BYTEA";
		case axDB_c_type_datetime:	return "TIMESTAMP"; //support ( from 4713 BC to 294276 AD )	Resolution( 1 microsecond / 14 digits )
	}
	assert( false );
	return "Unknown";
}

axStatus	axDBConn_PostgreSQL::_directExec( const char* sql ) {

	if( lastExecResult_ ) {
		lastExecResult_->release();
		lastExecResult_ = NULL;
	}

	if( echoSQL_ ) {
		ax_log("--- ExecSQL: ---\n{?}\n", sql );
	}
	 PGresult *res =  PQexec( p_, sql );
	 if( ! res ) return axStatus_Std::not_enough_memory;

	 PQclear( res );
	 return 0;
}


axStatus	axDBConn_PostgreSQL::beginTran		() { return _directExec( "BEGIN TRANSACTION;" ); }

axStatus	axDBConn_PostgreSQL::rollBackTran	() { return _directExec( "ROLLBACK TRANSACTION;" ); }
axStatus	axDBConn_PostgreSQL::commitTran		() { return _directExec( "COMMIT TRANSACTION;" ); }

axStatus	axDBConn_PostgreSQL::savePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );			if( !st ) return st;
	st = tmp.format("SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_PostgreSQL::rollBackToSavePoint	( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );						if( !st ) return st;
	st = tmp.format("ROLLBACK TO SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}
axStatus	axDBConn_PostgreSQL::releaseSavePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );					if( !st ) return st;
	st = tmp.format("RELEASE SAVEPOINT {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}
