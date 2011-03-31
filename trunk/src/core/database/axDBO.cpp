#include <ax/core/database/axDBO.h>
#include <ax/core/database/axDBO_pgsql.h>


axStatus axDBO::connect( const wchar_t* driver, 
						 const wchar_t* host, const wchar_t* dbname, 
						 const wchar_t* user, const wchar_t* passowrd ) 
{
	axStatus st;
	close();
	axConstStringW	_driver( driver );

	if( _driver->equals( L"pgsql" ) ) {
		dri_ = new axDBO_pgsql;
		return 0;
	}

	return axStatus::not_found;
}

void axDBO::close() {
	if( dri_ ) {
		dri_->close();
		delete dri_;
		dri_ = NULL;
	}
}

axStatus	axDBO::execSQL( const wchar_t* sql ) {
	if( ! dri_ ) return axStatus::not_initialized;
	return dri_->execSQL( sql );
}
