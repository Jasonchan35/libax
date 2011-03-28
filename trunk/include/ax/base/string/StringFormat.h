#pragma once
#ifndef __ax_StringFormat_h__
#define __ax_StringFormat_h__

#include "../data_structure/LocalArray.h"
#include "ax_string.h"

namespace ax {
//! \addtogroup string
//@{


enum { StringFormat_ArgListMaxSize = 16 };

class	StringFormat;

Status StringFormat_out( StringFormat &f, uint8_t   value );
Status StringFormat_out( StringFormat &f, uint16_t  value );
Status StringFormat_out( StringFormat &f, uint32_t  value );
Status StringFormat_out( StringFormat &f, uint64_t  value );
Status StringFormat_out( StringFormat &f, int8_t	value );
Status StringFormat_out( StringFormat &f, int16_t	value );
Status StringFormat_out( StringFormat &f, int32_t	value );
Status StringFormat_out( StringFormat &f, int64_t	value );

Status StringFormat_out( StringFormat &f, const char*	  value );
Status StringFormat_out( StringFormat &f, const wchar_t*  value );


template<class T> inline
Status StringFormat_out( StringFormat &f, const T& value ) {
	return value.toStringFormat( f );
}

/// @cond ax_internal

class StringFormat_FuncClass {
public:
	Status func( StringFormat &f ) { assert(false); return -1; }
	const void* data_;
};
typedef Status (StringFormat_FuncClass::*StringFormat_Func)( StringFormat &f );

template< class T >
class StringFormat_FuncClass_T : public StringFormat_FuncClass {
public:
	Status func( StringFormat &f ) { return StringFormat_out( f, *data() ); }
	const T* data() { return (const T*)data_; }
};

class StringFormat_Arg {
public:
	StringFormat_Arg() { data_=NULL; callback_=NULL; }

	template<class T> 
	StringFormat_Arg( const T &v )	{ 
		data_ = &v;
		callback_ = static_cast< StringFormat_Func > ( &StringFormat_FuncClass_T<T>::func );
	}

	Status	call( StringFormat &f ) const {
		StringFormat_FuncClass	wrapper;
		wrapper.data_ = data_;
		return (wrapper.*callback_)( f );
	}

	Status	takeOwnership( StringFormat_Arg &src ) { operator=( src ); return 0; }
private:
	const void*			data_;
	StringFormat_Func	callback_;
};

class StringFormat_ArgList : public LocalArray< StringFormat_Arg, StringFormat_ArgListMaxSize > {
	typedef	LocalArray< StringFormat_Arg, StringFormat_ArgListMaxSize >	B;
public:
	StringFormat_ArgList&	operator<<( const StringFormat_Arg &a ) { B::append( a ); return *this; }
};

/// @endcond



//@}
} //namespace ax

#endif //__ax_StringFormat_h__