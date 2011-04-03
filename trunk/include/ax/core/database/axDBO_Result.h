#ifndef __axDBO_Result_h__
#define __axDBO_Result_h__

#include "axDBO_Driver.h"


//! \ingroup core_database
//@{
class axDBO_Result {
public:
	axDBO_Result();
	~axDBO_Result();

	axStatus	status	() const;
	axStatus	print	() const;
	axSize		rowCount() const;
	axSize		colCount() const;

	operator	axStatus() const		{ return status(); }

friend class axDBO;
protected:
	axSharedPtr< axDBO_Driver_Result >	p_;
};

//@}

#endif //__axDBO_Result_h__

