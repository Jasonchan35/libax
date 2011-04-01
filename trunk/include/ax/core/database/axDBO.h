#ifndef __axDBO_h__
#define __axDBO_h__

//! \ingroup core_database
//@{

#include "axDBO_Driver.h"

//! Database Object
/*!

*/
class axDBO : public axNonCopyable {
public:

    axDBO();
    ~axDBO();

	axStatus	connect( const char* driver, const char* dsn );
	void		close();
	axStatus	execSQL(const char* sql );

private:
	axDBO_Driver*	p_;
};




//@}

#endif //__axDBO_h__
