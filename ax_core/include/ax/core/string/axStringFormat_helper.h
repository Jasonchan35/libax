#pragma once
#ifndef __axStringFormat_h__
#define __axStringFormat_h__

#include "ax_c_str.h"
#include "../data_structure/axLocalArray.h"

//! \ingroup base_string
//@{


#include "ax_str_to.h"
#include "axString.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axChunkArray.h"

axStatus axStringFormat_out( axStringFormat &f, uint8_t		value );
axStatus axStringFormat_out( axStringFormat &f, uint16_t	value );
axStatus axStringFormat_out( axStringFormat &f, uint32_t	value );
axStatus axStringFormat_out( axStringFormat &f, uint64_t	value );
axStatus axStringFormat_out( axStringFormat &f, int8_t		value );
axStatus axStringFormat_out( axStringFormat &f, int16_t		value );
axStatus axStringFormat_out( axStringFormat &f, int32_t		value );
axStatus axStringFormat_out( axStringFormat &f, int64_t		value );

axStatus axStringFormat_out( axStringFormat &f, unsigned long value );
axStatus axStringFormat_out( axStringFormat &f, long value );
axStatus axStringFormat_out( axStringFormat &f, unsigned long long value );
axStatus axStringFormat_out( axStringFormat &f, long long value );


axStatus axStringFormat_out( axStringFormat &f, float		value );
axStatus axStringFormat_out( axStringFormat &f, double		value );
axStatus axStringFormat_out( axStringFormat &f, bool		value );

axStatus axStringFormat_out( axStringFormat &f, char		value );
axStatus axStringFormat_out( axStringFormat &f, wchar_t		value );

axStatus axStringFormat_out( axStringFormat &f, const char*		value );
axStatus axStringFormat_out( axStringFormat &f, const wchar_t*  value );
axStatus axStringFormat_out( axStringFormat &f, const axUChar*  value );

inline axStatus axStringFormat_out( axStringFormat &f, axSize	value ) { return axStringFormat_out( f, value.asNative() ); }
inline axStatus axStringFormat_out( axStringFormat &f, char*	value ) { return axStringFormat_out( f, (const char*)   value ); }
inline axStatus axStringFormat_out( axStringFormat &f, wchar_t*	value ) { return axStringFormat_out( f, (const wchar_t*)value ); }
inline axStatus axStringFormat_out( axStringFormat &f, axUChar*	value ) { return axStringFormat_out( f, (const axUChar*)value ); }

axStatus axStringFormat_out( axStringFormat &f, const void* p );

template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, T*	  value ) { return axStringFormat_out( f, (const void* &)value ); }

template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, const T& value ) { return value.toStringFormat( f ); }
//---------


axStatus	axStringFormat_out( axStringFormat &f, axStatus value );


//@}

#endif //__ax_StringFormat_h__

