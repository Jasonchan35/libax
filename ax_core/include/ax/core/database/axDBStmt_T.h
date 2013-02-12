#ifndef __axDBStmt_T_h__
#define __axDBStmt_T_h__

#include "axDBStmt.h"

template<class T>
class axDBStmt_T {
public:
	axDBStmt_T() 
		: pkeyIndex_(-1)
		, pkeyAutoInc_(false)
	{}

protected:
	axStatus  getWherePKey( axIStringA &out, axDBConn & db, const axDBColumnList & list ) {
		axStatus		st;
		const axDBColumn* col = list.pkeyColumn();
		if( ! col ) return NULL;
		st = db.identifierString( out, col->name );		if( !st ) return st;
		st = out.append( "=?" );						if( !st ) return st;
		return 0;
	}

	axDBStmt	stmt_;
	axSize		pkeyIndex_;
	bool		pkeyAutoInc_;
};

template<class T, class PKeyType, PKeyType T::*PKeyMember >
class axDBStmt_Insert : public axDBStmt_T<T> {
public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		pkeyIndex_		= list.pkeyIndex();
		pkeyAutoInc_	= list.pkeyAutoInc();
		return stmt_.create_Insert( db, table, list );
	}

	axStatus	exec	( const T & values ) {
		axStatus st;
		axDBInParamList	list;
		st = list.create( values, pkeyIndex_ );			if( !st ) return st;
		return stmt_.exec_ParamList( list );
	}

	const char*	sql		() { return stmt_.sql(); }
};

template< class T, class PKeyType, PKeyType T::*PKeyMember >
class axDBStmt_Update : public axDBStmt_T<T> {
public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		axStatus st;
		pkeyIndex_		= list.pkeyIndex();
		pkeyAutoInc_	= list.pkeyAutoInc();

		axStringA whereStr;
		st = getWherePKey( whereStr, db, list );					if( !st ) return st;
		st = stmt_.create_Update( db, table, whereStr, list );		if( !st ) return st;
		return 0;
	}
	axStatus	exec( const T & values ) {
		axStatus st;
		axDBInParamList	list;
		st = list.create( values, pkeyIndex );			if( !st ) return st;
		//add param for "where pkey=?"
		st = list.io( values.*PKeyMember, NULL );		if( !st ) return st;
		return stmt_.exec_ParamList( list );
	}

	const char*	sql	() { return stmt_.sql(); }

private:
	axDBStmt	stmt_;
	axSize		pkeyIndex;
};


template<class T, class PKeyType, PKeyType T::*PKeyMember  >
class axDBStmt_Select : public axDBStmt_T<T> {
public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		axStatus st;
		pkeyIndex_		= list.pkeyIndex();
		pkeyAutoInc_	= list.pkeyAutoInc();

		axStringA whereStr;
		st = getWherePKey( whereStr, db, list );					if( !st ) return st;
		st = stmt_.create_Select( db, table, whereStr, list );		if( !st ) return st;
		return 0;
	}
	axStatus	exec( T & values, const PKeyType &pkey ) {
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
class axDBStmt_SelectAll : public axDBStmt_T<T> {
public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		axStatus st;
		st = stmt_.create_Select( db, table, NULL, list );		if( !st ) return st;
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



template<class T, class PKeyType=int64_t, PKeyType T::*PKeyMember=&T::id>
class axDBTableAccessor {
public:
	/*
		ToDo
		create :
			table, other stmt, 
		dump : rec, all,
		insert : get last id
	 
	*/

	axStatus create( axDBConn & db, const char* table, bool pkeyAutoInc=true ) {
		axStatus st;
		db_ = &db;
		st = table_.set( table );								if( !st ) return st;

		st = list_.createByPKeyMember<T,PKeyType, PKeyMember>( pkeyAutoInc );		
		if( !st ) return st;

		st = stmtInsert.create		( db, table, list_ );		if( !st ) return st;
		st = stmtUpdate.create		( db, table, list_ );		if( !st ) return st;
		st = stmtSelect.create		( db, table, list_ );		if( !st ) return st;
		st = stmtSelectAll.create	( db, table, list_ );		if( !st ) return st;
		return 0;
	}
		
	axStatus insert		( const T &v )					{ return stmtInsert.exec( v );	}
	axStatus update		( const T &v )					{ return stmtUpdate.exec( v );	}
	axStatus select		( T &v, const PKeyType& pkey )	{ return stmtSelect.exec( v, pkey ); }
	axStatus selectAll	( axIArray<T> &v )				{ return stmtSelectAll.execGetAllRow( v ); }

	axDBStmt_SelectAll<T>		stmtSelectAll;
	
private:
	axPtr<axDBConn> 	db_;
	axStringA			table_;
	axDBColumnList		list_;

	axDBStmt_Insert<T, PKeyType, PKeyMember>	stmtInsert;
	axDBStmt_Update<T, PKeyType, PKeyMember>	stmtUpdate;
	axDBStmt_Select<T, PKeyType, PKeyMember>	stmtSelect;
};

#endif //__axDBStmt_T_h__
