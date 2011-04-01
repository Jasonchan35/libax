#ifndef __axDBO_pgsql_h__
#define __axDBO_pgsql_h__

#include "axDBO_Driver.h"
#include "libpq/libpq-fe.h"

class axDBO_pgsql : public axDBO_Driver {
public:

	axDBO_pgsql();
	virtual ~axDBO_pgsql();

	virtual	axStatus	connect	( const char* dsn );
	virtual	void		close	();
	virtual axStatus	execSQL	( const char* sql );

private:
    PGconn* conn_;
};

#endif //__axDBO_pgsql_h__
