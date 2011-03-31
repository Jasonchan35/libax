#pragma once
#ifndef __axStringFormat_h__
#define __axStringFormat_h__

#include "../data_structure/axLocalArray.h"
#include "ax_string.h"

//! \ingroup base_string
//@{


enum { axStringFormat_ArgListMaxSize = 16 };

class	axStringFormat;

axStatus axStringFormat_out( axStringFormat &f, uint8_t   value );
axStatus axStringFormat_out( axStringFormat &f, uint16_t  value );
axStatus axStringFormat_out( axStringFormat &f, uint32_t  value );
axStatus axStringFormat_out( axStringFormat &f, uint64_t  value );
axStatus axStringFormat_out( axStringFormat &f, int8_t	value );
axStatus axStringFormat_out( axStringFormat &f, int16_t	value );
axStatus axStringFormat_out( axStringFormat &f, int32_t	value );
axStatus axStringFormat_out( axStringFormat &f, int64_t	value );

axStatus axStringFormat_out( axStringFormat &f, const char*	  value );
axStatus axStringFormat_out( axStringFormat &f, const wchar_t*  value );


template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, const T& value ) {
	return value.toStringFormat( f );
}

/// @cond ax_internal

class axStringFormat_FuncClass {
public:
	axStatus func( axStringFormat &f ) { assert(false); return -1; }
	const void* data_;
};
typedef axStatus (axStringFormat_FuncClass::*StringFormat_Func)( axStringFormat &f );

template< class T >
class axStringFormat_FuncClass_T : public axStringFormat_FuncClass {
public:
	axStatus func( axStringFormat &f ) { return axStringFormat_out( f, *data() ); }
	const T* data() { return (const T*)data_; }
};

class axStringFormat_Arg {
public:
	axStringFormat_Arg() { data_=NULL; callback_=NULL; }

	template<class T> 
	axStringFormat_Arg( const T &v )	{ 
		data_ = &v;
		callback_ = static_cast< StringFormat_Func > ( &axStringFormat_FuncClass_T<T>::func );
	}

	axStatus	call( axStringFormat &f ) const {
		axStringFormat_FuncClass	wrapper;
		wrapper.data_ = data_;
		return (wrapper.*callback_)( f );
	}

	axStatus	takeOwnership( axStringFormat_Arg &src ) { operator=( src ); return 0; }
private:
	const void*			data_;
	StringFormat_Func	callback_;
};

class axStringFormat_ArgList : public axLocalArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize > {
	typedef	axLocalArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize >	B;
public:
	axStringFormat_ArgList&	operator<<( const axStringFormat_Arg &a ) { B::append( a ); return *this; }
};

/// @endcond

//@}

#endif //__ax_StringFormat_h__

