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

axStatus axDBConn::setEchoSQL ( bool b ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->setEchoSQL(b);
}

bool axDBConn::echoSQL () {
	if( !p_ ) return false;
	return p_->echoSQL();
}

axStatus	axDBConn::escapeString( axIStringA & out, const char* sz ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->escapeString( out, sz );
}


axStatus	axDBConn_Imp::escapeString( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );					if( !st ) return st;
	st = tmp.replaceString("'","''");	if( !st ) return st;
	return out.format("'{?}'", tmp );
}

axStatus	axDBConn::identifierString( axIStringA & out, const char* sz ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->identifierString( out, sz );
}

axStatus	axDBConn_Imp::identifierString( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );						if( !st ) return st;
	st = tmp.replaceString("\"","\"\"");	if( !st ) return st;
	return out.format("\"{?}\"", tmp );
}

//== create table ==
axStatus axDBConn::createTable	( const char* table, const axDBColumnList & list ) {
	axStatus	st;
	axTempStringA	sql;
	st = createSQL_CreateTable( sql, table, list );		if( !st ) return st;
	return exec( sql );
}

axStatus axDBConn::createSQL_CreateTable( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_CreateTable( outSQL, table, list );
}

//== drop table ==
axStatus axDBConn::dropTable ( const char* table ) {
	axStatus	st;
	axTempStringA	sql;
	st = createSQL_DropTable( sql, table );			if( !st ) return st;
	return exec( sql );
}

axStatus axDBConn::createSQL_DropTable( axIStringA & outSQL, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_DropTable( outSQL, table );
}

axStatus axDBConn_Imp::createSQL_DropTable( axIStringA & outSQL, const char* table ) {
	axStatus	st;
	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;
	st = outSQL.format("DROP TABLE {?};", tableName);
	return 0;
}

// === drop table if exists
axStatus axDBConn::dropTableIfExists		( const char* table ) {
	axStatus	st;
	axTempStringA	sql;
	st = createSQL_DropTableIfExists( sql, table );			if( !st ) return st;
	return exec( sql );
}

axStatus axDBConn::createSQL_DropTableIfExists( axIStringA & outSQL, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_DropTableIfExists( outSQL, table );
}

axStatus axDBConn_Imp::createSQL_DropTableIfExists(axIStringA &outSQL, const char *table) {
	axStatus st;
	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;
	st = outSQL.format("DROP TABLE IF EXISTS {?};", tableName);
	return 0;
}

//==== insert ===
axStatus axDBConn::createSQL_Insert( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_Insert( outSQL, table, list );
}

axStatus axDBConn_Imp::createSQL_Insert( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	colName;

	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;

	st = outSQL.format("INSERT INTO {?} (\n", tableName );

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");					if( !st ) return st;
		}

		st = identifierString( colName, c.name );		if( !st ) return st;
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
	return 0;
}


//=== update ===
axStatus axDBConn::createSQL_Update( axIStringA & outSQL, const char* table, const axDBColumnList & list , const char* szWhere ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_Update( outSQL, table, list, szWhere );
}

axStatus axDBConn_Imp::createSQL_Update( axIStringA & outSQL, const char* table, const axDBColumnList & list, const char* szWhere ) {
	axStatus st;
	axTempStringA	colName;
	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;


	st = outSQL.format("UPDATE {?} SET\n", tableName );		if( !st ) return st;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");					if( !st ) return st;
		}

		st = identifierString( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}=?", colName );	if( !st ) return st;
	}

	st = outSQL.appendFormat("\n  WHERE {?};", szWhere );	if( !st ) return st;
	return 0;
}

//=== select ====
axStatus axDBConn::createSQL_Select( axIStringA & outSQL, const char* table, const axDBColumnList & list , const char* szWhere ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->createSQL_Select( outSQL, table, list, szWhere );
}

axStatus axDBConn_Imp::createSQL_Select	( axIStringA & outSQL, const char* table, const axDBColumnList & list, const char* szWhere ) {
	axStatus st;
	axTempStringA	colName;
	axTempStringA	tableName;
	st = identifierString( tableName, table );                  if( !st ) return st;

	st = outSQL.format("SELECT\n" );

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");                          if( !st ) return st;
		}

		st = identifierString( colName, c.name );               if( !st ) return st;
		st = outSQL.appendFormat( "  {?}", colName );           if( !st ) return st;
	}

	st = outSQL.appendFormat("\n  FROM {?}", tableName );		if( !st ) return st;
    
    if( szWhere ) {
        st = outSQL.appendFormat( "\n  WHERE {?}", szWhere );   if( !st ) return st;
    }
        
    st = outSQL.append( ";" );                                  if( !st ) return st;
    
	return 0;
}
