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
	return p_->sql_;
}

axSize		axDBStmt::numColumns	()				{ return p_ ? p_->numColumns() : 0; }
int			axDBStmt::columnType	( axSize col )	{ return p_ ? p_->columnType(col) : axDB_c_type_null; }
const char*	axDBStmt::columnName	( axSize col )	{ return p_ ? p_->columnName(col) : NULL; }

axStatus axDBStmt::create ( axDBConn &db, const char* sql ) { 
	if( ! db._getImp() ) return axStatus_Std::not_initialized;
	return db._getImp()->createStmt( *this, sql);
}

axStatus axDBStmt::exec_ParamList( const axDBParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	axStatus st;
	st = p_->exec_ParamList( list );	if( !st ) return st;
	return 0;
}

axStatus axDBStmt::createExec_ParamList( axDBConn & db, const char* sql, const axDBParamList & list ) {
	axStatus st;
	st = create( db, sql );		if( !st ) return st;
	return exec_ParamList( list );
}

axStatus axDBStmt::getRow_ValueList	( axDBValueList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getRow_ValueList( list );
}
