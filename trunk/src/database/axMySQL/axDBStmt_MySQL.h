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

class axDBStmt_MySQL : public axDBStmt_Interface {
public:
	axDBStmt_MySQL( axDBConn_MySQL* db );
	virtual ~axDBStmt_MySQL();

	void release();
	
	virtual axStatus	prepare ( const char * sql );	
	virtual	axStatus	exec_ParamList	( const axDB_ParamList & list );

	virtual axStatus	fetch();
	virtual axSize		colCount()	{ return colCount_; }
	
	virtual int			getValueType	( axSize col );
	virtual const char* getColumnName	( axSize col );

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

	axSharedPtr< axDBConn_MySQL >	db_;
	MYSQL_STMT* stmt_;
	
	axArray< MYSQL_BIND, 32 >		bind_;
	axArray< axStringA_<32>, 32 >	tempStr_;
	axArray< unsigned long, 32 >	tempLen_;
	MYSQL_RES*		col_meta_;
	MYSQL_FIELD*	columns_;
	unsigned int	colCount_;
};

template<>	inline axStatus	ax_take( MYSQL_BIND &a, MYSQL_BIND &b ) { a=b; return 0; }



#endif //__axDBStmt_MySQL_h__



