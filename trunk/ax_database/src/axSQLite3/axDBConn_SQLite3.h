//
//  axSQLite.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBConn_SQLite_h__
#define __axDBConn_SQLite_h__

#include "axDBStmt_SQLite3.h"

class axDBConn_SQLite3 : public axDBConn_Imp {
public:
	axDBConn_SQLite3();
	virtual ~axDBConn_SQLite3();

	virtual axStatus	createStmt			( axDBStmt & stmt, const char * sql );
	virtual	axStatus	getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );

	virtual axStatus	getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table );

			axStatus	openFile	( const char* filename );
			axStatus	openMemory	();
			
			bool		hasError	( int code, const char* sql = NULL );
			
	operator sqlite3* () { return p_; }
			

	const char*	DBTypeName( int c_type );

	sqlite3* p_;
};



#endif //__axDBConn_SQLite_h__

