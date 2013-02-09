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

						axStatus	exec					( const char* sql );

						axStatus	escapeString			( axIStringA & out, const char* sz );

						//! SQL identifier for table/column
						axStatus	identifierString		( axIStringA & out, const char* sz );

	template<class T>	axStatus	createTable				( const char* table, const T &dummy );
	template<class T>	axStatus	createTable				( const char* table );

						axStatus	dropTable				( const char* table );
						axStatus	dropTableIfExists		( const char* table );

//createSQL
	template<class T>	axStatus	createSQL_CreateTable	( axIStringA & outSQL, const char* table, const T &dummy );
	template<class T>	axStatus	createSQL_CreateTable	( axIStringA & outSQL, const char* table );

						axStatus	createSQL_DropTable		( axIStringA & outSQL, const char* table );
						axStatus	createSQL_DropTableIfExists( axIStringA & outSQL, const char * table );

	template<class T>	axStatus	createSQL_Insert		( axIStringA & outSQL, const char* table, const T &dummy );
	template<class T>	axStatus	createSQL_Insert		( axIStringA & outSQL, const char* table );

	template<class T>	axStatus	createSQL_Update		( axIStringA & outSQL, const char* table, const T &dummy,	const char* szWhere );
	template<class T>	axStatus	createSQL_Update		( axIStringA & outSQL, const char* table, 					const char* szWhere );

	template<class T>	axStatus	createSQL_Select		( axIStringA & outSQL, const char* table, const T &dummy,	const char* szWhere );
	template<class T>	axStatus	createSQL_Select		( axIStringA & outSQL, const char* table, 					const char* szWhere );

    template<class T>   axStatus    getWherePKey            ( axIStringA &wherePKey, T &dummy, void *PKeyMember );    
    
	void			_setImp	( axDBConn_Imp* p );
	axDBConn_Imp*	_getImp	()	{ return p_; }
private:

	axStatus	_createTable			( const char* table, const axDBColumnList & list );

	axStatus	_createSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	axStatus	_createSQL_Insert		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	axStatus	_createSQL_Update		( axIStringA & outSQL, const char* table, const axDBColumnList & list,	const char* szWhere );
	axStatus	_createSQL_Select		( axIStringA & outSQL, const char* table, const axDBColumnList & list,	const char* szWhere );

	axSharedPtr< axDBConn_Imp >	p_;
};


//==== create table ==
template<class T> inline
axStatus	axDBConn::createTable				( const char* table ) {
	T dummy;
	return createTable( table, dummy );
}

template<class T> inline
axStatus	axDBConn::createTable				( const char* table, const T &dummy ) {
	axDBColumnList	list;
	axStatus st = list.io( ax_const_cast(dummy), NULL );		if( !st ) return st;
	return _createTable( table, list );
}

//== create table ==
template<class T> inline
axStatus	axDBConn::createSQL_CreateTable( axIStringA & outSQL, const char* table ) {
	T dummy;
	return createSQL_CreateTable( outSQL, table, dummy );
}
template<class T> inline
axStatus	axDBConn::createSQL_CreateTable( axIStringA & outSQL, const char* table, const T & dummy ) {
	axDBColumnList	list;
	axStatus st = list.io( ax_const_cast(dummy), NULL );		if( !st ) return st;
	return _createSQL_CreateTable( outSQL, table, list );
}


//== insert ==
template<class T> inline
axStatus	axDBConn::createSQL_Insert( axIStringA & outSQL, const char* table ) {
	T dummy;
	return createSQL_Insert( outSQL, table, dummy );
}
template<class T> inline
axStatus	axDBConn::createSQL_Insert( axIStringA & outSQL, const char* table, const T & dummy ) {
	axDBColumnList	list;
	axStatus st = list.io( ax_const_cast(dummy), NULL );		if( !st ) return st;
	return _createSQL_Insert( outSQL, table, list );
}

//== update ==
template<class T> inline
axStatus	axDBConn::createSQL_Update( axIStringA & outSQL, const char* table, const char* szWhere ) {
	T dummy;
	return createSQL_Update( outSQL, table, dummy, szWhere );
}
template<class T> inline
axStatus	axDBConn::createSQL_Update( axIStringA & outSQL, const char* table, const T & dummy, const char* szWhere ) {
	axDBColumnList	list;
	axStatus st = list.io( ax_const_cast(dummy), NULL );		if( !st ) return st;
	return _createSQL_Update( outSQL, table, list, szWhere );
}

//== select ==
template<class T> inline
axStatus	axDBConn::createSQL_Select( axIStringA & outSQL, const char* table, const char* szWhere ) {
	T dummy;
	return createSQL_Select( outSQL, table, dummy, szWhere );
}
template<class T> inline
axStatus	axDBConn::createSQL_Select( axIStringA & outSQL, const char* table, const T & dummy, const char* szWhere ) {
	axDBColumnList	list;
	axStatus st = list.io( ax_const_cast(dummy), NULL );		if( !st ) return st;
	return _createSQL_Select( outSQL, table, list, szWhere );
}



template<class T> inline
axStatus   axDBConn::getWherePKey( axIStringA &wherePKey, T &dummy, void *PKeyMember ) {
	axStatus st ;
	
	axDBColumnList list;
	st = list.io( ax_const_cast(dummy), NULL );				if( !st ) return st;
	axDBColumn *pkey = list.findColumn( PKeyMember );
	
	if( ! pkey ) return axStatus_Std::DB_primary_key_not_found;
	st = identifierString( wherePKey, pkey->name );			if( !st ) return st;
	st = wherePKey.append( "=?" );							if( !st ) return st;

	return 0;
}



//!
class axDBConn_Imp : public axNonCopyable, public axSharedPte {
public:
	axDBConn_Imp() : echoSQL_(false) {}
	virtual ~axDBConn_Imp() {}

			axStatus	setEchoSQL	( bool b )	{ echoSQL_ = b; return 0; }
			bool		echoSQL		()			{ return echoSQL_; }

	virtual	axStatus	escapeString			( axIStringA & out, const char* sz );
	virtual	axStatus	identifierString		( axIStringA & out, const char* sz );

	virtual axStatus	createStmt				( axDBStmt & stmt, const char * sql ) = 0;
	virtual	axStatus	createSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list ) = 0;
	virtual axStatus	createSQL_DropTable		( axIStringA & outSQL, const char * table );
	virtual axStatus	createSQL_DropTableIfExists( axIStringA & outSQL, const char * table );

	virtual	axStatus	createSQL_Insert		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual	axStatus	createSQL_Update		( axIStringA & outSQL, const char* table, const axDBColumnList & list, const char* szWhere );
	virtual	axStatus	createSQL_Select		( axIStringA & outSQL, const char* table, const axDBColumnList & list, const char* szWhere );

	bool	echoSQL_;
};

#endif //__axDBConn_h__
