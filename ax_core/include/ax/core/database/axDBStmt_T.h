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

template<class T, class PKey=int64_t, PKey T::*PkeyMember=&T::id>
class axDBStmt_Update {
public:
	axStatus	create	( axDBConn & db, const char* table ) {
		axStatus st;
		axTempStringA	sql;
		st = db.createSQL_Update<T>( sql, table, "id=?" );
		st = stmt_.create( db, sql );	if( !st ) return st;
		return 0;
	}
	axStatus	exec( const T & values ) {
		return stmt_.exec( values, values.id );
	}

	const char*	sql	() { return stmt_.sql(); }

private:
	axDBStmt	stmt_;
};





#endif //__axDBStmt_T_h__
