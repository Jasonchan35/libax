#pragma once
#ifndef __axStringFormat_h__
#define __axStringFormat_h__

#include "ax_c_str.h"
#include "../data_structure/axLocalArray.h"

//! \ingroup base_string
//@{


enum { axStringFormat_ArgListMaxSize = 16 };

class	axStringFormat;

axStatus axStringFormat_out( axStringFormat &f, uint8_t		value );
axStatus axStringFormat_out( axStringFormat &f, uint16_t	value );
axStatus axStringFormat_out( axStringFormat &f, uint32_t	value );
axStatus axStringFormat_out( axStringFormat &f, uint64_t	value );
axStatus axStringFormat_out( axStringFormat &f, int8_t		value );
axStatus axStringFormat_out( axStringFormat &f, int16_t		value );
axStatus axStringFormat_out( axStringFormat &f, int32_t		value );
axStatus axStringFormat_out( axStringFormat &f, int64_t		value );
axStatus axStringFormat_out( axStringFormat &f, float		value );
axStatus axStringFormat_out( axStringFormat &f, double		value );
axStatus axStringFormat_out( axStringFormat &f, bool		value );

axStatus axStringFormat_out( axStringFormat &f, const char*		value );
axStatus axStringFormat_out( axStringFormat &f, const wchar_t*  value );

inline
axStatus axStringFormat_out( axStringFormat &f, char*	  value ) { return axStringFormat_out( f, (const char*)value ); }
inline
axStatus axStringFormat_out( axStringFormat &f, wchar_t*  value ) { return axStringFormat_out( f, (const char*)value ); }


axStatus axStringFormat_out( axStringFormat &f, const void* p );

template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, T*	  value ) { return axStringFormat_out( f, (const void* &)value ); }


template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, const T& value ) { return value.toStringFormat( f ); }

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
	const void*	data() const { return data_; }
private:
	const void*			data_;
	StringFormat_Func	callback_;
};

class axStringFormat_ArgList : public axLocalArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize > {
	typedef	axLocalArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize >	B;
public:
	axStringFormat_ArgList&	operator<<( const axStringFormat_Arg &a ) { 
		axStatus st;
		st = B::append( a );	assert( st );
		return *this; 
	}
};

//@}

#endif //__ax_StringFormat_h__

