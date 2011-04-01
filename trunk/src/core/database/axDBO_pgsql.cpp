#include "axDBO_pgsql.h"

axDBO_pgsql::axDBO_pgsql() {
	conn_ = NULL;
}

axDBO_pgsql::~axDBO_pgsql() {
	close();
}

axStatus axDBO_pgsql::connect( const char* dsn ){
    axStatus st;

    conn_ = PQconnectdb( dsn );
    if( PQstatus( conn_ ) != CONNECTION_OK ) {
		close();
		return -1;
    }
	return 0;
}

void axDBO_pgsql::close() {
	if( conn_ ) {
		PQfinish( conn_ );
		conn_ = NULL;
	}
}

axStatus axDBO_pgsql::execSQL	( const char* sql ) {
	axStatus st;
	if( ! conn_ ) return axStatus::not_initialized;
	
	PGresult* res = PQexec( conn_, sql );

	switch( PQresultStatus(res) ) {
		case PGRES_TUPLES_OK:	ax_print(L"PGRES_TUPLES_OK\n");		break;
		case PGRES_COMMAND_OK:	ax_print(L"PGRES_COMMAND_OK\n");	break;
		default: {
			fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn_) );
			PQclear(res);
			return -1;
		}break;
	}

	int row_count = PQntuples( res );
	int col_count = PQnfields( res );

	ax_print( "=== column === \n" );
	for( int c=0; c < col_count; c++ ) {
		ax_print( "{?} ", PQfname( res,c ) );
	}

	ax_print( L"\n--- rows --- \n" );
	for( int r=0; r<row_count; r++ ) {
		for( int c=0; c < col_count; c++ ) {
			ax_print( L"{?} ", PQgetvalue( res, r, c ) );
		}
		ax_print(L"\n");
	}

	return 0;
}
