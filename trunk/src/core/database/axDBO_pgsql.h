#ifndef __axDBO_pgsql_h__
#define __axDBO_pgsql_h__

#include <ax/core/database/axDBO.h>
#include "libpq/libpq-fe.h"


//from "libpq/server/pg_type.h"
enum PGType {
	BOOLOID		= 16,		// boolean
	BYTEA		= 17,			// bytea
	NAMEOID		= 19,		// name
	INT8OID		= 20,		// bigint
	INT2OID		= 21,		// smallint
	INT4OID		= 23,		// integer
	TEXTOID		= 25,		// character varying without length
	OIDOID		= 26,		// oid
	FLOAT4OID	= 700,	// real
	FLOAT8OID	= 701,	// double precision
	BPCHAROID	= 1042,	// character ( fix length )
	VARCHAROID	= 1043,	// character varying
	TIMESTAMPOID = 1114, // timestamp without time zone
};


class axDBO_pgsql;

class axDBO_pgsql_Result : public axDBO_Driver_Result {
public:
	axDBO_pgsql_Result() { res_ = NULL; }
	virtual	~axDBO_pgsql_Result() { 
		if( res_ ) {
			PQclear( res_ );
			res_ = NULL;
		}
	}

	virtual	axStatus	getValue( axIStringA & value, axSize row, axSize col ) const {
		value.clear();
		if( !res_) return axStatus::not_initialized;
		return value.set( _getValue(row,col) );
	}
	
	virtual	axStatus	getValue( axIStringW & value, axSize row, axSize col ) const {
		value.clear();
		if( !res_) return axStatus::not_initialized;
		return value.set( _getValue(row,col) );
	}

	virtual	axStatus	getValue( int32_t &	value, axSize row, axSize col ) const {
		if( !res_) return axStatus::not_initialized;
		return str_to( _getValue(row,col), value );
	}

friend class axDBO_pgsql;
protected:
	const char*	_getValue( axSize row, axSize col ) const {
		if( row >= axTypeOf<int>::max() ) return NULL;
		if( col >= axTypeOf<int>::max() ) return NULL;
		int r = (int)row;
		int c = (int)col;
		return PQgetvalue( res_, r, c );
	}
	PGresult*	res_;
};


class axDBO_pgsql_Stmt : public axDBO_Driver_Stmt { 
public:

};


class axDBO_pgsql : public axDBO_Driver {
public:

	axDBO_pgsql();
	virtual ~axDBO_pgsql();

	virtual	axStatus	connect		( const char* dsn );
	virtual	void		close		();
	virtual axStatus	execSQL		( axDBO_Driver_ResultSP &out, const char* sql );
	virtual axStatus	prepareStmt_ParamList ( axDBO_Driver_StmtSP	&out, const char* sql, const axDBO_ParamList &list );

private:
    PGconn* conn_;
};

#endif //__axDBO_pgsql_h__
