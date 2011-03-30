#ifndef __axExternalString_h__
#define __axExternalString_h__

#include "axIString.h"


template< class T >
class axExternalString_ : public axIString<T> {
	typedef	axIString<T>	B;
public:
	axExternalString_ ( T* sz = NULL )	: axIString<T>(buf_) { if( sz ) setExternal( sz ); } 
	void setExternal( T* sz )			{ buf_.setExternal( sz, ax_strlen(sz) ); }
private:
	ExternalArray<T>	buf_;
};

typedef	axExternalString_<char>		ExternalStringA;
typedef	axExternalString_<wchar_t>	ExternalStringW;


#endif //__axExternalString_h__
