#ifndef __axExternalString_h__
#define __axExternalString_h__

#include "axIString.h"
#include "../data_structure/axExternalArray.h"

//! \ingroup base_string
//@{


template< class T >
class axExternalString_ : public axIString_<T> {
	typedef	axIString_<T>	B;
public:
	axExternalString_( T* sz=NULL )	: axIString_<T>(buf_)	{ if( sz ) setExternal( sz ); } 
	void setExternal( T* sz )								{ buf_.setExternal( sz, ax_strlen(sz) ); }

private:
	axExternalArray<T>	buf_;
};

typedef	axExternalString_<char>		axExternalStringA;
typedef	axExternalString_<wchar_t>	axExternalStringW;
typedef	axExternalString_<axUChar>	axExternalStringU;

//@}

#endif //__axExternalString_h__
