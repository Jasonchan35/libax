//
//  axDBStmt.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBStmt_h__
#define __axDBStmt_h__

#include "axDB_common.h"
#include "axDBConn.h"

class axDBConn;
class axDBStmt_Imp;
class axDBResultSet;

class axDBStmt : public axNonCopyable {
public:	
	axDBStmt();
	
							axStatus	create			( axDBConn & db, const char* sql );

							axStatus	create_Insert	( axDBConn & db, const char* table, const axDBColumnList & list );
		template<class T>	axStatus	create_Insert	( axDBConn & db, const char* table, const char* pkey = NULL, bool pkey_auto_inc = false );

							axStatus	create_Update	( axDBConn & db, const char* table,	const char* szWhere, const axDBColumnList & list );
		template<class T>	axStatus	create_Update	( axDBConn & db, const char* table,	const char* szWhere, const char* pkey = NULL, bool pkey_auto_inc = false );
		template<class T>	axStatus	create_Select	( axDBConn & db, const char* table,	const char* szWhere );

		//	axStatus	exec			( ... )
			axStatus	exec_ParamList	( const axDBParamList & list );
			axExpandArgList0			( axStatus, exec,   const axDBParam_CB & , axDBParamList, exec_ParamList )
			
		//	axStatus	getRow			( ... )
			axStatus	getRow_ValueList( axDBValueList & list );
			axExpandArgList0			( axStatus, getRow, const axDBValue_CB & , axDBValueList, getRow_ValueList )
				
			axSize		numColumns		();
			int			columnType		( axSize col );
			const char*	columnName		( axSize col );
					
				void	_setImp			( axDBStmt_Imp* p );
		axDBStmt_Imp*	_getImp			()		{ return p_; }
			const char*	sql				();
protected:
	axSharedPtr< axDBStmt_Imp >	p_;
};


//==== Implementation ===

class axDBStmt_Imp :  public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBStmt_Imp() {}
	virtual	axStatus	exec_ParamList	( const axDBParamList & list ) = 0;
	
	virtual axSize		numColumns		() = 0;
	
	virtual int			columnType		( axSize col ) = 0;
	virtual const char* columnName		( axSize col ) = 0;
	
	virtual	axStatus	getRow_ValueList	( axDBValueList & list );

	virtual	axStatus	fetch() = 0;

	virtual axStatus	getResultAtCol( axSize col, int8_t			&value )= 0;
	virtual axStatus	getResultAtCol( axSize col, int16_t			&value )= 0;
	virtual axStatus	getResultAtCol( axSize col, int32_t			&value )= 0;
	virtual axStatus	getResultAtCol( axSize col, int64_t			&value )= 0;

	virtual axStatus	getResultAtCol( axSize col, float			&value )= 0;
	virtual axStatus	getResultAtCol( axSize col, double			&value )= 0;

	virtual axStatus	getResultAtCol( axSize col, bool			&value )= 0;

	virtual axStatus	getResultAtCol( axSize col, axIStringA		&value )= 0;
	virtual axStatus	getResultAtCol( axSize col, axIStringW		&value )= 0;

	virtual axStatus	getResultAtCol( axSize col, axIByteArray	&value )= 0;

	virtual axStatus	getResultAtCol( axSize col, axTimeStamp		&value )= 0;
	virtual axStatus	getResultAtCol( axSize col, axDateTime		&value )= 0;

	virtual const char*	sql() = 0;
};


//=== insert ===
template<class T> inline
axStatus	axDBStmt::create_Insert( axDBConn & db, const char* table, const char* pkey, bool pkey_auto_inc ) {
    axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Insert<T>( sql, table );	if( !st ) return st;
	return create( db, sql );
}

//=== update ==

template<class T> inline
axStatus	axDBStmt::create_Update( axDBConn & db, const char* table, const char* szWhere, const char* pkey, bool pkey_auto_inc ) {
    axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Update<T>( sql, table, szWhere, pkey, pkey_auto_inc );	if( !st ) return st;
	return create( db, sql );
}

//=== select ===
template<class T> inline
axStatus	axDBStmt::create_Select( axDBConn & db, const char* table, const char* szWhere ) {
    axStatus st;
	axTempStringA	sql;
	st = db.getSQL_Select<T>( sql, table, szWhere );	if( !st ) return st;
	return create( db, sql );
}

#endif //__axDBStmt_h__
