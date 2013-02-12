#ifndef __axDBTableAccessor_h__
#define __axDBTableAccessor_h__

#include "axDBStmt.h"

template<class T>
class axDBTableAccessor_Stmt {
public:
	axDBTableAccessor_Stmt() 
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
class axDBTableAccessor_InsertStmt : public axDBTableAccessor_Stmt<T> {
typedef axDBTableAccessor_Stmt<T> B;

public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		B::pkeyIndex_	= list.pkeyIndex();
		B::pkeyAutoInc_	= list.pkeyAutoInc();
		return B::stmt_.create_Insert( db, table, list );
	}

	axStatus	exec	( const T & values ) {
		axStatus st;
		axDBInParamList	list;
		st = list.create( values, B::pkeyIndex_ );			if( !st ) return st;
		return B::stmt_.exec_ParamList( list );
	}

	const char*	sql		() { return B::stmt_.sql(); }
};

template< class T, class PKeyType, PKeyType T::*PKeyMember >
class axDBTableAccessor_UpdateStmt : public axDBTableAccessor_Stmt<T> {
	typedef axDBTableAccessor_Stmt<T> B;
public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		axStatus st;
		B::pkeyIndex_	= list.pkeyIndex();
		B::pkeyAutoInc_	= list.pkeyAutoInc();

		axStringA whereStr;
		st = B::getWherePKey( whereStr, db, list );					if( !st ) return st;
		st = B::stmt_.create_Update( db, table, whereStr, list );		if( !st ) return st;
		return 0;
	}
	axStatus	exec( const T & values ) {
		axStatus st;
		axDBInParamList	list;
		st = list.create( values, B::pkeyIndex_ );		if( !st ) return st;
		//add param for "where pkey=?"
		st = list.io( values.*PKeyMember, NULL );		if( !st ) return st;
		return B::stmt_.exec_ParamList( list );
	}

	const char*	sql	() { return B::stmt_.sql(); }

};


template<class T, class PKeyType, PKeyType T::*PKeyMember  >
class axDBTableAccessor_SelectStmt : public axDBTableAccessor_Stmt<T> {
typedef axDBTableAccessor_Stmt<T> B;
public:
	axStatus	create	( axDBConn & db, const char* table, const axDBColumnList & list ) {
		axStatus st;
		B::pkeyIndex_	= list.pkeyIndex();
		B::pkeyAutoInc_	= list.pkeyAutoInc();

		axStringA whereStr;
		st = B::getWherePKey( whereStr, db, list );					if( !st ) return st;
		st = B::stmt_.create_Select( db, table, whereStr, list );		if( !st ) return st;
		return 0;
	}
	axStatus	exec( T & values, const PKeyType &pkey ) {
		axStatus st;
		st = B::stmt_.exec( pkey );		if( !st ) return st;
		st = B::stmt_.getRow( values );	if( !st ) return st;
		return 0;
	}
	
	const char*	sql	() { return B::stmt_.sql(); }

};


template<class T >
class axDBTableAccessor_SelectMultipleStmt : public axDBTableAccessor_Stmt<T> {
	typedef axDBTableAccessor_Stmt<T> B;
public:
	axStatus	create	( axDBConn & db, const char* table, const char* szWhere, const axDBColumnList & list ) {
		axStatus st;
		st = B::stmt_.create_Select( db, table, szWhere, list );		if( !st ) return st;
		return 0;
	}
	axStatus	exec_ParamList	( const axDBInParamList &params )	{ return B::stmt_.exec_ParamList( params ); }
	axStatus	exec			()									{ return B::stmt_.exec(); }
	
	axStatus	getRow			( T & v )							{ return B::stmt_.getRow( v ); }
	axStatus	getAllRows		( axIArray<T> &v )	{
		axStatus st;
		for(;;) {
			st = v.incSize(1);	if( !st ) return st;
			st = getRow( v.last() );
			if( st.isEOF() )	{ v.decSize(1); return 1; }
			if( !st )			{ v.decSize(1); return st; }
		}
		return 0;
	}
	
	const char*	sql	()			{ return B::stmt_.sql(); }
	
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

		st = insertStmt.create		( db, table, list_ );			if( !st ) return st;
		st = updateStmt.create		( db, table, list_ );			if( !st ) return st;
		st = selectStmt.create		( db, table, list_ );			if( !st ) return st;
		st = selectAllStmt.create	( db, table, NULL, list_ );		if( !st ) return st;
		return 0;
	}
		
	axStatus insert		( const T &v )					{ return insertStmt.exec( v );	}
	axStatus update		( const T &v )					{ return updateStmt.exec( v );	}

	axStatus select		( T &v, const PKeyType& pkey )	{ return selectStmt.exec( v, pkey ); }

	axStatus selectAll	( axIArray<T> &rows ) { 
		axStatus st = selectAllStmt.exec();			if( !st ) return st;
		return selectAllStmt.getAllRows( rows ); 
	}

//	axStatus selectWhere			( axIArray<T> &rows, const char* szWhere, params... );
	axStatus selectWhere_ParamList	( axIArray<T> &rows, const char* szWhere, const axDBInParamList & params );
	axExpandArgList2 ( axStatus,  selectWhere, axIArray<T> & /*rows*/, const char* /*szWhere*/, 
						const axDBInParam_CB &, axDBInParamList, selectWhere_ParamList );

	axDBTableAccessor_InsertStmt<T, PKeyType, PKeyMember>	insertStmt;
	axDBTableAccessor_UpdateStmt<T, PKeyType, PKeyMember>	updateStmt;
	axDBTableAccessor_SelectStmt<T, PKeyType, PKeyMember>	selectStmt;
	axDBTableAccessor_SelectMultipleStmt<T>					selectAllStmt;
	
private:
	axPtr<axDBConn> 	db_;
	axStringA			table_;
	axDBColumnList		list_;
};


template<class T, class PKeyType, PKeyType T::*PKeyMember>
axStatus axDBTableAccessor<T, PKeyType, PKeyMember>::selectWhere_ParamList ( axIArray<T> &rows, const char* szWhere, const axDBInParamList & params ) {
	axDBTableAccessor_SelectMultipleStmt<T>	stmt;
	axStatus st;
	st = stmt.create( *db_, table_, szWhere, list_ );	if( !st ) return st;
	st = stmt.exec_ParamList( params );					if( !st ) return st;
	st = stmt.getAllRows( rows );						if( !st ) return st;
	return 0;
}


#endif //__axDBTableAccessor_h__
