#ifndef __axDBO_pgsql_h__
#define __axDBO_pgsql_h__

#include <ax/core/database/axDBO.h>
#include "libpq/libpq-fe.h"

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

	virtual	axStatus	getValue( axIStringA& value, axSize row, axSize col ) const {
		value.clear();
		if( !res_) return axStatus::not_initialized;
		return value.set( _getValue(row,col) );
	}
	
	virtual	axStatus	getValue( axIStringW& value, axSize row, axSize col ) const {
		value.clear();
		if( !res_) return axStatus::not_initialized;
		return value.set( _getValue(row,col) );
	}

	virtual	axStatus	getValue( int32_t	  value, axSize row, axSize col ) const {
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

class axDBO_pgsql : public axDBO_Driver {
public:

	axDBO_pgsql();
	virtual ~axDBO_pgsql();

	virtual	axStatus	connect	( const char* dsn );
	virtual	void		close	();
	virtual axStatus	execSQL	( axDBO_Driver_ResultSP &out, const char* sql );

private:
    PGconn* conn_;
};

#endif //__axDBO_pgsql_h__
