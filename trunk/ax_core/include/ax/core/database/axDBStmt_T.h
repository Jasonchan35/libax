#ifndef __axDBStmt_T_h__
#define __axDBStmt_T_h__

#include "axDBStmt.h"


template<class T, class PKey=int64_t, PKey T::*PKeyMember=&T::id>
class axDBStmt_Insert {
public:
	axStatus	create	( axDBConn & db, const char* table ) {
		axStatus st;
		axTempStringA	sql;
		st = db.getSQL_Insert<T>( sql, table );
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
class axDBStmt_SelectAll {
public:
	axStatus	create	( axDBConn & db, const char* table ) {
		axStatus st;
		st = stmt_.create_Select<T>( db, table, NULL );		if( !st ) return st;
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
		db_ = &db;
		st = table_.set( table );						if( !st ) return st;
		st = stmtInsert.create( db, table );			if( !st ) return st;
		st = stmtUpdate.create( db, table );			if( !st ) return st;
		st = stmtSelect.create( db, table );			if( !st ) return st;
		st = stmtSelectAll.create( db, table );			if( !st ) return st;
		return 0;
	}
	
	axStatus createStmt_Insert ( axDBStmt & stmt ) 							{ return stmt.create_Insert<T>( db_, table_ ); }
	axStatus createStmt_Update ( axDBStmt & stmt, const char* szWhere ) 	{ return stmt.create_Update<T>( db_, table_, szWhere ); }
	axStatus createStmt_Select ( axDBStmt & stmt, const char* szWhere ) 	{ return stmt.create_Select<T>( db_, table_, szWhere ); }

	
	axStatus insert		( const T &v )				{ return stmtInsert.exec( v );	}
	axStatus update		( const T &v )				{ return stmtUpdate.exec( v );	}
	axStatus select		( T &v, const PKey& pkey )	{ return stmtSelect.exec( v, pkey ); }
	axStatus selectAll	( axIArray<T> &v )			{ return stmtSelectAll.execGetAllRow( v ); }

	axDBStmt_SelectAll< T >		stmtSelectAll;
	
private:
	axDBStmt_Insert< T, PKey, PKeyMember > stmtInsert;
	axDBStmt_Update< T, PKey, PKeyMember > stmtUpdate;
	axDBStmt_Select< T, PKey, PKeyMember > stmtSelect;
	axPtr<axDBConn> 	db_;
	axStringA			table_;
};

#endif //__axDBStmt_T_h__
