#include <ax/core/database/axDBO_pgsql.h>
#include "libpq/libpq-fe.h"

axDBO_pgsql::axDBO_pgsql() {
	conn_ = NULL;
}

axDBO_pgsql::~axDBO_pgsql() {
	close();
}

axStatus axDBO_pgsql::connect( const wchar_t* dsn ){
    axStatus st;
    axStringA_<1024>  _dsn;
    st = _dsn.convert( dsn );       if( !st ) return st;

    conn_ = PQconnectdb( _dsn );
    if( PQstatus( (PGconn*)conn_ ) != CONNECTION_OK ) {
		close();
		return -1;
    }
	return 0;
}

void axDBO_pgsql::close() {
	if( conn_ ) {
		PQfinish( (PGconn*)conn_ );
		conn_ = NULL;
	}
}

axStatus axDBO_pgsql::execSQL	( const wchar_t* sql ) {
	if( ! conn_ ) return axStatus::not_initialized;
	return 0;
}
