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

						axStatus	createTable				( const char* table, const axDBColumnList & list );
	template<class T>	axStatus	createTable				( const char* table, const char* pkey=NULL, bool pkeyAutoInc=false );

	template<class T, class PKeyType, PKeyType T::*PKeyMember>	
						axStatus	createTable				( const char* table, bool pkeyAutoInc=false );

			//		TODO
				//		axStatus	setTableAutoIncrement	( const char* table, int64_t   value );
				//		axStatus	getTableAutoIncrement	( const char* table, int64_t & value );
		
						axStatus	dropTable				( const char* table );
						axStatus	dropTableIfExists		( const char* table );

//Get SQL String
						axStatus	getSQL_CreateTable			( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_CreateTable			( axIStringA & outSQL, const char* table, const char* pkey=NULL, bool pkeyAutoInc=false );

						axStatus	getSQL_CreateTable_Step2	( axIStringA & outSQL, const char* table, const axDBColumnList & list );
						axStatus	getSQL_CreateTable_Step3	( axIStringA & outSQL, const char* table, const axDBColumnList & list );

						axStatus	getSQL_DropTable			( axIStringA & outSQL, const char* table );

						axStatus	getSQL_DropTableIfExists		( axIStringA & outSQL, const char* table );
						axStatus	getSQL_DropTableIfExists_Step2	( axIStringA & outSQL, const char* table );

						axStatus	getSQL_Insert	( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_Insert	( axIStringA & outSQL, const char* table, const char* pkey=NULL, bool pkeyAutoInc=false );

						axStatus	getSQL_Update	( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_Update	( axIStringA & outSQL, const char* table, const char* szWhere, const char* pkey=NULL, bool pkeyAutoInc=false );

						axStatus	getSQL_Select	( axIStringA & outSQL, const char* table, const char* szWhere, const axDBColumnList & list );
	template<class T>	axStatus	getSQL_Select	( axIStringA & outSQL, const char* table, const char* szWhere );
    
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

	virtual	axStatus	getSQL_CreateTable				( axIStringA & outSQL, const char* table, const axDBColumnList & list ) = 0;
	virtual	axStatus	getSQL_CreateTable_Step2		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual	axStatus	getSQL_CreateTable_Step3		( axIStringA & outSQL, const char* table, const axDBColumnList & list );

	virtual axStatus	getSQL_DropTable				( axIStringA & outSQL, const char* table );

	virtual axStatus	getSQL_DropTableIfExists		( axIStringA & outSQL, const char* table );
	virtual axStatus	getSQL_DropTableIfExists_Step2	( axIStringA & outSQL, const char* table );

	virtual	axStatus	getSQL_Insert				( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual	axStatus	getSQL_Update				( axIStringA & outSQL, const char* table, const char* szWhere,	const axDBColumnList & list );
	virtual	axStatus	getSQL_Select				( axIStringA & outSQL, const char* table, const char* szWhere,	const axDBColumnList & list );

	bool	echoSQL_;
};


//==== create table ==
template<class T> inline
axStatus	axDBConn::createTable	( const char* table, const char* pkey, bool pkeyAutoInc ) {
	axStatus st;
	axDBColumnList	list;
	st = list.create<T>( pkey, pkeyAutoInc );		if( !st ) return st;
	return createTable( table, list );
}

template<class T, class PKeyType, PKeyType T::*PKeyMember>	
axStatus	axDBConn::createTable	( const char* table, bool pkeyAutoInc ) {
	axStatus st;
	axDBColumnList	list;
	st = list.createByPKeyMember<T, PKeyType, PKeyMember >( pkeyAutoInc );		if( !st ) return st;
	return createTable( table, list );
}

//== create table ==
template<class T> inline
axStatus	axDBConn::getSQL_CreateTable( axIStringA & outSQL, const char* table, const char* pkey, bool pkeyAutoInc ) {
	axStatus st;
	axDBColumnList	list;
	st = list.create<T>( pkey, pkeyAutoInc );		if( !st ) return st;
	return getSQL_CreateTable( outSQL, table, list );
}

//== insert ==
template<class T> inline
axStatus	axDBConn::getSQL_Insert( axIStringA & outSQL, const char* table, const char* pkey, bool pkeyAutoInc ) {
	axStatus st;
	axDBColumnList	list;
	st = list.create<T>( pkey, pkeyAutoInc );		if( !st ) return st;
	return getSQL_Insert( outSQL, table, list );
}

//== update ==
template<class T> inline
axStatus	axDBConn::getSQL_Update( axIStringA & outSQL, const char* table, const char* szWhere, const char* pkey, bool pkeyAutoInc ) {
	axStatus st;
	axDBColumnList	list;
	st = list.create<T>( pkey, pkeyAutoInc );		if( !st ) return st;
	return getSQL_Update( outSQL, table, szWhere, list );
}

//== select ==
template<class T> inline
axStatus	axDBConn::getSQL_Select( axIStringA & outSQL, const char* table, const char* szWhere ) {
	axStatus st;
	axDBColumnList	list;
	st = list.create<T>( NULL, false );			if( !st ) return st;
	return getSQL_Select( outSQL, table, szWhere, list );
}


#endif //__axDBConn_h__
