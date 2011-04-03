#include <ax/core/database/axDBO.h>
#include "axDBO_pgsql.h"


axDBO::axDBO() {
}

axDBO::~axDBO() {
    close();
}

axStatus axDBO::connect( const char* driver, const char* dsn ) {
	axStatus st;
	close();

	if(  ax_strcmp( driver, "pgsql" ) == 0 ) {
		p_.ref( new axDBO_pgsql );
		if( !p_ ) return axStatus::not_enough_memory;
	}else{
        return axStatus::not_found;
	}

    return p_->connect( dsn );
}

void axDBO::close() {
	p_.unref();
}

axDBO_Result axDBO::execSQL_ParamList( const char* sql, const axDBO_ParamList &list ) {
	axDBO_Result	res;
	if( !p_ ) return res;

	p_->execSQL_ParamList( res.p_, sql, list );
	return res;
}

axDBO_Stmt  axDBO::prepareSQL_ParamList( const char* sql, const axDBO_ParamList &list ) {
	axDBO_Stmt	stmt;
	if( !p_ ) return stmt;
	p_->prepareSQL_ParamList( stmt.p_, sql, list );
	return stmt;
}

axDBO_Result::axDBO_Result() {
}

axDBO_Result::~axDBO_Result() {
}

axStatus axDBO_Result :: status () const { 
	return (p_) ? p_->status() : axStatus::not_initialized; 
}

axStatus axDBO_Result :: print() const {
	if( !p_ ) return axStatus::not_initialized;

	axStatus st;
	axSize r, c;
	axSize nr = rowCount();
	axSize nc = colCount();

	axStringA_<256>	value;

	for( r=0; r<nr; r++ ) {
		for( c=0; c<nc; c++ ) {
			st = p_->getValue( value, r, c );	if( !st ) return st;
			ax_print("{?},", value );
		}
		ax_print("\n");
	}
	return 0;
}

axSize axDBO_Result :: rowCount() const {
	return p_ ? p_->rowCount() : 0;
}

axSize axDBO_Result :: colCount() const {
	return p_ ? p_->colCount() : 0;
}
