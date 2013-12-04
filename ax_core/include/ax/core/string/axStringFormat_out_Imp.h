#pragma once
#ifndef __axStringFormat_out_Imp_h__
#define __axStringFormat_out_Imp_h__

#include "../common/axTypeTrait.h"
#include "axStringFormat.h"
#include "ax_str_to.h"
#include "../data_structure/axPtr.h"

//! \ingroup base_string
//@{

inline axStatus	axStringFormat_out( axStringFormat &f, axStatus value ) { return f.format("({?}:{?})", value.code(), value.c_str() ); }

axStatus axStringFormat_out( axStringFormat &f, const void* p );

axStatus axStringFormat_out( axStringFormat &f, const char*		value );
axStatus axStringFormat_out( axStringFormat &f, const wchar_t*	value );

axStatus axStringFormat_out( axStringFormat &f, wchar_t	value );
axStatus axStringFormat_out( axStringFormat &f, char	value );

#define	axTYPE_LIST(T)	\
	axStatus axStringFormat_out( axStringFormat &f, T   value );
//---
	#include "../common/axTypeList_int.h"
	#include "../common/axTypeList_uint.h"
	#include "../common/axTypeList_float.h"	
#undef axTYPE_LIST

axStatus axStringFormat_out( axStringFormat &f, bool value );

template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, const axPtr<T> &p ) {
	if( !p ) return f.out("null");
	return f.out( *p );
}


//@}

#endif //__axStringFormat_out_Imp_h__


