#pragma once
#ifndef __ax_str_to_h__
#define __ax_str_to_h__

#include "axString.h"
#include "../data_structure/axArray_types.h"

//! \ingroup base_string
//@{

axStatus	ax_to_utf16_array( axIArray< axUTF16 > & out, const char*	 sz );
axStatus	ax_to_utf16_array( axIArray< axUTF16 > & out, const wchar_t* sz );

inline	axStatus ax_str_to( const char*		sz, axIStringA &v )	{ return v.set(sz); }
inline	axStatus ax_str_to( const char*		sz, axIStringW &v )	{ return v.set(sz); }

inline	axStatus ax_str_to( const wchar_t*	sz, axIStringA &v )	{ return v.set(sz); }
inline	axStatus ax_str_to( const wchar_t*	sz, axIStringW &v )	{ return v.set(sz); }

//==================  ax_str_to char ============================

axStatus ax_str_to( const char* sz, bool		&v );
axStatus ax_str_to( const char* sz, uint8_t    	&v );
axStatus ax_str_to( const char* sz, uint16_t    &v );
axStatus ax_str_to( const char* sz, uint32_t   	&v );
axStatus ax_str_to( const char* sz, uint64_t   	&v );
axStatus ax_str_to( const char* sz, int8_t    	&v );
axStatus ax_str_to( const char* sz, int16_t    	&v );
axStatus ax_str_to( const char* sz, int32_t   	&v );
axStatus ax_str_to( const char* sz, int64_t   	&v );
axStatus ax_str_to( const char* sz, float  		&v );
axStatus ax_str_to( const char* sz, double 		&v );
axStatus ax_str_to( const char* sz, axSize 		&v );

//==================  ax_str_to wchar ============================


axStatus ax_str_to( const wchar_t* sz, uint8_t  &v );
axStatus ax_str_to( const wchar_t* sz, uint16_t &v );
axStatus ax_str_to( const wchar_t* sz, uint32_t &v );
axStatus ax_str_to( const wchar_t* sz, uint64_t &v );
axStatus ax_str_to( const wchar_t* sz, int8_t	&v );
axStatus ax_str_to( const wchar_t* sz, int16_t  &v );
axStatus ax_str_to( const wchar_t* sz, int32_t  &v );
axStatus ax_str_to( const wchar_t* sz, int64_t  &v );
axStatus ax_str_to( const wchar_t* sz, float    &v );
axStatus ax_str_to( const wchar_t* sz, double   &v );
//==============================

uint32_t ax_string_hash( const char *sz );
uint32_t ax_string_hash( const wchar_t *sz );


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
	return axStatus_Std::hex_format_error;
}


axStatus 	ax_hex_str_to 	( axSize &out, const char *v );
axStatus 	ax_encode_xor	( axIByteArray &output, const axIByteArray &input, uint8_t key );
void 		ax_encode_xor	( axIByteArray &in_and_out, uint8_t key );
//@}

#endif //__ax_str_to_h__
