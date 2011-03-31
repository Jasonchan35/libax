#ifndef __axDBO_h__
#define __axDBO_h__

//! \ingroup core_database
//@{

#include "axDBO_Driver.h"

class axDBO : public axNonCopyable {
public:
	axStatus	connect( const char* driver, 
						 const char* host, const char* dbname, 
						 const char* user, const char* passowrd );
	void		close();

private:
	axDBO_Driver*	dri_;
};




//@}

#endif //__axDBO_h__
