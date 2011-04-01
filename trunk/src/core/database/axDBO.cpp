#include <ax/core/database/axDBO.h>
#include "axDBO_pgsql.h"

axDBO::axDBO() {
    p_ = NULL;
}

axDBO::~axDBO() {
    close();
}

axStatus axDBO::connect( const wchar_t* driver, const wchar_t* dsn ) {
	axStatus st;
	close();
	axConstStringW	_driver( driver );

	if( _driver->equals( L"pgsql" ) ) {
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

axStatus	axDBO::execSQL( const wchar_t* sql ) {
	if( ! p_ ) return axStatus::not_initialized;
	return p_->execSQL( sql );
}
