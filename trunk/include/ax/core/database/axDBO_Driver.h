#ifndef __axDBO_Driver_h__
#define __axDBO_Driver_h__

#include "../../ax_base.h"

class axDBO_Driver {
public:
	axDBO_Driver() {}
	virtual ~axDBO_Driver() {}

	virtual	axStatus	connect	( const wchar_t* dsn ) = 0;
	virtual	void		close	() = 0;
	virtual axStatus	execSQL	( const wchar_t* sql ) = 0;
};

#endif //__axDBO_Driver_h__

