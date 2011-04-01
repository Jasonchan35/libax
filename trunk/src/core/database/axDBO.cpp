#include "axDBO_Driver.h"
#include "axDBO_pgsql.h"

#define	DEFINE_WRAPPER   axDBO_Driver* & p_ = (axDBO_Driver* &)(p__);

axDBO::axDBO() {
    p__ = NULL;
}

axDBO::~axDBO() {
    close();
}

axStatus axDBO::connect( const char* driver, const char* dsn ) {
	DEFINE_WRAPPER

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
	DEFINE_WRAPPER
	if( p_ ) {
		p_->close();
		delete p_;
		p_ = NULL;
	}
}

axStatus	axDBO::execSQL( const char* sql, Result *res ) {
	DEFINE_WRAPPER
	if( ! p_ ) return axStatus::not_initialized;
	return p_->execSQL( sql, res );
}
