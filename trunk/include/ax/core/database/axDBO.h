#ifndef __axDBO_h__
#define __axDBO_h__

//! \ingroup core_database
//@{

#include "axDBO_Driver.h"

class axDBO : public axNonCopyable {
public:
	axStatus	connect( const wchar_t* driver, 
						 const wchar_t* host, const wchar_t* dbname, 
						 const wchar_t* user, const wchar_t* passowrd );
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
	void*	p_;
};




//@}

#endif //__axDBO_h__
