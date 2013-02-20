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
	axStatus  getWherePKey( axIStringA &out, const axDBColumnList & list, axDBConn & db ) {
		axStatus		st;
		const axDBColumn* col = list.pkeyColumn();
		if( ! col ) return axStatus_Std::DB_primary_key_not_found;
		st = db.identifierString( out, col->name );		if( !st ) return st;
		st = out.append( "=?" );						if( !st ) return st;
		return 0;
	}

	axDBStmt	stmt_;
	axSize		pkeyIndex_;
	bool		pkeyAutoInc_;
};

template<class T, class PKeyType, PKeyType T::*PKeyMember, bool PKeyAutoInc >
class axDBTableAccessor_InsertStmt : public axDBTableAccessor_Stmt<T> {
typedef axDBTableAccessor_Stmt<T> B;

public:
	axStatus	create	( axDBConn & db, const axDBColumnList & list, const char* table ) {
		axStatus st;

		B::pkeyIndex_	= list.pkeyIndex();
		B::pkeyAutoInc_	= list.pkeyAutoInc();

		if( B::pkeyAutoInc_ ) {
			const axDBColumn * p = list.pkeyColumn();
			if( !p ) return axStatus_Std::DB_error;
			st = pkeyName_.set( p->name );		if( !st ) return st;

			st = lastInsertIdStmt_.create_LastInsertId( db, list, table );	if( !st ) return st;
		}

		return B::stmt_.create_Insert( db, list, table );
	}

	axStatus	exec	( T & values ) {

		axStatus st;
		axDBInParamList	list;
		st = list.create( values, B::pkeyIndex_ );		if( !st ) return st;
		st = B::stmt_.exec_ArgList( list );				if( !st ) return st;

		if( B::pkeyAutoInc_ ) {
			st = lastInsertIdStmt_.exec();							if( !st ) return st;
			st = lastInsertIdStmt_.getRow( values.*PKeyMember );	if( !st ) return st;
		}

		return 0;
	}

	const char*	sql		() { return B::stmt_.sql(); }
private:
	axDBStmt	lastInsertIdStmt_;

	axStringA	pkeyName_;
};

template< class T, class PKeyType, PKeyType T::*PKeyMember, bool PKeyAutoInc >
class axDBTableAccessor_UpdateStmt : public axDBTableAccessor_Stmt<T> {
	typedef axDBTableAccessor_Stmt<T> B;
public:
	axStatus	create	( axDBConn & db, const axDBColumnList & list, const char* table ) {
		axStatus st;
		B::pkeyIndex_	= list.pkeyIndex();
		B::pkeyAutoInc_	= list.pkeyAutoInc();

		axStringA whereStr;
		st = B::getWherePKey( whereStr, list, db );					if( !st ) return st;
		st = B::stmt_.create_Update( db, list, table, whereStr );	if( !st ) return st;
		return 0;
	}
	axStatus	exec( const T & values ) {
		axStatus st;
		axDBInParamList	list;
		st = list.create( values, B::pkeyIndex_ );		if( !st ) return st;
		//add param for "where pkey=?"
		st = list.io( values.*PKeyMember, NULL );		if( !st ) return st;
		return B::stmt_.exec_ArgList( list );
	}

	const char*	sql	() { return B::stmt_.sql(); }

};


