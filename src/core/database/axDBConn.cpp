//
//  axDBConn.cpp
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/database/axDBConn.h>
#include <ax/core/database/axDBStmt.h>

axDBConn::axDBConn() {
}

axDBConn::~axDBConn() {
    close();
}

void axDBConn::close() {
	p_.unref();
}

void axDBConn::_setInstance( axDBConn_Interface* p ) {
	p_.ref( p );
}

axStatus axDBConn::exec_ParamList( axDBStmt &stmt, const char* sql, const axDB_ParamList &list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->exec_ParamList( stmt, sql, list );
}

axStatus axDBConn::prepareStmt( axDBStmt &stmt, const char * sql ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->prepareStmt( stmt, sql );
}

//=========
axStatus axDBConn_Interface::exec_ParamList( axDBStmt &stmt, const char* sql, const axDB_ParamList &list ) {
	axStatus	st;
	st = prepareStmt( stmt, sql );		if( !st ) return st;
	st = stmt.exec_ParamList( list );	if( !st ) return st;	
	return 0;
}
