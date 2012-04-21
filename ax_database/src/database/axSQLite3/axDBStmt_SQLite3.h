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

class axDBStmt_SQLite3 : public axDBStmt_Interface {
public:
	axDBStmt_SQLite3( axDBConn_SQLite3* db );
	virtual ~axDBStmt_SQLite3() {}
	
	virtual axStatus	prepare ( const char * sql );	
	virtual	axStatus	exec_ParamList	( const axDB_ParamList & list );

	virtual axStatus	fetch		();
	virtual axSize		colCount	() { return colCount_; }
	virtual int			getValueType	( axSize col );
	virtual const char* getColumnName	( axSize col );
	
	template< class T > axStatus getValue_int( axSize col, T & value );
	virtual axStatus	getValue( axSize col, int8_t	&value );
	virtual axStatus	getValue( axSize col, int16_t	&value );
	virtual axStatus	getValue( axSize col, int32_t	&value );
	virtual axStatus	getValue( axSize col, int64_t	&value );
	
	virtual axStatus	getValue( axSize col, float		&value );
	virtual axStatus	getValue( axSize col, double	&value );
	
	virtual axStatus	getValue( axSize col, bool		&value );
	
	virtual axStatus	getValue( axSize col, axIStringA    &value );
	virtual axStatus	getValue( axSize col, axIStringW    &value );
	
	virtual axStatus	getValue( axSize col, axIByteArray	&value );
	
	virtual axStatus	getValue( axSize col, axTimeStamp	&value );

	
	axSharedPtr< axDBConn_SQLite3 >	db_;
	axStringA	sql_;
	axSize		paramCount_;
	
	axArray< int, 32 >				tmpData;
	axArray< axStringA_<32>, 32 >	strData;
	
	void	releaseStmt();
	
	axSize	colCount_;
	sqlite3_stmt*	stmt_;	
};

#endif //__axDBStmt_SQLite3_h__

