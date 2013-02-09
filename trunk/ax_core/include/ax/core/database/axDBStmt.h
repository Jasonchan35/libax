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

		template<class T>	axStatus	create_Insert	( axDBConn & db, const char* table );
		template<class T>	axStatus	create_Insert	( axDBConn & db, const char* table, T & dummy );

		template<class T>	axStatus	create_Update	( axDBConn & db, const char* table,				const char* szWhere );
		template<class T>	axStatus	create_Update	( axDBConn & db, const char* table, T & dummy,	const char* szWhere );

		template<class T>	axStatus	create_Select	( axDBConn & db, const char* table,				const char* szWhere );
		template<class T>	axStatus	create_Select	( axDBConn & db, const char* table, T & dummy,	const char* szWhere );

		//	axStatus	exec			( ... )
			axStatus	exec_ParamList	( const axDBParamList & list );
			axExpandArgList0			( axStatus, exec,   const axDBParam_CB & , axDBParamList, exec_ParamList )
			
		//	axStatus	createExec		( db, sql, ... )
			axStatus	createExec_ParamList( axDBConn & db, const char* sql, const axDBParamList & list );
			axExpandArgList2			( axStatus, createExec, axDBConn &, const char*, const axDBParam_CB & , axDBParamList, createExec_ParamList )

		//	axStatus	getRow			( ... )
			axStatus	getRow_ValueList( axDBValueList & list );
			axExpandArgList0			( axStatus, getRow, const axDBValue_CB & , axDBValueList, getRow_ValueList )
				
			axSize		numColumns		();
			int			columnType		( axSize col );
			const char*	columnName		( axSize col );
					
				void	_setImp			( axDBStmt_Imp* p );

			const char*	sql				();
protected:
	axSharedPtr< axDBStmt_Imp >	p_;
};

//=== insert ===
template<class T> inline
axStatus	axDBStmt::create_Insert( axDBConn & db, const char* table ) {
    axStatus st;
	axTempStringA	sql;
	st = db.createSQL_Insert<T>( sql, table );	if( !st ) return st;
	return create( db, sql );
}

template<class T> inline
axStatus	axDBStmt::create_Insert( axDBConn & db, const char* table, T & dummy ) {
	return create_Insert<T>( db, table );
}

//=== update ==

template<class T> inline
axStatus	axDBStmt::create_Update( axDBConn & db, const char* table, const char* szWhere ) {
    axStatus st;
	axTempStringA	sql;
	st = db.createSQL_Update<T>( sql, table, szWhere );	if( !st ) return st;
	return create( db, sql );
}

template<class T> inline
axStatus	axDBStmt::create_Update( axDBConn & db, const char* table, T & dummy, const char* szWhere ) {
	return create_Update<T>( db, table, szWhere );
}


//=== select ===
template<class T> inline
axStatus	axDBStmt::create_Select( axDBConn & db, const char* table, const char* szWhere ) {
    axStatus st;
	axTempStringA	sql;
	st = db.createSQL_Select<T>( sql, table, szWhere );	if( !st ) return st;
	return create( db, sql );
}

template<class T> inline
axStatus	axDBStmt::create_Select( axDBConn & db, const char* table, T & dummy, const char* szWhere ) {
	return create_Select<T>( db, table, szWhere );
}


//==== Implementation ===

class axDBStmt_Imp :  public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBStmt_Imp() {}
	virtual	axStatus	exec_ParamList	( const axDBParamList & list ) = 0;
	
	virtual axSize		numColumns		() = 0;
	
	virtual int			columnType		( axSize col ) = 0;
	virtual const char* columnName		( axSize col ) = 0;
	
	virtual	axStatus	getRow_ValueList	( axDBValueList & list ) = 0;

	axStringA	sql_;
};

#endif //__axDBStmt_h__
