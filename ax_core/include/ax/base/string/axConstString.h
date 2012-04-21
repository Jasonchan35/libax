#ifndef __axConstString_h__
#define __axConstString_h__

#include "axExternalString.h"

//! \ingroup base_string
//@{

//! Cosnt String Wrapper
/*!
*/
template< class T >
class axConstString_ {
public:
	axConstString_  ( const T* sz = NULL ) { setExternal(sz); }
	axConstString_  ( const axConstString_<T> &src ) { setExternal(src); }

	void setExternal( const T* sz )	{ 
		( const_cast< axExternalString_<T>&>(v_) ).setExternal( (T*)sz ); 
	}

	const axExternalString_<T>* operator->() const { return &v_; }
	const axExternalString_<T>& operator* () const { return  v_; }

	operator const T*		() const	{ return v_.c_str(); }

	axStatus		toStringFormat( axStringFormat &f ) const { return v_.toStringFormat(f); }

private:
	const axExternalString_<T> v_;
};

typedef axConstString_<char>	axConstStringA;
typedef axConstString_<wchar_t>	axConstStringW;
typedef axConstString_<axUChar>	axConstStringU;

//@}

#endif //__axConstString_h__
