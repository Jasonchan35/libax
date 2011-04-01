#ifndef __axDBO_h__
#define __axDBO_h__

//! \ingroup core_database
//@{

#include "axDBO_Driver.h"

class axDBO : public axNonCopyable {
public:
    axDBO();
    ~axDBO();

	axStatus	connect( const wchar_t* driver, const wchar_t* dsn );
	void		close();
	axStatus	execSQL( const wchar_t* sql );

	class Stmt {
	public:
	private:
		void*	p_;
	};

	class Result {
	public:
	private:
		void*	p_;
	};

private:
	axDBO_Driver*	p_;
};




//@}

#endif //__axDBO_h__
