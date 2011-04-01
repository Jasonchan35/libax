#ifndef __axDBO_Driver_h__
#define __axDBO_Driver_h__

#include <ax/core/database/axDBO.h>

class axDBO_Driver {
public:
	typedef	axDBO::Result	Result;


	axDBO_Driver() {}
	virtual ~axDBO_Driver() {}

	virtual	axStatus	connect	( const char* dsn ) = 0;
	virtual	void		close	() = 0;
	virtual axStatus	execSQL	( const char* sql, Result *res ) = 0;
};

#endif //__axDBO_Driver_h__

