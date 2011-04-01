#ifndef __axDBO_h__
#define __axDBO_h__

#include "axDBO_Driver.h"

//! \ingroup core_database
//@{

//! Database Object
/*!

*/

class axDBO;
class axDBO_Driver_Result;

class axDBO_Result {
public:
	axDBO_Result();
	~axDBO_Result();

	operator	axStatus () const;

	axStatus	print	() const;
	axSize		rowCount() const;
	axSize		colCount() const;

friend class axDBO;
protected:
	axSharedPtr< axDBO_Driver_Result >	p_;
};

class axDBO : public axNonCopyable {
public:
    axDBO();
    ~axDBO();

	axStatus		connect	( const char* driver, const char* dsn );
	void			close	();
	axDBO_Result	execSQL	( const char* sql );

private:
	axSharedPtr< axDBO_Driver >	p_;
};


//@}

#endif //__axDBO_h__
