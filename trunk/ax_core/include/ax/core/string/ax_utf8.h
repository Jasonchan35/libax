#pragma once
#ifndef __ax_utf8_h__
#define __ax_utf8_h__

#include "ax_c_str.h"

//! \ingroup base_string
//@{

inline int ax_utf8_to_wchar( wchar_t &out, const char *utf8, axSize len ) {
	if( len < 1 ) return 0;
	if( (utf8[0] & 0x80) == 0 ) {
		out = (wchar_t)utf8[0];
		return 1;
	}
	if( (utf8[0] & 0xE0) == 0xC0 ) {
		if( len < 2 ) return 0;
		out = (wchar_t)(utf8[0] & 0x1F ) << 6
			| (wchar_t)(utf8[1] & 0x3F );
		return 2;
	}
	if( (utf8[0] & 0xF0) == 0xE0 ) {
		if( len < 3 ) return 0;
		out = (wchar_t)(utf8[0] & 0x0F ) << 12
			| (wchar_t)(utf8[1] & 0x3F ) << 6
			| (wchar_t)(utf8[2] & 0x3F );
		return 3;
	}

	assert(false);
	return -1;
}

inline int ax_wchar_to_utf8( char *utf8, axSize utf8_len, wchar_t wc ) {
	if( wc <= 0x7F ) {
		if( utf8_len < 1 ) return -1;
		utf8[0] = (char)wc;
		return 1;
	}
	if( wc <= 0x7FF ) {
		if( utf8_len < 2 ) return -2;
		utf8[0] = ( wc >> 6 ) | 0xC0;
		utf8[1] = ( wc & 0x3F ) | 0x80;
		return 2;
	}
	if(utf8_len < 3) return -3;
	utf8[0] = ( wc >> 12) | 0xE0;
	utf8[1] = ((wc >> 6 ) & 0x3F) | 0x80;
	utf8[2] = ( wc & 0x3F) | 0x80;
	return 3;
}


inline int ax_utf8_to_uchar( axUChar &out, const char *utf8, axSize len ) {
	if( len < 1 ) return 0;
	if( (utf8[0] & 0x80) == 0 ) {
		out = (axUChar)utf8[0];
		return 1;
	}
	if( (utf8[0] & 0xE0) == 0xC0 ) {
		if( len < 2 ) return 0;
		out = (axUChar)(utf8[0] & 0x1F ) << 6
			| (axUChar)(utf8[1] & 0x3F );
		return 2;
	}
	if( (utf8[0] & 0xF0) == 0xE0 ) {
		if( len < 3 ) return 0;
		out = (axUChar)(utf8[0] & 0x0F ) << 12
			| (axUChar)(utf8[1] & 0x3F ) << 6
			| (axUChar)(utf8[2] & 0x3F );
		return 3;
	}
	return -1;
}

inline int ax_uchar_to_utf8( char *utf8, axSize utf8_len, axUChar wc ) {
	if( wc <= 0x7F ) {
		if( utf8_len < 1 ) return -1;
		utf8[0] = ((axUChar)wc) & 0x00FF;
		return 1;
	}
	if( wc <= 0x7FF ) {
		if( utf8_len < 2 ) return -2;
		utf8[0] = ( (axUChar)wc >> 6 ) | 0xC0;
		utf8[1] = ( (axUChar)wc & 0x3F ) | 0x80;
		return 2;
	}
	if(utf8_len < 3) return -3;
	utf8[0] = ( (axUChar)wc >> 12) | 0xE0;
	utf8[1] = (((axUChar)wc >> 6 ) & 0x3F) | 0x80;
	utf8[2] = ( (axUChar)wc & 0x3F) | 0x80;
	return 3;
}

inline
axStatus ax_utf8_count_in_wchar( axSize &out_len, const wchar_t* wcs ) {
	out_len = 0;
	for( ; *wcs; wcs++ ) {
		if( *wcs <= 0x7F  ) { out_len++;  continue; }
		if( *wcs <= 0x7FF ) { out_len+=2; continue;	}
		out_len+=3;
	}
	return 0;
}

inline
axStatus ax_utf8_count_in_uchar( axSize &out_len, const axUChar* wcs ) {
	out_len = 0;
	for( ; *wcs; wcs++ ) {
		if( *wcs <= 0x7F  ) { out_len++;  continue; }
		if( *wcs <= 0x7FF ) { out_len+=2; continue;	}
		out_len+=3;
	}
	return 0;
}

inline
axStatus ax_utf8_count_in_wchar( axSize &out_len, wchar_t wc ) {
	if( wc <= 0x7F ) {
		out_len = 1;
	}else if( wc <= 0x7FF ) {
		out_len = 2;
	}else{
		out_len = 3;
	}
	return 0;
}

inline
axStatus ax_utf8_count_in_wchar( axSize &out_len, const wchar_t* wcs, axSize wcs_len ) {
	axSize i;
	out_len = 0;
	for( i=0; i<wcs_len && *wcs; i++, wcs++ ) {
		if( *wcs <= 0x7F  ) { out_len++;  continue; }
		if( *wcs <= 0x7FF ) { out_len+=2; continue; }
		out_len+=3;
	}
	return 0;
}

inline
axStatus ax_utf8_count_in_uchar( axSize &out_len, const axUChar* wcs, axSize wcs_len ) {
	axSize i;
	out_len = 0;
	for( i=0; i<wcs_len && *wcs; i++, wcs++ ) {
		if( *wcs <= 0x7F  ) { out_len++;  continue; }
		if( *wcs <= 0x7FF ) { out_len+=2; continue; }
		out_len+=3;
	}
	return 0;
}

inline
axStatus ax_wchar_count_in_utf8( axSize &out_len, const char* utf8 ) {
	out_len = 0;
	for( ; *utf8; utf8++, out_len++ ) {
		if( (*utf8 & 0x80) == 0 ) {
			continue;
		}
		if( (*utf8 & 0xE0) == 0xC0 ) {
			utf8++;	if( *utf8 == 0 ) return axStatus_Std::invalid_parameter;
			continue;
		}
		if( (*utf8 & 0xF0) == 0xE0 ) {
			utf8++;	if( *utf8 == 0 ) return axStatus_Std::invalid_parameter;
			utf8++;	if( *utf8 == 0 ) return axStatus_Std::invalid_parameter;
			continue;
		}
	}
	return 0;
}

inline
axStatus ax_wchar_count_in_utf8( axSize &out_len, const char* utf8, axSize utf8_len ) {
	out_len = 0;
	axSize i;
	for( i=0; i<utf8_len && *utf8; i++, utf8++, out_len++ ) {
		if( (*utf8 & 0x80) == 0 ) {
			continue;
		}
		if( (*utf8 & 0xE0) == 0xC0 ) {
			utf8++;	if( *utf8 == 0 ) return axStatus_Std::invalid_parameter;
			continue;
		}
		if( (*utf8 & 0xF0) == 0xE0 ) {
			utf8++;	if( *utf8 == 0 ) return axStatus_Std::invalid_parameter;
			utf8++;	if( *utf8 == 0 ) return axStatus_Std::invalid_parameter;
			continue;
		}
	}
	return 0;
}


//@}

#endif //__ax_utf8_h__

