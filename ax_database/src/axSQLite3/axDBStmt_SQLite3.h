//
//  axDBStmt_SQLite3.h
//  axDB_SQLite3
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBStmt_SQLite3_h__
#define __axDBStmt_SQLite3_h__

#include <ax/ax_core.h>

#if axOS_WIN
	#include "../../external/sqlite3/sqlite3.h"
#else
	#include <sqlite3.h>
#endif


class axDBConn_SQLite3;

class axDBStmt_SQLite3 : public axDBStmt_Imp {
public:
	axDBStmt_SQLite3( axDBConn_SQLite3* db );
	virtual ~axDBStmt_SQLite3() {}
	
			 axStatus	create ( const char * sql );	

	virtual	axStatus	exec_ParamList	( const axDB_ParamList & list );

	virtual axSize		numColumns	() { return numColumns_; }
	virtual int			columnType	( axSize col );
	virtual const char* columnName	( axSize col );
	
	virtual	axStatus	getRow_ValueList( axDB_ValueList & list );

			axStatus	fetch		();

	template< class T > axStatus getResultAtCol_int( axSize col, T & value );
			axStatus	getResultAtCol	( axSize col, int8_t			&value );
			axStatus	getResultAtCol	( axSize col, int16_t			&value );
			axStatus	getResultAtCol	( axSize col, int32_t			&value );
			axStatus	getResultAtCol	( axSize col, int64_t			&value );
	
			axStatus	getResultAtCol	( axSize col, float				&value );
			axStatus	getResultAtCol	( axSize col, double			&value );
	
			axStatus	getResultAtCol	( axSize col, bool				&value );
	
			axStatus	getResultAtCol	( axSize col, axIStringA		&value );
			axStatus	getResultAtCol	( axSize col, axIStringW		&value );
	
			axStatus	getResultAtCol	( axSize col, axIByteArray		&value );
	
			axStatus	getResultAtCol	( axSize col, axTimeStamp		&value );
			axStatus	getResultAtCol	( axSize col, axDateTime		&value );

	axSharedPtr< axDBConn_SQLite3 >	db_;
	axStringA	sql_;
	axSize		paramCount_;
	
	axArray< int, axDB_kArgListLocalBufSize >			tmpIntData;
	axArray< axStringA, axDB_kArgListLocalBufSize >		tmpStrData;
	
	void	releaseStmt();
	
	axSize	numColumns_;
	sqlite3_stmt*	stmt_;	
};

#endif //__axDBStmt_SQLite3_h__

