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

	void setExternal( const T* sz )	{ ax_const_cast(v_).setExternal( (T*)sz ); }

	operator const axIString_<T> & () const				{ return v_; }
			 const axIString_<T> & asIString () const	{ return v_; }

	axStatus		toStringFormat( axStringFormat &f ) const { return v_.toStringFormat(f); }

private:
	const axExternalString_<T> v_;
};

typedef axConstString_<char>	axConstStringA;
typedef axConstString_<wchar_t>	axConstStringW;
typedef axConstString_<axUChar>	axConstStringU;

//@}

#endif //__axConstString_h__
