//
//  ax_str_to.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/ax_str_to.h>

axStatus	ax_to_utf16_array( axIArray< axUTF16 > & out, const char*	  sz ) {
	axStatus st;
	axTempStringW	w;
	st = w.set( sz );		if( !st ) return st;
	return ax_to_utf16_array( out, w );
}

axStatus	ax_to_utf16_array( axIArray< axUTF16 > & out, const wchar_t* sz ) {
	axStatus st;
	size_t n = ax_strlen( sz )+1;
	st = out.resize( n, false );		if( !st ) return st;

	const wchar_t* src = sz;
		  axUTF16* dst = out.ptr();	

	for( size_t i=0; i<n; i++ ) {
		*dst = *src;
		dst++;
		src++;
	}
	out[n] = 0;
	return 0;
}


//==================  ax_str_to char ============================
axStatus ax_str_to( const char* sz, bool &v ) {
	if( ax_strcasecmp( sz, "true" ) == 0 ) {
		v = true;
		return 0;
	}else if( ax_strcasecmp( sz, "false" ) == 0 ) {
		v = false;
		return 0;
	}
	return axStatus_Std::str_to_format_error;
}

axStatus ax_str_to( const char* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const char* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const char* sz, uint32_t   &v ) {
	if( 1 != sscanf( sz, "%u", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, uint64_t   &v ) {
	if( 1 != sscanf( sz, "%llu", (long long unsigned*)&v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const char* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const char* sz, int32_t   &v ) {
	if( 1 != sscanf( sz, "%d", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, int64_t   &v ) {
	if( 1 != sscanf( sz, "%lld", (long long int *)&v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, float  &v ) {
	if( 1 != sscanf( sz, "%f", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, double &v ) {
	if( 1 != sscanf( sz, "%lf", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, axSize &v ) {
	return ax_str_to( sz, v.asNative() );
}


//==================  ax_str_to wchar ============================
axStatus ax_str_to( const wchar_t* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const wchar_t* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const wchar_t* sz, uint32_t   &v ) {
	if( 1 != swscanf( sz, L"%u", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, uint64_t   &v ) {
	if( 1 != swscanf( sz, L"%llu", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return -1;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const wchar_t* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return axStatus_Std::str_to_format_error;
	return ax_safe_assign( v, tmp );
}

axStatus ax_str_to( const wchar_t* sz, int32_t   &v ) {
	if( 1 != swscanf( sz, L"%d", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, int64_t   &v ) {
	if( 1 != swscanf( sz, L"%lld", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, float  &v ) {
	if( 1 != swscanf( sz, L"%f", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, double &v ) {
	if( 1 != swscanf( sz, L"%lf", &v ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

//==============================

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

uint32_t ax_string_hash( const wchar_t *sz ) {
    uint32_t h = 5381;
    uint32_t c;
    while ( 0 != (c=(uint32_t)*sz++) )
        h = ((h<<5)+h)+c; // h * 33 + c
	return h;
}

 
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


axStatus ax_encode_xor( axIByteArray &output, const axIByteArray &input, uint8_t key ) {
	axStatus st ;
	st = output.resize( input.size() ); if(!st) return st;
	
	uint8_t *p = output.ptr();
	
	for( size_t i=0; i<input.size(); i++ ) {
		p[i] = input[i] ^ key;
	}
	return 0;
}



void ax_encode_xor( axIByteArray &in_and_out, uint8_t key ) {
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
