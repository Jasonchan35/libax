#pragma once
#ifndef __ax_str_to_h__
#define __ax_str_to_h__

#include "../common/axStatus.h"

//! \ingroup base_string
//@{

inline void		ax_dumpStringHex( const char*    sz, FILE* file = stdout ) { ax_dumpBufferHex( sz, ax_strlen(sz), file ); }
inline void		ax_dumpStringHex( const wchar_t* sz, FILE* file = stdout ) { ax_dumpBufferHex( sz, ax_strlen(sz)*sizeof(wchar_t), file ); }

//==================  str_to char ============================
inline
axStatus str_to( const char* sz, bool &v ) {
	if( ax_strcasecmp( sz, "true" ) == 0 ) {
		v = true;
		return 0;
	}else if( ax_strcasecmp( sz, "false" ) == 0 ) {
		v = false;
		return 0;
	}
	return -1;
}

inline
axStatus str_to( const char* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const char* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const char* sz, uint32_t   &v ) {
	if( 1 != sscanf( sz, "%u", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const char* sz, uint64_t   &v ) {
	if( 1 != sscanf( sz, "%llu", (long long unsigned*)&v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const char* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const char* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const char* sz, int32_t   &v ) {
	if( 1 != sscanf( sz, "%d", &v ) )
		return -1;
	return 0;
}


inline
axStatus str_to( const char* sz, int64_t   &v ) {
	if( 1 != sscanf( sz, "%lld", (long long int *)&v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const char* sz, float  &v ) {
	if( 1 != sscanf( sz, "%f", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const char* sz, double &v ) {
	if( 1 != sscanf( sz, "%lf", &v ) )
		return -1;
	return 0;
}


//==================  str_to wchar ============================

inline
axStatus str_to( const wchar_t* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const wchar_t* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const wchar_t* sz, uint32_t   &v ) {
	if( 1 != swscanf( sz, L"%u", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const wchar_t* sz, uint64_t   &v ) {
	if( 1 != swscanf( sz, L"%llu", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const wchar_t* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const wchar_t* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus str_to( const wchar_t* sz, int32_t   &v ) {
	if( 1 != swscanf( sz, L"%d", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const wchar_t* sz, int64_t   &v ) {
	if( 1 != swscanf( sz, L"%lld", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const wchar_t* sz, float  &v ) {
	if( 1 != swscanf( sz, L"%f", &v ) )
		return -1;
	return 0;
}

inline
axStatus str_to( const wchar_t* sz, double &v ) {
	if( 1 != swscanf( sz, L"%lf", &v ) )
		return -1;
	return 0;
}

//==============================


inline
uint32_t string_hash( const char *sz ) {
	/*
	"djb2" string hash
	this algorithm (k=33) was first reported by dan bernstein many years ago
	in comp.lang.c. another version of this algorithm (now favored by bernstein)
	uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
	(why it works better than many other constants, prime or not) has never
	been adequately explained.
	*/
    uint32_t h = 5381;
    uint32_t c;
    while ( 0 != (c=(uint32_t)*sz++) )
        h = ((h<<5)+h)+c; // h * 33 + c
	return h;
}

inline
uint32_t string_hash( const wchar_t *sz ) {
    uint32_t h = 5381;
    uint32_t c;
    while ( 0 != (c=(uint32_t)*sz++) )
        h = ((h<<5)+h)+c; // h * 33 + c
	return h;
}

inline 
axStatus hex_str_to ( axSize &out, const char *v ) {
	out = 0 ;
	if( !*v ) return -1;

	while( *v ) {

		if ( *v >= '0' && *v <= '9' ) {
			out = out * 0x10 + (*v - '0');
		}else if ( *v >= 'A' && *v <= 'F') {
			out = out * 0x10 + (*v - 'A' +10);
		}else if ( *v >= 'a' && *v <= 'f') {
			out = out * 0x10 + (*v - 'a' +10);
		}else {
			return -2;
		}

		v++;
	}
	return 0;
}

//@}

#endif //__ax_str_to_h__
