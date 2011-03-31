#ifndef __axDBO_pgsql_h__
#define __axDBO_pgsql_h__

#include "axDBO_Driver.h"

class axDBO_pgsql : public axDBO_Driver {
public:
	axDBO_pgsql();
	virtual ~axDBO_pgsql();

	virtual	axStatus	connect	( const wchar_t* host, const wchar_t* dbname, 
								  const wchar_t* user, const wchar_t* password );
	virtual	void		close	();
	virtual axStatus	execSQL	( const wchar_t* sql );

private:
    void* conn_;
};

#endif //__axDBO_pgsql_h__
