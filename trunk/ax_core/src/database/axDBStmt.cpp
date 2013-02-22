//
//  axDBStmt.cpp
//  ax
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/database/axDBStmt.h>
#include <ax/core/database/axDBConn.h>
#include <ax/core/system/axLog.h>

axDBStmt::axDBStmt() {
}

void	axDBStmt::_setImp( axDBStmt_Imp* p ) {
	p_.ref( p );
}

const char*	axDBStmt::sql() {
	if( !p_ ) return NULL;
	return p_->sql();
}

axSize		axDBStmt::numParams		()				{ return p_ ? p_->numParams () : axSize(0); }
axSize		axDBStmt::numColumns	()				{ return p_ ? p_->numColumns() : axSize(0); }
int			axDBStmt::columnType	( axSize col )	{ return p_ ? p_->columnType(col) : axDB_c_type_null; }
const char*	axDBStmt::columnName	( axSize col )	{ return p_ ? p_->columnName(col) : NULL; }

axStatus axDBStmt::create ( axDBConn &db, const char* sql ) { 
	if( ! db._getImp() ) return axStatus_Std::not_initialized;
	return db._getImp()->createStmt( *this, sql);
}

axExpandArgList0_Imp ( axStatus axDBStmt::, exec,   const axDBInParam_CB & , axDBInParamList )

axStatus axDBStmt::exec_ArgList( const axDBInParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;

	axStatus st;
	st = p_->exec_ArgList( list );	if( !st ) return st;
	return 0;
}

void axDBStmt_Imp::echoExecSQL( axDBConn_Imp* db, const axDBInParamList & list ) {
	if( db && db->echoSQL() ) {
		ax_log("--- ExecSQL: ---\n{?}\n--- with Params:{?}\n---------------\n", sql(), list );
	}
}


axExpandArgList0_Imp ( axStatus axDBStmt::, getRow, const axDBOutParam_CB & , axDBOutParamList )

axStatus axDBStmt::getRow_ArgList	( axDBOutParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getRow_ArgList( list );
}

axStatus axDBStmt_Imp::getRow_ArgList( axDBOutParamList & list ) {
	axStatus st;
	st = fetch();		if( !st ) return st;

	if( numColumns() != list.size() ) return axStatus_Std::DB_invalid_param_count;
	
	for( axSize i=0; i<list.size(); i++ ) {
		axDBOutParam & v = list[i];
		switch( v.type ) {
			#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
				case axDB_c_type_##NAME: st = getResultAtCol( i, *(C_ITYPE*)v.data );	break; \
			//-----
				#include <ax/core/database/axDB_c_type_list.h>
			#undef axDB_c_type_list
		}
		if( !st ) return st;
	}
	return 0;
}

axStatus axDBStmt::create_Insert	( axDBConn & db, const axDBColumnList & list, const char* table ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Insert( sql, list, table );		if( !st ) return st;
	return create( db, sql );
}

axStatus axDBStmt::create_Update	( axDBConn & db, const axDBColumnList & list, const char* table, const char* szWhere ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Update( sql, list, table, szWhere );		if( !st ) return st;
	return create( db, sql );
}

axStatus axDBStmt::create_Select	( axDBConn & db, const axDBColumnList & list, const char* table, const char* szWhere, const char* szOrder ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Select( sql, list, table, szWhere, szOrder );		if( !st ) return st;
	return create( db, sql );
}

axStatus axDBStmt::create_LastInsertId( axDBConn & db,  const axDBColumnList & list, const char* table ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_LastInsertId( sql, list, table );		if( !st ) return st;
	return create( db, sql );
}
