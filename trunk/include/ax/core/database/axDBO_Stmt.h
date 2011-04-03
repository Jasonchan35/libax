#ifndef __axDBO_Stmt_h__
#define __axDBO_Stmt_h__

#include "axDBO_Driver.h"

//! \ingroup core_database
//@{

class axDBO_Stmt {
public:

	axStatus	exec();

friend class axDBO;
protected:
	axSharedPtr< axDBO_Driver_Stmt >	p_;
};

//@}

#endif //__axDBO_Stmt_h__
