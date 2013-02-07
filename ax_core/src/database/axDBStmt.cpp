//
//  axDBStmt.cpp
//  ax
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/database/axDBStmt.h>
#include <ax/core/database/axDBConn.h>

axDBStmt::axDBStmt() {
	numColumns_ = 0;
}

void	axDBStmt::_setImp( axDBStmt_Imp* p ) {
	p_.ref( p );
}


axStatus axDBStmt::create ( axDBConn &db, const char* sql ) { 
	if( ! db._getImp() ) return axStatus_Std::not_initialized;
	return db._getImp()->createStmt( *this, sql);
}

axStatus axDBStmt::exec_ParamList( const axDB_ParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	axStatus st;
	st = p_->exec_ParamList( list );	if( !st ) return st;
	numColumns_ = p_->numColumns();
	return 0;
}

axStatus axDBStmt::createExec_ParamList( axDBConn & db, const char* sql, const axDB_ParamList & list ) {
	axStatus st;
	st = create( db, sql );		if( !st ) return st;
	return exec_ParamList( list );
}

axStatus axDBStmt::getRow_ValueList	( axDB_ValueList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getRow_ValueList( list );
}
