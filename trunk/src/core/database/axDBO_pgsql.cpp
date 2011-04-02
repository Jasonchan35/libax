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

axStatus	axDBO_pgsql_Result::pgStatus() const {
	if( ! res_ ) return axStatus::not_initialized;
	ExecStatusType e = PQresultStatus( res_ );
	switch( e ) {
		case PGRES_TUPLES_OK:	return 0;
		case PGRES_COMMAND_OK:	return 0;
	}

#ifdef _DEBUG
	ax_print( "psql Error: {?}\n", PQresultErrorMessage( res_ ) );
#endif
	return -1;
}

axStatus	axDBO_pgsql::execSQL ( axDBO_Driver_ResultSP &out, const char* sql ) {
	if( ! conn_ ) return axStatus::not_initialized;
	axStatus st;

	axDBO_pgsql_Result* res = new axDBO_pgsql_Result;
	if( ! res ) return axStatus::not_enough_memory;
	out.ref( res );
	res->dbo_ = this;

	*res = PQexec( conn_, sql );
	st = res->pgStatus();		if( !st ) return st; 

	res->rowCount_ = PQntuples( *res );
	res->colCount_ = PQnfields( *res );

	/*
	int row_count = PQntuples( _res );
	int col_count = PQnfields( _res );

	ax_print( "=== column === \n" );
	for( int c=0; c < col_count; c++ ) {
		ax_print( "{?} ", PQfname( _res,c ) );
	}

	ax_print( L"\n--- rows --- \n" );
	for( int r=0; r<row_count; r++ ) {
		for( int c=0; c < col_count; c++ ) {
			ax_print( L"{?} ", PQgetvalue( _res, r, c ) );
		}
		ax_print(L"\n");
	}
*/
	return 0;
}


//virtual 
axStatus axDBO_pgsql::prepareSQL_ParamList ( axDBO_Driver_StmtSP &out, const char* sql, const axDBO_ParamList &list ) {
	if( ! conn_ ) return axStatus::not_initialized;
	axStatus st;

	axDBO_pgsql_Stmt* stmt = new axDBO_pgsql_Stmt;
	if( ! stmt ) return axStatus::not_enough_memory;
	out.ref( stmt );

	axStringA_<64>	stmt_name;

	//using pointer as stmt name
	stmt->dbo_.ref( this );
	st = stmt_name.format( "{?}", stmt );		if( !st ) return st;

	axDBO_pgsql_Result res;


	res = PQprepare( conn_, stmt_name, sql, (int)list.size(), NULL );
	st = res.pgStatus();		if( !st ) return st;

	res = PQdescribePrepared ( conn_, stmt_name );
	st = res.pgStatus();		if( !st ) return st;

	axSize n_param = (axSize) PQnparams( res );

	st = stmt->paramList_.resize( n_param );		if( !st ) return st;

	axSize i;
	for( i=0; i<n_param; i++ ) {
		axDBO_pgsql_Stmt::Param	&pa = stmt->paramList_[i];
		pa.type_ = PQparamtype ( res, (int)i );

		ax_print( "param type {?}\n", pa.type_ );
	}

//	PQexecPrepared( conn_, name, nParam, &val

	st = stmt->stmtName_.set( stmt_name );		if( !st ) return st;
	return 0;
}


void axDBO_pgsql_Stmt::release() {
	axStatus st;
	axStringA_<128> sql;

	if( ! stmtName_ ) return;

	st = sql.format( "DEALLOCATE \"{?}\" ", stmtName_ );
	stmtName_.clear();

	if( ! dbo_ ) { assert(false); return; }
	if( ! dbo_->conn_ ) { assert(false); return; }

	if( !st ) { assert(false); return; }

	PGresult *res = PQexec( dbo_->conn_, sql );
	if ( !res ) { assert( false ); return; }

	ExecStatusType s = PQresultStatus( res );
//	if( s != PGRES_TUPLES_OK  ) assert( false );
}