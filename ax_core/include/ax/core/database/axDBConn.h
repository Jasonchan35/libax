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
class axDB_Param;
class axDB_ParamList;

axStatus	ax_sql_escape_str( axIStringA & out, const char* sz );


class axDBConn : public axNonCopyable {
public:	
    axDBConn();
    ~axDBConn();
	
	void		close	();

						axStatus	exec					( const char* sql );

	template<class T>	axStatus	createTable				( const char* table, const T &dummy );
	template<class T>	axStatus	createTable				( const char* table );

						axStatus	dropTableIfExists		( const char* table );

//createSQL
	template<class T>	axStatus	createSQL_CreateTable	( axIStringA & outSQL, const char* table, const T &dummy );
	template<class T>	axStatus	createSQL_CreateTable	( axIStringA & outSQL, const char* table );

						axStatus	createSQL_DropTableIfExists( axIStringA & outSQL, const char * table );

	template<class T>	axStatus	createSQL_Insert		( axIStringA & outSQL, const char* table, const T &dummy );
	template<class T>	axStatus	createSQL_Insert		( axIStringA & outSQL, const char* table );

	template<class T>	axStatus	createSQL_Update		( axIStringA & outSQL, const char* table, const T &dummy,	const char* szWhere );
	template<class T>	axStatus	createSQL_Update		( axIStringA & outSQL, const char* table, 					const char* szWhere );

	template<class T>	axStatus	createSQL_Select		( axIStringA & outSQL, const char* table, const T &dummy,	const char* szWhere );
	template<class T>	axStatus	createSQL_Select		( axIStringA & outSQL, const char* table, 					const char* szWhere );

	void			_setImp	( axDBConn_Imp* p );
	axDBConn_Imp*	_getImp	()	{ return p_; }
private:

	axStatus	_createTable			( const char* table, const axDB_ColumnList & list );

	axStatus	_createSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDB_ColumnList & list );
	axStatus	_createSQL_Insert		( axIStringA & outSQL, const char* table, const axDB_ColumnList & list );
	axStatus	_createSQL_Update		( axIStringA & outSQL, const char* table, const axDB_ColumnList & list,	const char* szWhere );
	axStatus	_createSQL_Select		( axIStringA & outSQL, const char* table, const axDB_ColumnList & list,	const char* szWhere );

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
	axDB_ColumnList	list;
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
	axDB_ColumnList	list;
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
	axDB_ColumnList	list;
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
	axDB_ColumnList	list;
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
	axDB_ColumnList	list;
	axStatus st = list.io( ax_const_cast(dummy), NULL );		if( !st ) return st;
	return _createSQL_Select( outSQL, table, list, szWhere );
}

class axDBConn_Imp : public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBConn_Imp() {}

	virtual axStatus	createStmt				( axDBStmt & stmt, const char * sql ) = 0;
	virtual	axStatus	createSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDB_ColumnList & list ) = 0;
	virtual axStatus	createSQL_DropTableIfExists( axIStringA & outSQL, const char * table ) = 0;

	virtual	axStatus	createSQL_Insert		( axIStringA & outSQL, const char* table, const axDB_ColumnList & list );
	virtual	axStatus	createSQL_Update		( axIStringA & outSQL, const char* table, const axDB_ColumnList & list, const char* szWhere );
	virtual	axStatus	createSQL_Select		( axIStringA & outSQL, const char* table, const axDB_ColumnList & list, const char* szWhere );
};

#endif //__axDBConn_h__
