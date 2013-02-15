//
//  axDBConn.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBConn_h__
#define __axDBConn_h__

#include "axDB_common.h"
#include "axDBColumn.h"

class axDBConn_Imp;
class axDBResultSet;
class axDBStmt;
class axDBInParam;
class axDBInParamList;


class axDBConn : public axNonCopyable {
public:	
    axDBConn();
    ~axDBConn();
						void		close					();

						axStatus	setEchoSQL				( bool b );
						bool		echoSQL					();

						axStatus	escapeString			( axIStringA & out, const char* sz );

						//! SQL identifier for table/column
						axStatus	identifierString		( axIStringA & out, const char* sz );

						axStatus	createTable				( const axDBColumnList & list, const char* table );

			//		TODO
				//		axStatus	setTableAutoIncrement	( const char* table, int64_t   value );
				//		axStatus	getTableAutoIncrement	( const char* table, int64_t & value );
		
						axStatus	dropTable				( const char* table );
						axStatus	dropTableIfExists		( const char* table );

//Get SQL String
						axStatus	getSQL_CreateTable			( axIStringA & outSQL, const axDBColumnList & list, const char* table );
						axStatus	getSQL_CreateTable_Step2	( axIStringA & outSQL, const axDBColumnList & list, const char* table );
						axStatus	getSQL_CreateTable_Step3	( axIStringA & outSQL, const axDBColumnList & list, const char* table );

						axStatus	getSQL_DropTable			( axIStringA & outSQL, const char* table );

						axStatus	getSQL_DropTableIfExists		( axIStringA & outSQL, const char* table );
						axStatus	getSQL_DropTableIfExists_Step2	( axIStringA & outSQL, const char* table );

						axStatus	getSQL_Insert	( axIStringA & outSQL, const axDBColumnList & list, const char* table );
						axStatus	getSQL_Update	( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere );
						axStatus	getSQL_Select	( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere );
    
	void			_setImp	( axDBConn_Imp* p );
	axDBConn_Imp*	_getImp	()	{ return p_; }
private:


	axSharedPtr< axDBConn_Imp >	p_;
};




//!
class axDBConn_Imp : public axNonCopyable, public axSharedPte {
public:
	axDBConn_Imp() : echoSQL_(false) {}
	virtual ~axDBConn_Imp() {}

			axStatus	setEchoSQL	( bool b )	{ echoSQL_ = b; return 0; }
			bool		echoSQL		()			{ return echoSQL_; }

	virtual	axStatus	escapeString				( axIStringA & out, const char* sz );
	virtual	axStatus	identifierString			( axIStringA & out, const char* sz );

	virtual	const char*	DBTypeName					( int c_type ) = 0;

	virtual axStatus	createStmt					( axDBStmt & stmt, const char * sql ) = 0;

	virtual	axStatus	directExec_ArgList			( const char* sql, const axDBInParamList & list ) { assert(false); return axStatus_Std::not_implemented; }

	virtual	axStatus	getSQL_CreateTable				( axIStringA & outSQL, const axDBColumnList & list, const char* table ) = 0;
	virtual	axStatus	getSQL_CreateTable_Step2		( axIStringA & outSQL, const axDBColumnList & list, const char* table );
	virtual	axStatus	getSQL_CreateTable_Step3		( axIStringA & outSQL, const axDBColumnList & list, const char* table );

	virtual axStatus	getSQL_DropTable				( axIStringA & outSQL, const char* table );

	virtual axStatus	getSQL_DropTableIfExists		( axIStringA & outSQL, const char* table );
	virtual axStatus	getSQL_DropTableIfExists_Step2	( axIStringA & outSQL, const char* table );

	virtual	axStatus	getSQL_Insert					( axIStringA & outSQL, const axDBColumnList & list, const char* table );
	virtual	axStatus	getSQL_Update					( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere );
	virtual	axStatus	getSQL_Select					( axIStringA & outSQL, const axDBColumnList & list, const char* table, const char* szWhere );

	bool	echoSQL_;
};

#endif //__axDBConn_h__
