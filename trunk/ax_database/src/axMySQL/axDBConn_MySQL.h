//
//  axDBConn_MySQL.h
//  axDB_MySQL
//
//  Created by Jason on 19/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBConn_MySQL_h__
#define __axDBConn_MySQL_h__

#include "axDB_MySQL_common.h"

class axDBConn_MySQL : public axDBConn_Imp {
public:
	axDBConn_MySQL();
	virtual ~axDBConn_MySQL();
	
	axStatus	connect	( const char* db, const char* user, const char* password, const char* host, uint32_t port );
	void		close	();

	virtual axStatus	createStmt			( axDBStmt & stmt, const char * sql );
	virtual	axStatus	getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list );

	virtual	axStatus	identifierString	( axIStringA & out, const char* sz );
		
	operator MYSQL* () { return &p_; }
	
	const char*	DBTypeName( int c_type );

	MYSQL	p_;
};

#endif //__axDBConn_MySQL_h__
