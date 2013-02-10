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

class axDBConn_Imp;
class axDBResultSet;
class axDBStmt;
class axDBParam;
class axDBParamList;



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

						axStatus	createTable				( const char* table, const axDBColumnList & list );
	template<class T>	axStatus	createTable				( const char* table, const char* pkey );
	
			//		TODO
				//		axStatus	setTableAutoIncrement	( const char* table, int64_t   value );
				//		axStatus	getTableAutoIncrement	( const char* table, int64_t & value );
		
						axStatus	dropTable				( const char* table );
						axStatus	dropTableIfExists		( const char* table );

//Get SQL String
						axStatus	getSQL_CreateTable		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_CreateTable		( axIStringA & outSQL, const char* table, const char* pkey );

						axStatus	getSQL_DropTable		( axIStringA & outSQL, const char* table );
						axStatus	getSQL_DropTableIfExists( axIStringA & outSQL, const char* table );

						axStatus	getSQL_Insert			( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_Insert			( axIStringA & outSQL, const char* table, const char* pkey );

						axStatus	getSQL_Update			( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_Update			( axIStringA & outSQL, const char* table, const char* szWhere );

						axStatus	getSQL_Select			( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_Select			( axIStringA & outSQL, const char* table, const char* szWhere );

	template<class T, class PKey, PKey T::*PKeyMember>   
						axStatus    getWherePKey           	( axIStringA &out );    
    
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

	virtual	axStatus	escapeString			( axIStringA & out, const char* sz );
	virtual	axStatus	identifierString		( axIStringA & out, const char* sz );

			axStatus	createStmt				( axDBStmt & stmt, const char * sql );
	virtual axStatus	onCreateStmt			( axDBStmt & stmt, const char * sql ) = 0;

	virtual	axStatus	getSQL_CreateTable		( axIStringA & outSQL, const char* table, const axDBColumnList & list ) = 0;
	virtual axStatus	getSQL_DropTable		( axIStringA & outSQL, const char* table );
	virtual axStatus	getSQL_DropTableIfExists( axIStringA & outSQL, const char* table );

	virtual	axStatus	getSQL_Insert			( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual	axStatus	getSQL_Update			( axIStringA & outSQL, const char* table, const char* szWhere,	const axDBColumnList & list );
	virtual	axStatus	getSQL_Select			( axIStringA & outSQL, const char* table, const char* szWhere,	const axDBColumnList & list );

	bool	echoSQL_;
};


//==== create table ==
template<class T> inline
axStatus	axDBConn::createTable	( const char* table, const char* pkey ) {
	axStatus st;
	axDBColumnList	list;
	st = list.build<T>( pkey );		if( !st ) return st;
	return createTable( table, list );
}

//== create table ==
template<class T> inline
axStatus	axDBConn::getSQL_CreateTable( axIStringA & outSQL, const char* table, const char* pkey ) {
	axStatus st;
	axDBColumnList	list;
	st = list.build<T>( pkey );		if( !st ) return st;
	return getSQL_CreateTable( outSQL, table, list );
}

//== insert ==
template<class T> inline
axStatus	axDBConn::getSQL_Insert( axIStringA & outSQL, const char* table, const char* pkey ) {
	axStatus st;
	axDBColumnList	list;
	st = list.build<T>( pkey );		if( !st ) return st;
	return getSQL_Insert( outSQL, table, list );
}

//== update ==
template<class T> inline
axStatus	axDBConn::getSQL_Update( axIStringA & outSQL, const char* table, const char* szWhere ) {
	axStatus st;
	axDBColumnList	list;
	st = list.build<T>( NULL );		if( !st ) return st;
	return getSQL_Update( outSQL, table, szWhere, list );
}

//== select ==
template<class T> inline
axStatus	axDBConn::getSQL_Select( axIStringA & outSQL, const char* table, const char* szWhere ) {
	axStatus st;
	axDBColumnList	list;
	st = list.build<T>( NULL );		if( !st ) return st;
	return getSQL_Select( outSQL, table, szWhere, list );
}

//=====
template<class T, class PKey, PKey T::*PKeyMember> inline
axStatus   axDBConn::getWherePKey( axIStringA &out ) {
	axStatus		st;
	T				t;

	axDBColumnList	list;
	st = list.io( t, NULL );					if( !st ) return st;

	axDBColumn* pkey = list.findColumnByData( &(t.*PKeyMember) );
	if( ! pkey ) return axStatus_Std::DB_primary_key_not_found;

	st = identifierString( out, pkey->name );	if( !st ) return st;
	st = out.append( "=?" );					if( !st ) return st;

	return 0;
}


#endif //__axDBConn_h__
