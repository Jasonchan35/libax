//
//  axDBConn_MySQL.cpp
//  axDB_MySQL
//
//  Created by Jason on 19/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_MySQL.h"
#include "axDBStmt_MySQL.h"

axStatus axMySQL_connect ( axDBConn & conn, const char* dbname, const char* user, const char* password, const char* host, uint32_t port ) {
	axStatus st;
	axDBConn_MySQL* p = new axDBConn_MySQL;
	if( !p ) return axStatus_Std::not_enough_memory;
	conn._setInstance( p );
	return p->connect( dbname, user, password, host, port );
}

axDBConn_MySQL::axDBConn_MySQL() {
}

axDBConn_MySQL::~axDBConn_MySQL() {
	close();
}

void axDBConn_MySQL::close() {
	mysql_close( &p_ );
}

axStatus axDBConn_MySQL::connect( const char* dbname, const char* user, const char* password, const char* host, uint32_t port ) {
	mysql_init( &p_ );
	if( ! mysql_real_connect( &p_, host, user, password, dbname, port, NULL, 0 ) ) {
		ax_log( "MySQL Error {?}: {?}", mysql_errno(&p_), mysql_error(&p_) );
		return axStatus_Std::DB_error_connect;
	}
	
	return 0;
}

axStatus axDBConn_MySQL::prepareStmt		( axDBStmt & stmt, const char * sql ) {
	axDBStmt_MySQL* p = new axDBStmt_MySQL( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setInstance( p );
	return p->prepare( sql );		
}


