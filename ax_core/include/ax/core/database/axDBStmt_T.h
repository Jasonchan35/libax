#ifndef __axDBStmt_T_h__
#define __axDBStmt_T_h__

#include "axDBStmt.h"


template<class T>
class axDBStmt_Insert {
public:
	axStatus	create	( axDBConn & db, const char* table ) {
		axStatus st;
		axTempStringA	sql;
		st = db.createSQL_Insert<T>( sql, table );
		st = stmt_.create( db, sql );	if( !st ) return st;
		return 0;
	}
	axStatus	exec( const T & values ) {
		return stmt_.exec( values );
	}

	const char*	sql	() { return stmt_.sql(); }

private:
	axDBStmt	stmt_;
};

template<class T, class PKey=int64_t, PKey T::*PKeyMember=&T::id>
class axDBStmt_Update {
public:
	axStatus	create	( axDBConn & db, const char* table ) {
		axStatus st;
		axStringA wherePKey;
		T dummy;
		st = db.getWherePKey( wherePKey, dummy, &( dummy.*PKeyMember ) );		if( !st ) return st;
		st = stmt_.create_Update<T>( db, table, wherePKey );					if( !st ) return st;
		return 0;
	}
	axStatus	exec( const T & values ) {
		return stmt_.exec( values, values.*PKeyMember );
	}

	const char*	sql	() { return stmt_.sql(); }

private:
	axDBStmt	stmt_;
};


template<class T, class PKey=int64_t, PKey T::*PKeyMember=&T::id>
class axDBStmt_Select {
public:
	axStatus	create	( axDBConn & db, const char* table ) {
		axStatus st;
		axStringA wherePKey;
		T dummy;
		st = db.getWherePKey( wherePKey, dummy, &( dummy.*PKeyMember ) );		if( !st ) return st;
		st = stmt_.create_Select<T>( db, table, wherePKey );					if( !st ) return st;
		return 0;
	}
	axStatus	exec( T & values, const PKey &pkey ) {
		axStatus st;
		st = stmt_.exec( pkey );		if( !st ) return st;
		st = stmt_.getRow( values );	if( !st ) return st;
		
		return 0;
	}
	
	const char*	sql	() { return stmt_.sql(); }
	
private:
	axDBStmt	stmt_;
};


template<class T >
class axDBStmt_SelectWhere {
public:
	axStatus	create	( axDBConn & db, const char* table, const char* szWhere ) {
		axStatus st;
		st = stmt_.create_Select<T>( db, table, szWhere );	if( !st ) return st;
		return 0;
	}
	axStatus	exec() {
		return stmt_.exec();
	}
	
	axStatus getRow( T & v ) { return stmt_.getRow( v );	}


	axStatus execGetAllRow( axIArray<T> &v )	{
		axStatus st;
		st = exec(); if( !st ) return st;
		
		for(;;) {
			st = v.incSize(1);	if( !st ) return st;
			st = getRow( v.last() );
			if( st.isEOF() )	{ v.decSize(1); return 1; }
			if( !st )			{ v.decSize(1); return st; }
		}
		
		return 0;
	}
	
	const char*	sql	()			{ return stmt_.sql(); }
	
private:
	axDBStmt	stmt_;
};



template<class T, class PKey=int64_t, PKey T::*PKeyMember=&T::id>
class axDBTableAccessor {
public:
	

	/*
		ToDo
		create :
			table, other stmt, 
		dump : rec, all,
		insert : get last id
	 
	*/
	
	axStatus create( axDBConn & db, const char* table ) {
		axStatus st;
		st = _insert.create( db, table );			if( !st ) return st;
		st = _update.create( db, table );			if( !st ) return st;
		st = _select.create( db, table );			if( !st ) return st;
		st = selectAll.create( db, table, NULL );	if( !st ) return st;
		return 0;
	}
	
	axStatus insert( const T &v )				{ return _insert.exec( v );	}
	axStatus update( const T &v )				{ return _update.exec( v );	}
	axStatus select( T &v, const PKey& pkey )	{ return _select.exec( v, pkey ); }
	
	axDBStmt_SelectWhere		< T >			selectAll;
	
private:
	
	axDBStmt_Insert			< T >					_insert;
	axDBStmt_Update			< T, PKey, PKeyMember > _update;
	axDBStmt_Select			< T, PKey, PKeyMember > _select;
	
};

#endif //__axDBStmt_T_h__
