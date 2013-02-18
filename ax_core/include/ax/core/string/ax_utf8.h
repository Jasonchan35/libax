#pragma once
#ifndef __ax_utf8_h__
#define __ax_utf8_h__

#include "ax_c_str.h"

//! \ingroup base_string
//@{

int ax_utf8_to_wchar( wchar_t &out, const char *utf8, axSize len );
int ax_wchar_to_utf8( char *utf8, axSize utf8_len, wchar_t wc );

axStatus ax_utf8_count_in_wchar( axSize &out_len, const wchar_t* wcs );
axStatus ax_utf8_count_in_wchar( axSize &out_len, wchar_t wc );
axStatus ax_utf8_count_in_wchar( axSize &out_len, const wchar_t* wcs, axSize wcs_len );

axStatus ax_wchar_count_in_utf8( axSize &out_len, const char* utf8 );
axStatus ax_wchar_count_in_utf8( axSize &out_len, const char* utf8, axSize utf8_len );

//@}

#endif //__ax_utf8_h__

