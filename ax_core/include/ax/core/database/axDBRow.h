#ifndef __axDBRow_h__
#define __axDBRow_h__

#include "axDBStmt.h"


template<class T>
class axDBRow_Insert {
public:
	axStatus	create	( const char* table, const char* pkey="id", bool auto );
	axStatus	exec	( T & value );

private:
	axDBStmt	stmt_;
};


template<class T>
class axDBRow_Update {
public:
	axStatus	create	( const char* table, const char* pkey );
	axStatus	exec	( T & value );

private:
	axDBStmt	stmt_;
};



#endif //__axDBRow_h__
