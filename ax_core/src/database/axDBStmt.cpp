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

axSize		axDBStmt::numColumns	()				{ return p_ ? p_->numColumns() : axSize(0); }
int			axDBStmt::columnType	( axSize col )	{ return p_ ? p_->columnType(col) : axDB_c_type_null; }
const char*	axDBStmt::columnName	( axSize col )	{ return p_ ? p_->columnName(col) : NULL; }

axStatus axDBStmt::create ( axDBConn &db, const char* sql ) { 
	if( ! db._getImp() ) return axStatus_Std::not_initialized;
	return db._getImp()->createStmt( *this, sql);
}

axStatus axDBStmt::exec_ParamList( const axDBInParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	axStatus st;
	st = p_->exec_ParamList( list );	if( !st ) return st;
	return 0;
}

axStatus axDBStmt::getRow_ValueList	( axDBOutParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getRow_ValueList( list );
}

axStatus axDBStmt_Imp::getRow_ValueList( axDBOutParamList & list ) {
	axStatus st;
	st = fetch();		if( !st ) return st;

	if( numColumns() < list.size() ) return axStatus_Std::DB_invalid_param_count;
	
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

axStatus axDBStmt::create_Insert	( axDBConn & db, const char* table, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Insert( sql, table, list );		if( !st ) return st;
	return create( db, sql );
}

axStatus axDBStmt::create_Update	( axDBConn & db, const char* table, const char* szWhere, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Update( sql, table, szWhere, list );		if( !st ) return st;
	return create( db, sql );
}

axStatus axDBStmt::create_Select	( axDBConn & db, const char* table, const char* szWhere, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Select( sql, table, szWhere, list );		if( !st ) return st;
	return create( db, sql );
}