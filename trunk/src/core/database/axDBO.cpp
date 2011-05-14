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

axStatus axDBO::execSQL_ParamList( axDBO_DataSet &data, const char* sql, const axDBO_ParamList &list ) {
	if( !p_ ) return -1;
	return p_->execSQL_ParamList( data.p_, sql, list );
}

axStatus axDBO::prepareSQL_ParamList( axDBO_Stmt &stmt, const char* sql, const axDBO_ParamList &list ) {
	if( !p_ ) return -1;
	return p_->prepareSQL_ParamList( stmt.p_, sql, list );
}
