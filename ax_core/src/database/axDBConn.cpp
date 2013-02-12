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
	axStatus		st;
	axTempStringA	sql;
	axDBStmt		stmt;

	st = getSQL_CreateTable( sql, table, list );		if( !st ) return st;
	st = stmt.create( *this, sql );						if( !st ) return st;
	st = stmt.exec();									if( !st ) return st;
	return 0;
}

axStatus axDBConn::getSQL_CreateTable( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_CreateTable( outSQL, table, list );
}

//== drop table ==
axStatus axDBConn::dropTable ( const char* table ) {
	axStatus		st;
	axTempStringA	sql;
	axDBStmt		stmt;
	st = getSQL_DropTable( sql, table );			if( !st ) return st;
	st = stmt.create( *this, sql );					if( !st ) return st;
	st = stmt.exec();								if( !st ) return st;
	return 0;
}

axStatus axDBConn::getSQL_DropTable( axIStringA & outSQL, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_DropTable( outSQL, table );
}

axStatus axDBConn_Imp::getSQL_DropTable( axIStringA & outSQL, const char* table ) {
	axStatus	st;
	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;
	st = outSQL.format("DROP TABLE {?};", tableName);
	return 0;
}

// === drop table if exists
axStatus axDBConn::dropTableIfExists		( const char* table ) {
	axStatus		st;
	axTempStringA	sql;
	axDBStmt		stmt;
	st = getSQL_DropTableIfExists( sql, table );		if( !st ) return st;
	st = stmt.create( *this, sql );						if( !st ) return st;
	st = stmt.exec();									if( !st ) return st;
	return 0;
}

axStatus axDBConn::getSQL_DropTableIfExists( axIStringA & outSQL, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_DropTableIfExists( outSQL, table );
}

axStatus axDBConn_Imp::getSQL_DropTableIfExists(axIStringA &outSQL, const char *table) {
	axStatus st;
	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;
	st = outSQL.format("DROP TABLE IF EXISTS {?};", tableName);
	return 0;
}

//==== insert ===
axStatus axDBConn::getSQL_Insert( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_Insert( outSQL, table, list );
}

axStatus axDBConn_Imp::getSQL_Insert( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	colName;

	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;

	st = outSQL.format("INSERT INTO {?} (\n", tableName );

	size_t col = 0;
	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];

		if( list.pkeyAutoInc() && list.pkeyIndex() == i ) {
			continue; //don't insert data to pkeyAutoInc column
		}

		if( col > 0 ) {
			st = outSQL.append(",\n");					if( !st ) return st;
		}
		st = identifierString( colName, c.name );		if( !st ) return st;
		st = outSQL.appendFormat( "  {?}", colName );	if( !st ) return st;
		col++;
	}

	st = outSQL.append( " )\n  VALUES ( " );			if( !st ) return st;

	for( size_t i=0; i<col; i++ ) {
		if( i > 0 ) {
			st = outSQL.append(",");					if( !st ) return st;
		}
		st = outSQL.append( "?" );						if( !st ) return st;
	}

	st = outSQL.append(" );");							if( !st ) return st;
	return 0;
}


//=== update ===
axStatus axDBConn::getSQL_Update( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_Update( outSQL, table, szWhere, list );
}

axStatus axDBConn_Imp::getSQL_Update( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	colName;
	axTempStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;


	st = outSQL.format("UPDATE {?} SET\n", tableName );		if( !st ) return st;

	size_t col = 0;
	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( list.pkeyAutoInc() && list.pkeyIndex() == i ) {
			continue; //MSSQL cannot update pkeyAutoInc column
		}

		if( col > 0 ) {
			st = outSQL.append(",\n");						if( !st ) return st;
		}
		st = identifierString( colName, c.name );			if( !st ) return st;
		st = outSQL.appendFormat( "  {?}\t= ?", colName );	if( !st ) return st;
		col++;
	}

	st = outSQL.appendFormat("\n  WHERE {?};", szWhere );	if( !st ) return st;
	return 0;
}

//=== select ====
axStatus axDBConn::getSQL_Select( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_Select( outSQL, table, szWhere, list );
}

axStatus axDBConn_Imp::getSQL_Select	( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list ) {
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
