#ifndef __ax_ExternalString_h__
#define __ax_ExternalString_h__

#include "xString.h"

namespace ax {

template< class T >
class ExternalString_ : public xString<T> {
	typedef	xString<T>	B;
public:
	ExternalString_ ( T* sz = NULL )	: xString<T>(buf_) { if( sz ) setExternal( sz ); } 
	void setExternal( T* sz )			{ buf_.setExternal( sz, ax_strlen(sz) ); }
private:
	ExternalArray<T>	buf_;
};

typedef	ExternalString_<char>		ExternalStringA;
typedef	ExternalString_<wchar_t>	ExternalStringW;



} //namespace ax 

#endif //__ax_ExternalString_h__
