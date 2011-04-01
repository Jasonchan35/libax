#include <ax/core/database/axDBO.h>
#include "axDBO_pgsql.h"

axDBO::axDBO() {
    p_ = NULL;
}

axDBO::~axDBO() {
    close();
}

axStatus axDBO::connect( const char* driver, const char* dsn ) {
	axStatus st;
	close();

	if(  ax_strcmp( driver, "pgsql" ) == 0 ) {
		p_ = new axDBO_pgsql;
		if( !p_ ) return axStatus::not_enough_memory;
	}else{
        return axStatus::not_found;
	}

    return p_->connect( dsn );
}

void axDBO::close() {
	if( p_ ) {
		p_->close();
		delete p_;
		p_ = NULL;
	}
}

axStatus	axDBO::execSQL( const char* sql ) {
	if( ! p_ ) return axStatus::not_initialized;
	return p_->execSQL( sql );
}
