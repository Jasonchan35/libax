#ifndef __axDBO_h__
#define __axDBO_h__

#include "../../ax_base.h"

//! \ingroup core_database
//@{

//! Database Object
/*!

*/
class axDBO : public axNonCopyable {
public:
	class Result;

    axDBO();
    ~axDBO();

	axStatus	connect	( const char* driver, const char* dsn );
	void		close	();
	axStatus	execSQL	( const char* sql, Result* res = NULL );

private:
	void*	p__;
};

class axDBO::Result : public axNonCopyable {
public:
protected:
	axDBO	dbo_;
	void*	p_;
};


//@}

#endif //__axDBO_h__
