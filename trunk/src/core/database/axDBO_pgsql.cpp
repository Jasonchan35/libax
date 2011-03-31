#include <ax/core/database/axDBO_pgsql.h>
#include "libpq/libpq-fe.h"

axDBO_pgsql::axDBO_pgsql() {
	conn_ = NULL;
}

axDBO_pgsql::~axDBO_pgsql() {
	close();
}

axStatus axDBO_pgsql::connect( const wchar_t *host, const wchar_t *dbname, const wchar_t *user, const wchar_t *password ){
	axStringA_<1024>	dsn;
	dsn.format(L"host={?} dbname={?} user={?} password={?}", host, dbname, user, password );

    conn_ = PQconnectdb( dsn );
    if( PQstatus( (PGconn*)conn_ ) != CONNECTION_OK ) {
		close();
		return -1;
    }
	return 0;
}

void axDBO_pgsql::close() {
	if( conn_ ) {
		PQfinish( (PGconn*)conn_ ); 
	}
}