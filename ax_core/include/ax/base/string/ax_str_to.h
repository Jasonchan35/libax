#pragma once
#ifndef __ax_str_to_h__
#define __ax_str_to_h__

#include "axString.h"
#include "../../base/data_structure/axArray_types.h"

//! \ingroup base_string
//@{

#ifdef __OBJC__
inline
NSString* ax_toNSString( const char *sz ) {
	if( sz == NULL ) return @"";
	return [NSString stringWithUTF8String: sz ];
}

inline
NSString* ax_toNSString( const wchar_t *sz ) {
	if( sz == NULL ) return @"";
	axTempStringA lstr;	
	if( ! lstr.set( sz ) ) return nil;
	return ax_toNSString( lstr );
}

inline
NSString* ax_toNSString( const axUChar *sz ) {
	if( sz == NULL ) return @"";
	axTempStringA lstr;	
	if( ! lstr.set( sz ) ) return nil;
	return ax_toNSString( lstr );
}

#endif //__OBJC__

inline	axStatus ax_str_to( const char*		sz, axIStringA &v )	{ return v.set(sz); }
inline	axStatus ax_str_to( const char*		sz, axIStringW &v )	{ return v.set(sz); }

inline	axStatus ax_str_to( const wchar_t*	sz, axIStringA &v )	{ return v.set(sz); }
inline	axStatus ax_str_to( const wchar_t*	sz, axIStringW &v )	{ return v.set(sz); }


//==================  ax_str_to char ============================
inline
axStatus ax_str_to( const char* sz, bool &v ) {
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
axStatus ax_str_to( const char* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const char* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const char* sz, uint32_t   &v ) {
	if( 1 != sscanf( sz, "%u", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const char* sz, uint64_t   &v ) {
	if( 1 != sscanf( sz, "%llu", (long long unsigned*)&v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const char* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const char* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const char* sz, int32_t   &v ) {
	if( 1 != sscanf( sz, "%d", &v ) )
		return -1;
	return 0;
}


inline
axStatus ax_str_to( const char* sz, int64_t   &v ) {
	if( 1 != sscanf( sz, "%lld", (long long int *)&v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const char* sz, float  &v ) {
	if( 1 != sscanf( sz, "%f", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const char* sz, double &v ) {
	if( 1 != sscanf( sz, "%lf", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const char* sz, axSize &v ) {
	return ax_str_to( sz, v.asNative() );
}


//==================  ax_str_to wchar ============================

inline
axStatus ax_str_to( const wchar_t* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const wchar_t* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const wchar_t* sz, uint32_t   &v ) {
	if( 1 != swscanf( sz, L"%u", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const wchar_t* sz, uint64_t   &v ) {
	if( 1 != swscanf( sz, L"%llu", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const wchar_t* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const wchar_t* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

inline
axStatus ax_str_to( const wchar_t* sz, int32_t   &v ) {
	if( 1 != swscanf( sz, L"%d", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const wchar_t* sz, int64_t   &v ) {
	if( 1 != swscanf( sz, L"%lld", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const wchar_t* sz, float  &v ) {
	if( 1 != swscanf( sz, L"%f", &v ) )
		return -1;
	return 0;
}

inline
axStatus ax_str_to( const wchar_t* sz, double &v ) {
	if( 1 != swscanf( sz, L"%lf", &v ) )
		return -1;
	return 0;
}

//==============================


inline
uint32_t ax_string_hash( const char *sz ) {
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
uint32_t ax_string_hash( const wchar_t *sz ) {
    uint32_t h = 5381;
    uint32_t c;
    while ( 0 != (c=(uint32_t)*sz++) )
        h = ((h<<5)+h)+c; // h * 33 + c
	return h;
}

template<class T> inline
axStatus ax_to_hex_str ( axIString_<T> &out, const axIByteArray &b ) {
	const char* hex = ax_HEX_chars();

	axStatus st;

	axSize	n = b.byteSize();
	st = out.resize( n*2 );		if( !st ) return st;

	T* o = out._getInternalBufferPtr();
	const uint8_t* p = b.ptr();

	for( axSize i=0; i<n; i++ ) {
		*o = (T)hex[ (p[i] >> 4) ];	o++;
		*o = (T)hex[ (p[i] % 16) ];	o++;
	}

	return 0;
}

template<class T> inline
int ax_hex_to_int( T ch ) {
	if( ch >= '0' && ch <='9' ) return ch -'0';
	if( ch >= 'a' && ch <='f' ) return ch -'a' + 10;
	if( ch >= 'A' && ch <='F' ) return ch -'A' + 10;
	return -1;
}

inline 
axStatus ax_hex_str_to ( axSize &out, const char *v ) {
	out = 0 ;
	if( !*v ) return -1;

	size_t o = 0;
	int ret;
	for( ; *v; v++ ) {
		ret = ax_hex_to_int( *v );
		if( ret < 0 ) return -2;
		o <<=4;
		o |= ret;
	}
	out = o;
	return 0;
}


inline axStatus ax_encode_xor( axIByteArray &output, const axIByteArray &input, uint8_t key ) {
	axStatus st ;
	st = output.resize( input.size() ); if(!st) return st;
	
	uint8_t *p = output.ptr();
	
	for( size_t i=0; i<input.size(); i++ ) {
		p[i] = input[i] ^ key;
	}
	return 0;
}



inline void ax_encode_xor( axIByteArray &in_and_out, uint8_t key ) {
	ax_encode_xor( in_and_out, in_and_out, key );
}

/*
#include "../data_structure/axArray_types.h".
inline void ax_encode_xor( axIByteArray &in_and_out, uint8_t key ) {

inline
void ax_encode_xor( axIStringA &str, uint8_t key ) {
	size_t i;
	char *p = str._getInternalBufferPtr();
	for( i=0; i<str.size(); i++ ) {
		p[i] ^= key;
	}
}


inline
void ax_encode_xor( axIStringW &str, uint8_t key ) {
	size_t i;
	wchar_t *p = str._getInternalBufferPtr();
	for( i=0; i<str.size(); i++ ) {
		p[i] ^= key;
	}
}
inline
void ax_xor( uint8_t *buf, size_t len, uint8_t key ) {
	size_t i;
	for( i=0; i<len; i++ ) {
		buf[i] ^= key;
	}
}


*/

//@}

#endif //__ax_str_to_h__
