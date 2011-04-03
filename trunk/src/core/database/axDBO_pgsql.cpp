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

axStatus	axDBO_pgsql_Result::status() const {
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

axSize	axDBO_pgsql_Result::rowCount() const {
	if( ! res_ ) return 0;
	return PQntuples( res_ );
}

axSize	axDBO_pgsql_Result::colCount() const {
	if( ! res_ ) return 0;
	return PQnfields( res_ );
}

axStatus	axDBO_pgsql_Result::getValue( axIStringA & value, axSize row, axSize col ) const {
	value.clear();
	if( !res_) return axStatus::not_initialized;
	return value.set( _getValue(row,col) );
}

axStatus	axDBO_pgsql_Result::getValue( axIStringW & value, axSize row, axSize col ) const {
	value.clear();
	if( !res_) return axStatus::not_initialized;
	return value.set( _getValue(row,col) );
}

axStatus	axDBO_pgsql_Result::getValue( int16_t &	value, axSize row, axSize col ) const {
	if( !res_) return axStatus::not_initialized;
	return str_to( _getValue(row,col), value );
}

axStatus	axDBO_pgsql_Result::getValue( int32_t &	value, axSize row, axSize col ) const {
	if( !res_) return axStatus::not_initialized;
	return str_to( _getValue(row,col), value );
}

axStatus	axDBO_pgsql_Result::getValue( int64_t &	value, axSize row, axSize col ) const {
	if( !res_) return axStatus::not_initialized;
	return str_to( _getValue(row,col), value );
}

axStatus	axDBO_pgsql::execSQL_ParamList ( axDBO_Driver_ResultSP &out, const char* sql, const axDBO_ParamList &list ) {
	if( ! conn_ ) return axStatus::not_initialized;
	axStatus st;

	axDBO_pgsql_Result* res = new axDBO_pgsql_Result;
	if( ! res ) return axStatus::not_enough_memory;
	out.ref( res );
	res->dbo_ = this;

	if( list.size() == 0 ) {
		*res = PQexec( conn_, sql );
	}else{

		axSize i;
		for( i=0; i<list.size(); i++ ) {
			const axDBO_Param &p = list[i];
			switch( p.type() ) {
				case axDBO_c_type_int32: {
					_param_values [i].int32_ = ax_host_to_be( *(int*) p.data() );
					_param_pvalues[i] = (const char*)&_param_values[i];
					_param_types  [i] = INT4OID;
					_param_lengths[i] = sizeof( int32_t );
					_param_formats[i] = BINARY_FORMAT;
				}break;
				default: {
					assert( false );
					return -100;
				}break;
			}
		}

		*res = PQexecParams( conn_, sql, (int)list.size(), 
							 _param_types, _param_pvalues, _param_lengths, _param_formats, BINARY_FORMAT );
	}

	st = res->status();		if( !st ) return st; 

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
	st = res.status();		if( !st ) return st;

	res = PQdescribePrepared ( conn_, stmt_name );
	st = res.status();		if( !st ) return st;

	axSize n_param = (axSize) PQnparams( res );

	st = stmt->paramList_.copy( list );		if( !st ) return st;
/*
	axSize i;
	for( i=0; i<n_param; i++ ) {
		Oid t = PQparamtype ( res, (int)i );
		ax_print( "param type {?}\n", t );

		st = stmt->bindParam( list[i] );
		if( !st ) return st;
	}
*/
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

axStatus axDBO_pgsql_Stmt::exec() {
	if( ! stmtName_ ) return axStatus::not_initialized;
	if( ! dbo_ ) return axStatus::not_initialized;
	if( ! dbo_->conn_ ) return axStatus::not_initialized;

	axDBO_pgsql_Result	res;
//	res = PQexecPrepared( dbo_->conn_, stmtName_, paramList_.size(), 
//							paramValues_, paramLengths_, paramFormats_, 1 );
	return 0;
}
