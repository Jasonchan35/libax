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
	virtual ~axDBStmt_SQLite3();
	
						axStatus	create		( const char * sql );	
						void		destroy		();

				virtual	axStatus	exec_ArgList( const axDBInParamList & list );
				virtual axSize		numParams	();

				virtual axSize		numColumns	() { return numColumns_; }
				virtual int			columnType	( axSize col );
				virtual const char* columnName	( axSize col );
				
				virtual	axStatus	fetch			();

	template< class T > axStatus	_getResultAtCol_int( axSize col, T & value );
				virtual axStatus	getResultAtCol	( axSize col, int8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int64_t			&value );

				virtual axStatus	getResultAtCol	( axSize col, uint8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint64_t			&value );

				virtual axStatus	getResultAtCol	( axSize col, float				&value );
				virtual axStatus	getResultAtCol	( axSize col, double			&value );

				virtual axStatus	getResultAtCol	( axSize col, bool				&value );

				virtual axStatus	getResultAtCol	( axSize col, axIStringA		&value );
				virtual axStatus	getResultAtCol	( axSize col, axIStringW		&value );

				virtual axStatus	getResultAtCol	( axSize col, axIByteArray		&value );

				virtual axStatus	getResultAtCol	( axSize col, axTimeStamp		&value );
				virtual axStatus	getResultAtCol	( axSize col, axDateTime		&value );

				virtual	const char*	sql	() { return sql_; }

	axSharedPtr< axDBConn_SQLite3 >	db_;
	
	axArray< int, axDB_kArgListLocalBufSize >			tmpIntData;
	axArray< axStringA, axDB_kArgListLocalBufSize >		tmpStrData;
	
	
	axTempStringA	sql_;
	axSize			numColumns_;
	sqlite3_stmt*	stmt_;	
};

#endif //__axDBStmt_SQLite3_h__

