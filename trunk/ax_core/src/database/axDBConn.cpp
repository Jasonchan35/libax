//
//  axDBConn.cpp
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include <ax/core/database/axDBConn.h>
#include <ax/core/database/axDBStmt.h>
#include <ax/core/system/axLog.h>

axStatus	ax_sql_escape_str( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );					if( !st ) return st;
	st = tmp.replaceString("'","''");	if( !st ) return st;
	return out.format("'{?}'", tmp );
}

axDBConn::axDBConn() {
}

axDBConn::~axDBConn() {
    close();
}

void axDBConn::close() {
	p_.unref();
}

void axDBConn::_setImp( axDBConn_Imp* p ) {
	p_.ref( p );
}

axStatus axDBConn::exec	( const char* sql ) {
	axDBStmt	stmt;
	return stmt.createExec( *this, sql );
}

axStatus axDBConn::_createTable	( const char* table, const axDB_ColumnList & list ) {
	axStatus	st;
	axTempStringA	sql;
	st = _createSQL_CreateTable( sql, table, list );		if( !st ) return st;
	return exec( sql );
}

axStatus axDBConn::dropTableIfExists		( const char* table ) {
	axStatus	st;
	axTempStringA	sql;
	st = createSQL_DropTableIfExists( sql, table );			if( !st ) return st;
	return exec( sql );
}

axStatus axDBConn::_createSQL_CreateTable( axIStringA & outSQL, const char* table, const axDB_ColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_CreateTable( outSQL, table, list );
}

axStatus axDBConn::createSQL_DropTableIfExists( axIStringA & outSQL, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_DropTableIfExists( outSQL, table );
}

axStatus axDBConn::_createSQL_Insert( axIStringA & outSQL, const char* table, const axDB_ColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_Insert( outSQL, table, list );
}

axStatus axDBConn::_createSQL_Update( axIStringA & outSQL, const char* table, const axDB_ColumnList & list , const char* szWhere ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_Update( outSQL, table, list, szWhere );
}


axStatus axDBConn::_createSQL_Select( axIStringA & outSQL, const char* table, const axDB_ColumnList & list , const char* szWhere ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_Select( outSQL, table, list, szWhere );
}

axStatus axDBConn_Imp::createSQL_Insert( axIStringA & outSQL, const char* table, const axDB_ColumnList & list ) {
	axStatus st;
	axTempStringA	colName;

	axTempStringA	tableName;
	st = ax_sql_escape_str( tableName, table );		if( !st ) return st;

	st = outSQL.format("INSERT INTO {?} (\n", tableName );

	for( size_t i=0; i<list.size(); i++ ) {
		const axDB_Column & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");					if( !st ) return st;
		}

		st = ax_sql_escape_str( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}", colName );	if( !st ) return st;
	}

	st = outSQL.append( ")\n  VALUES (" );					if( !st ) return st;

	for( size_t i=0; i<list.size(); i++ ) {
		if( i > 0 ) {
			st = outSQL.append(",");					if( !st ) return st;
		}
		st = outSQL.append( "?" );						if( !st ) return st;
	}

	st = outSQL.append(");");							if( !st ) return st;

	ax_log( "SQL:\n{?}",outSQL );

	return 0;
}


axStatus axDBConn_Imp::createSQL_Update( axIStringA & outSQL, const char* table, const axDB_ColumnList & list, const char* szWhere ) {
	axStatus st;
	axTempStringA	colName;
	axTempStringA	tableName;
	st = ax_sql_escape_str( tableName, table );		if( !st ) return st;


	st = outSQL.format("UPDATE {?} SET\n", tableName );		if( !st ) return st;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDB_Column & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");					if( !st ) return st;
		}

		st = ax_sql_escape_str( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}=?", colName );	if( !st ) return st;
	}

	st = outSQL.appendFormat("\n  WHERE {?};", szWhere );	if( !st ) return st;
	
	ax_log( "SQL:\n{?}",outSQL );

	return 0;
}

axStatus axDBConn_Imp::createSQL_Select	( axIStringA & outSQL, const char* table, const axDB_ColumnList & list, const char* szWhere ) {
	axStatus st;
	axTempStringA	colName;
	axTempStringA	tableName;
	st = ax_sql_escape_str( tableName, table );		if( !st ) return st;

	st = outSQL.format("SELECT\n" );

	for( size_t i=0; i<list.size(); i++ ) {
		const axDB_Column & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");					if( !st ) return st;
		}

		st = ax_sql_escape_str( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}", colName );		if( !st ) return st;
	}

	st = outSQL.appendFormat("\n  FROM {?}\n  WHERE {?};", tableName, szWhere );		if( !st ) return st;

	ax_log( "SQL:\n{?}",outSQL );

	return 0;
}
