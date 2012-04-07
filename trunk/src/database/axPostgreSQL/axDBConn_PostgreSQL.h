//
//  axDBConn_PostgreSQL.h
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBConn_PostgreSQL_h__
#define __axDBConn_PostgreSQL_h__

#include "axDBStmt_PostgreSQL.h"

class axDBConn_PostgreSQL : public axDBConn_Interface {
public:
	axDBConn_PostgreSQL();
	~axDBConn_PostgreSQL();

	virtual axStatus	prepareStmt		( axDBStmt & stmt, const char * sql );
	
	axStatus	connect	( const char * dsn );
	void		close	();

	operator PGconn*() { return p_; }

    PGconn* p_;
};

#endif //__axDBConn_PostgreSQL_h__