template<class T, class PKeyType, PKeyType T::*PKeyMember, bool PKeyAutoInc >
class axDBTableAccessor_SelectStmt : public axDBTableAccessor_Stmt<T> {
typedef axDBTableAccessor_Stmt<T> B;
public:
	axStatus	create	( axDBConn & db, const axDBColumnList & list, const char* table ) {
		axStatus st;
		B::pkeyIndex_	= list.pkeyIndex();
		B::pkeyAutoInc_	= list.pkeyAutoInc();

		axStringA whereStr;
		st = B::getWherePKey( whereStr, list, db );							if( !st ) return st;
		st = B::stmt_.create_Select( db, list, table, whereStr, NULL );		if( !st ) return st;
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
	axStatus	create	( axDBConn & db, const axDBColumnList & list, const char* table, const char* szWhere, const char* szOrder ) {
		axStatus st;
		st = B::stmt_.create_Select( db, list, table, szWhere, szOrder );		if( !st ) return st;
		return 0;
	}
	axStatus	exec_ArgList	( const axDBInParamList &params )	{ return B::stmt_.exec_ArgList( params ); }
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



template<class T, class PKeyType=int64_t, PKeyType T::*PKeyMember=&T::id, bool PKeyAutoInc=true>
class axDBTableAccessor {
public:
	/*
		ToDo
		create :
			table, other stmt, 
		dump : rec, all,
		insert : get last id
	 
	*/
	static
	axStatus createTable( axDBConn & db, const char* table ) {
		axStatus st;
		axDBColumnList	list;
		st = list.create<T, PKeyType, PKeyMember, PKeyAutoInc>();	if( !st ) return st;
		st = db.createTable( list, table );							if( !st ) return st;
		return 0;
	}

	axStatus create( axDBConn & db, const char* table ) {
		axStatus st;
		db_ = &db;
		st = table_.set( table );									if( !st ) return st;

		st = list_.create<T,PKeyType, PKeyMember, PKeyAutoInc>();	if( !st ) return st;

		st = insertStmt.create		( db, list_, table );			if( !st ) return st;
		st = updateStmt.create		( db, list_, table );			if( !st ) return st;
		st = selectStmt.create		( db, list_, table );			if( !st ) return st;

		const axDBColumn * pkey = list_.pkeyColumn();
		if( ! pkey ) {
			st = selectAllStmt.create	( db, list_, table, NULL, NULL );		if( !st ) return st;
		}else{
			axStringA	strOrder;
			axStringA	pkeyColName;

			st = db.identifierString( pkeyColName, pkey->name );				if( !st ) return st;
			st = strOrder.format("{?} ASC", pkeyColName );						if( !st ) return st;

			st = selectAllStmt.create	( db, list_, table, NULL, strOrder );	if( !st ) return st;
		}
		
		return 0;
	}
		
	axStatus insert		(	    T &v )					{ return insertStmt.exec( v );	}
	axStatus update		( const T &v )					{ return updateStmt.exec( v );	}

	axStatus select		( T &v, const PKeyType& pkey )	{ return selectStmt.exec( v, pkey ); }

	axStatus selectAll	( axIArray<T> &rows ) { 
		axStatus st = selectAllStmt.exec();			if( !st ) return st;
		return selectAllStmt.getAllRows( rows ); 
	}

//	axStatus selectWhere			( axIArray<T> &rows, const char* szWhere, params... );
	axStatus selectWhere_ArgList	( axIArray<T> &rows, const char* szWhere, const axDBInParamList & params );
	axExpandArgList2_Imp			( axStatus,  selectWhere, axIArray<T> & /*rows*/, const char* /*szWhere*/, const axDBInParam_CB &, axDBInParamList );

	axDBTableAccessor_InsertStmt<T, PKeyType, PKeyMember, PKeyAutoInc>	insertStmt;
	axDBTableAccessor_UpdateStmt<T, PKeyType, PKeyMember, PKeyAutoInc>	updateStmt;
	axDBTableAccessor_SelectStmt<T, PKeyType, PKeyMember, PKeyAutoInc>	selectStmt;
	axDBTableAccessor_SelectMultipleStmt<T>								selectAllStmt;
	
private:
	axPtr<axDBConn> 	db_;
	axStringA			table_;
	axDBColumnList		list_;
};


template<class T, class PKeyType, PKeyType T::*PKeyMember, bool PKeyAutoInc>
axStatus axDBTableAccessor<T, PKeyType, PKeyMember,PKeyAutoInc>::selectWhere_ArgList ( axIArray<T> &rows, const char* szWhere, const axDBInParamList & params ) {
	axDBTableAccessor_SelectMultipleStmt<T>	stmt;
	axStatus st;
	st = stmt.create( *db_, list_, table_, szWhere );	if( !st ) return st;
	st = stmt.exec_ArgList( params );					if( !st ) return st;
	st = stmt.getAllRows( rows );						if( !st ) return st;
	return 0;
}


#endif //__axDBTableAccessor_h__
