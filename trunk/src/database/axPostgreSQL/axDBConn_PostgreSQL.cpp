//
//  axDBConn_PostgreSQL.cpp
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDBConn_PostgreSQL.h"
#include "axDBStmt_PostgreSQL.h"

axStatus axPostgreSQL_connect ( axDBConn & conn, const char * dsn ) {
	axStatus st;
	axDBConn_PostgreSQL* p = new axDBConn_PostgreSQL;
	if( !p ) return axStatus_Std::not_enough_memory;
	conn._setInstance( p );
	return p->connect( dsn );
}

axDBConn_PostgreSQL::axDBConn_PostgreSQL() {
	p_ = NULL;
}

axDBConn_PostgreSQL::~axDBConn_PostgreSQL() {
	close();
}

void axDBConn_PostgreSQL::close() {
	if( p_ ) {
		PQfinish( p_ );
		p_ = NULL;
	}
}

axStatus axDBConn_PostgreSQL::connect( const char * dsn ) {
	p_ = PQconnectdb( dsn );
	if( PQstatus( p_ ) != CONNECTION_OK ) {
		close();
		return axStatus_Std::DB_error_connect;
	}
	return 0;
}

axStatus axDBConn_PostgreSQL::prepareStmt( axDBStmt & stmt, const char * sql ) {
	axDBStmt_PostgreSQL* p = new axDBStmt_PostgreSQL( this );
	if( !p ) return axStatus_Std::not_enough_memory;	
	stmt._setInstance( p );
	return p->prepare( sql );		
}



