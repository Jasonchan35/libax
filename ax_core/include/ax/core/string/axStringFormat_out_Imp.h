#pragma once
#ifndef __axStringFormat_out_Imp_h__
#define __axStringFormat_out_Imp_h__

#include "../common/axTypeOf.h"
#include "axStringFormat.h"
#include "ax_str_to.h"

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

//@}

#endif //__axStringFormat_out_Imp_h__


