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

class axDBConn_PostgreSQL : public axDBConn_Imp {
public:
	axDBConn_PostgreSQL();
	~axDBConn_PostgreSQL();

	virtual axStatus	createStmt			( axDBStmt & stmt, const char * sql );
	virtual	axStatus	getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );
	virtual axStatus	getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table );

	axStatus	connect	( const char * dsn );
	void		close	();

	virtual axStatus	beginTran			();
	virtual axStatus	rollBackTran		();
	virtual axStatus	commitTran			();

	virtual	axStatus	savePoint			( const char* name );
	virtual axStatus	rollBackToSavePoint	( const char* name );
	virtual axStatus	releaseSavePoint	( const char* name );

	axStatus	_directExec( const char* sql );

	const char*	DBTypeName( int c_type );

	operator PGconn*() { return p_; }

	axDBStmt_PostgreSQL::Result*	lastExecResult_;

    PGconn* p_;
};

#endif //__axDBConn_PostgreSQL_h__
