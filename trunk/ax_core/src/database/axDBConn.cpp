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

axStatus	axDBConn_Imp::identifierString( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );						if( !st ) return st;
	st = tmp.replaceString("\"","\"\"");	if( !st ) return st;
	return out.format("\"{?}\"", tmp );
}

axStatus	axDBConn::identifierString( axIStringA & out, const char* sz ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->identifierString( out, sz );
}

axStatus	axDBConn::beginTran	() {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->beginTran();
}

axStatus	axDBConn::rollbackTran	() {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->rollbackTran();
}

axStatus	axDBConn::commitTran	() {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->commitTran();
}


//== create table ==
axStatus axDBConn::createTable	( const axDBColumnList & list, const char* table ) {
	axStatus		st;
	axStringA_Array	sql;

	st = getSQL_CreateTable( sql, list, table );	if( !st ) return st;

	for( size_t i=0; i<sql.size(); i++ ) {
		axDBStmt stmt;
		st = stmt.create( *this, sql[i] );			if( !st ) return st;
		st = stmt.exec();							if( !st ) return st;
	}

	return 0;
}

axStatus axDBConn::getSQL_CreateTable( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_CreateTable( outSQLArray, list, table );
}

//== drop table ==
axStatus axDBConn::dropTable ( const char* table ) {
	axStatus		st;
	axStringA_Array	sql;

	st = getSQL_DropTable( sql, table );		if( !st ) return st;

	for( size_t i=0; i<sql.size(); i++ ) {
		axDBStmt stmt;
		st = stmt.create( *this, sql[i] );			if( !st ) return st;
		st = stmt.exec();							if( !st ) return st;
	}

	return 0;
}

axStatus axDBConn::getSQL_DropTable( axStringA_Array & outSQLArray, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_DropTable( outSQLArray, table );
}

axStatus axDBConn_Imp::getSQL_DropTable( axStringA_Array & outSQLArray, const char* table ) {
	axStatus	st;

	st = outSQLArray.resize(1);		if( !st ) return st;

	axIStringA & outSQL = outSQLArray[0];

	axTempStringA	tableName;
	st = identifierString( tableName, table );			if( !st ) return st;
	st = outSQL.format("DROP TABLE {?};", tableName);	if( !st ) return st;
	return 0;
}

// === drop table if exists
axStatus axDBConn::dropTableIfExists		( const char* table ) {
	axStatus		st;
	axStringA_Array	sql;
	st = getSQL_DropTableIfExists( sql, table );	if( !st ) return st;

	for( size_t i=0; i<sql.size(); i++ ) {
		axDBStmt stmt;
		st = stmt.create( *this, sql[i] );			if( !st ) return st;
		st = stmt.exec();							if( !st ) return st;
	}

	return 0;
}

axStatus axDBConn::getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_DropTableIfExists( outSQLArray, table );
}
axStatus axDBConn_Imp::getSQL_DropTableIfExists(axStringA_Array &outSQLArray, const char *table) {
	axStatus st;
	axTempStringA	tableName;

	st = outSQLArray.resize(1);		if( !st ) return st;
	axIStringA & outSQL = outSQLArray[0];

	st = identifierString( tableName, table );							if( !st ) return st;
	st = outSQL.format("DROP TABLE IF EXISTS {?};", tableName);		if( !st ) return st;
	return 0;
}

axStatus  axDBConn::getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_LastInsertId( outSQL, list, table );
}

//==== insert ===
axStatus axDBConn::getSQL_Insert( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_Insert( outSQL, list, table );
}

axStatus axDBConn_Imp::getSQL_Insert( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
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
axStatus axDBConn::getSQL_Update( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_Update( outSQL, list, table, szWhere );
}

axStatus axDBConn_Imp::getSQL_Update( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere ) {
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
axStatus axDBConn::getSQL_Select( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getSQL_Select( outSQL, list, table, szWhere );
}

axStatus axDBConn_Imp::getSQL_Select ( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere ) {
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
