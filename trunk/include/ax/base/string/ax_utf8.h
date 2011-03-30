#pragma once
#ifndef __ax_utf8_h__
#define __ax_utf8_h__

//! \addtogroup string
//@{

inline int utf8_to_wchar( wchar_t &out_wc, const char *mb, axSize len ) {
	if( len < 1 ) return 0;
	if( (mb[0] & 0x80) == 0 ) {
		out_wc = (wchar_t)mb[0];
		return 1;
	}
	if( (mb[0] & 0xE0) == 0xC0 ) {
		if( len < 2 ) return 0;
		out_wc =  (wchar_t)(mb[0] & 0x1F ) << 6
				| (wchar_t)(mb[1] & 0x3F );
		return 2;
	}
	if( (mb[0] & 0xF0) == 0xE0 ) {
		if( len < 3 ) return 0;
		out_wc =  (wchar_t)(mb[0] & 0x0F ) << 12
				| (wchar_t)(mb[1] & 0x3F ) << 6
				| (wchar_t)(mb[2] & 0x3F );
		return 3;
	}
	return 0;
}

inline int wchar_to_utf8( char *mb, axSize mb_len, const wchar_t wc ) {
	if( wc <= 0x7F ) {
		if( mb_len < 1 ) return -1;
		mb[0] = ((wchar_t)wc) & 0x00FF;
		return 1;
	}
	if( wc <= 0x7FF ) {
		if( mb_len < 2 ) return -2;
		mb[0] = ( wc >> 6 ) | 0xC0;
		mb[1] = ( wc & 0x3F ) | 0x80;
		return 2;
	}
	if(mb_len < 3) return -3;
	mb[0] = (wc >> 12) | 0xE0;
	mb[1] = ((wc >> 6 ) & 0x3F) | 0x80;
	mb[2] = (wc & 0x3F) | 0x80;
	return 3;
}


inline
axStatus utf8_count_in_wchar( axSize &out_len, const wchar_t* sz ) {
	out_len = 0;
	for( ; *sz; sz++ ) {
		if( *sz <= 0x7F ) {
			out_len++;
			continue;
		}
		if( *sz <= 0x7FF ) {
			out_len+=2;
			continue;
		}
		out_len+=3;
	}
	return 0;
}

inline
axStatus wchar_count_in_utf8( axSize &out_len, const char* sz ) {
	out_len = 0;
	for( ; *sz; sz++ ) {
		if( (*sz & 0x80) == 0 ) {
			out_len++;
			continue;
		}
		if( (*sz & 0xE0) == 0xC0 ) {
			sz++;	if( *sz == 0 ) return axStatus::invalid_param;
			out_len+=2;
			continue;
		}
		if( (*sz & 0xF0) == 0xE0 ) {
			sz++;	if( *sz == 0 ) return axStatus::invalid_param;
			sz++;	if( *sz == 0 ) return axStatus::invalid_param;
			out_len+=3;
			continue;
		}
	}
	return 0;
}

//@}

#endif //__ax_utf8_h__

