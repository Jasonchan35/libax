//
//  axDBStmt_MySQL.h
//  axDB_MySQL
//
//  Created by Jason on 19/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBStmt_MySQL_h__
#define __axDBStmt_MySQL_h__

#include "axDB_MySQL_common.h"

class axDBConn_MySQL;

template<>	axStatus	ax_take( MYSQL_BIND &a, MYSQL_BIND &b );

class axDBStmt_MySQL : public axDBStmt_Imp {
public:
	axDBStmt_MySQL( axDBConn_MySQL* db );
	virtual ~axDBStmt_MySQL();

	
			axStatus	create		( const char * sql );	
			void		destroy		();

	virtual	axStatus	exec_ParamList	( const axDBInParamList & list );

	virtual axSize		numColumns	()	{ return numColumns_; }
	virtual int			columnType	( axSize col );
	virtual const char* columnName	( axSize col );

	virtual	axStatus	fetch();

	virtual axStatus	getResultAtCol( axSize col, int8_t		&value );
	virtual axStatus	getResultAtCol( axSize col, int16_t		&value );
	virtual axStatus	getResultAtCol( axSize col, int32_t		&value );
	virtual axStatus	getResultAtCol( axSize col, int64_t		&value );

	virtual axStatus	getResultAtCol( axSize col, float		&value );
	virtual axStatus	getResultAtCol( axSize col, double		&value );

	virtual axStatus	getResultAtCol( axSize col, bool		&value );

	virtual axStatus	getResultAtCol( axSize col, axIStringA		&value );
	virtual axStatus	getResultAtCol( axSize col, axIStringW		&value );

	virtual axStatus	getResultAtCol( axSize col, axIByteArray	&value );

	virtual axStatus	getResultAtCol( axSize col, axTimeStamp		&value );
	virtual axStatus	getResultAtCol( axSize col, axDateTime		&value );

	virtual	const char*	sql	() { return sql_; }

	axSharedPtr< axDBConn_MySQL >	db_;
	axTempStringA		sql_;
	MYSQL_STMT* stmt_;
	
	axArray< MYSQL_BIND, 32 >		bind_;
	axArray< axStringA_<32>, 32 >	tempStr_;
	axArray< unsigned long, 32 >	tempLen_;
	MYSQL_RES*		col_meta_;
	MYSQL_FIELD*	columns_;
	unsigned int	numColumns_;
};

template<>	inline axStatus	ax_take( MYSQL_BIND &a, MYSQL_BIND &b ) { a=b; return 0; }



#endif //__axDBStmt_MySQL_h__



