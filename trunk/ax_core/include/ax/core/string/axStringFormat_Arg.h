//
//  axStringFormat_Arg.h
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axStringFormat_Arg_h
#define ax_core_axStringFormat_Arg_h

#include "../data_structure/axArray.h"

class axStringFormat;

#define axTYPE_LIST( T ) \
	axStatus axStringFormat_out( axStringFormat &f, T value ); \
//----
	#include "../common/axTypeList_int.h"
#undef axTYPE_LIST

#define axTYPE_LIST( T ) \
	axStatus axStringFormat_out( axStringFormat &f, T value ); \
//----
	#include "../common/axTypeList_uint.h"
#undef axTYPE_LIST

axStatus axStringFormat_out( axStringFormat &f, float		value );
axStatus axStringFormat_out( axStringFormat &f, double		value );
axStatus axStringFormat_out( axStringFormat &f, bool		value );

axStatus axStringFormat_out( axStringFormat &f, char		value );
axStatus axStringFormat_out( axStringFormat &f, wchar_t		value );

axStatus axStringFormat_out( axStringFormat &f, const char*		value );
axStatus axStringFormat_out( axStringFormat &f, const wchar_t*  value );

inline axStatus axStringFormat_out( axStringFormat &f, axSize	value ) { return axStringFormat_out( f, value.asNative() ); }
inline axStatus axStringFormat_out( axStringFormat &f, char*	value ) { return axStringFormat_out( f, (const char*)   value ); }
inline axStatus axStringFormat_out( axStringFormat &f, wchar_t*	value ) { return axStringFormat_out( f, (const wchar_t*)value ); }

axStatus axStringFormat_out( axStringFormat &f, const void* p );
inline axStatus  axStringFormat_out( axStringFormat &f, void* p ) {
	return axStringFormat_out( f, (const void*) p );
}

template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, const T& value ) { return value.toStringFormat( f ); }
//---------

axStatus axStringFormat_out( axStringFormat &f, axStatus value );

//----------
enum { axStringFormat_ArgListMaxSize = 16 };

//--------
typedef axStatus (*axStringFormat_Func)( axStringFormat &f, const void* v );

template<class T> inline
axStatus axStringFormat_Func_T( axStringFormat &f, const void* v ) {
	return axStringFormat_out( f, *(const T*)v );
}

class axStringFormat_Arg {
public:
	axStringFormat_Arg() { data=NULL; func=NULL; }

	template<class T> 
	axStringFormat_Arg( const T &v )	{ 
		data = &v;
		func = axStringFormat_Func_T<T>;
	}
	axStatus	call( axStringFormat &f ) const {
		return func( f, data );
	}
	
	axStatus	onTake( axStringFormat_Arg &src ) { operator=( src ); return 0; }

	const void*			data;
	axStringFormat_Func	func;
};

class axStringFormat_ArgList : public axArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize > {
	typedef	axArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize >	B;
public:
	axStatus addArg( const axStringFormat_Arg &a ) { 
		return B::append( a );
	}

private:
	axStatus	toStringFormat( axStringFormat &f ) const { assert(false); return -1; } // ArgList.toString is not allowed
};

#endif
