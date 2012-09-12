//
//  ax_str_encode.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/ax_str_encode.h>
#include <ax/core/string/ax_str_to.h>

static const char    k_ax_base64_encode_table[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const uint8_t INV = 0x80;
static const uint8_t k_ax_base64_decode_table[] = {
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV, 62,INV,INV,INV, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,INV,INV,
	INV,INV,INV,INV,INV,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,INV,INV,INV,INV,INV,INV, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
	49, 50, 51,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,
	INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV,INV
};

template<class T> inline
axStatus _ax_decodeBase64( axIString_<T> &dst, const T* src ) {
	axStatus st;
	dst.clear();

	T block[4];
	uint8_t tmp;
	size_t i, count;

	size_t src_len = ax_strlen( src );
	axSize dst_max_len = src_len / 4 * 3 + 4 + 1; //  4-byte to 3-byte blocks with null termination

	st = dst.resize( dst_max_len ); if( !st ) return st;

	T *pos = dst._getInternalBufferPtr();

	count = 0;

	for ( i=0; i<src_len; i++ ) {

		tmp = k_ax_base64_decode_table[ (uint8_t)src[i] ];
		if ( tmp == INV ) continue;

		block[ count ] = tmp;
		count++;

		if ( count == 4 ) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) |  block[3];
			count = 0;
		}
	}

	if( count > 0 ) {

		for( i=count; i<4; i++ ) {
			block[i] = '\0';
		}

		*pos++ = (block[0] << 2) | (block[1] >> 4);
		*pos++ = (block[1] << 4) | (block[2] >> 2);
		*pos++ = ((block[2] << 6) & 0xc0 ) |  block[3];

	}

	*pos++ = 0;
	dst._recalcSizeByZeroEnd();

	return 0;
}



template<class T> inline
axStatus _ax_encodeBase64( axIString_<T> &dst, const T* src ) {
	dst.clear();

	axStatus st;
	T *pos;
	const T *e, *s;

	size_t len = ax_strlen( src );
	size_t new_len;

	new_len = len * 4 / 3 + 4 + 1; // 3-byte blocks to 4-byte with null termination

	st = dst.resize( new_len ); if (!st) return st;

	e = src + len;
	s = src;

	pos = dst._getInternalBufferPtr();

	T input[3];

	while( e - s >= 3 ) {
		input[0] = *s++;
		input[1] = *s++;
		input[2] = *s++;
		*pos++ = k_ax_base64_encode_table[ (uint8_t) input[0] >> 2 ];
		*pos++ = k_ax_base64_encode_table[ (uint8_t) ((input[0] & 0x03) << 4) + (input[1] >> 4) ];
		*pos++ = k_ax_base64_encode_table[ (uint8_t) ((input[1] & 0x0f) << 2) + (input[2] >> 6) ];
		*pos++ = k_ax_base64_encode_table[ (uint8_t) input[2] & 0x3f];
	}

	size_t remain = (size_t)( e - s );

	if( remain != 0 ) {
		input[0] = input[1] = input[2] = '\0';

		T output[3];
		output[0] = output[1] = output[2] = '\0';

		size_t i=0;
		for (i = 0; i < remain; i++)
			input[i] = *s++;

		output[0] = input[0] >> 2;
		output[1] = ((input[0] & 0x03) << 4) + (input[1] >> 4);
		output[2] = ((input[1] & 0x0f) << 2) + (input[2] >> 6);


		*pos++ = k_ax_base64_encode_table[ (uint8_t) output[0] ];
		*pos++ = k_ax_base64_encode_table[ (uint8_t) output[1] ];

		if( remain == 1 ) {
			*pos++ = '=';
		}else {
			*pos++ = k_ax_base64_encode_table[ (uint8_t) output[2] ];
		}
		*pos++ = '=';
	}

	*pos++ = 0;
	dst._recalcSizeByZeroEnd();

	return 0;
}


axStatus ax_decodeBase64( axIStringA &dst, const char*		src ) { return _ax_decodeBase64(dst,src); }
axStatus ax_decodeBase64( axIStringW &dst, const wchar_t*	src ) { return _ax_decodeBase64(dst,src); }

axStatus ax_encodeBase64( axIStringA &dst, const char*		src ) { return _ax_encodeBase64(dst,src); }
axStatus ax_encodeBase64( axIStringW &dst, const wchar_t*	src ) { return _ax_encodeBase64(dst,src); }

template<class T> inline
axStatus _ax_decodeURI( axIString_<T> &dst, const T* src ) {
	axStatus st;
	const T* p = src;
	const T* s = p;
	int ret;
	for( ;*p; ) {
		switch( *p ) {
			case (T)'%': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;

				p++; ret = ax_hex_to_int( *p );			if( ret < 0 ) return axStatus_Std::error_decodeURI;
				char hex = ret << 4;

				p++; ret = ax_hex_to_int( *p );			if( ret < 0 ) return axStatus_Std::error_decodeURI;
				hex |= ret;

				st = dst.append( hex );					if( !st ) return st;
				p++;
				s = p;
			}break;
			case '+': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;
				st = dst.append( ' ' );					if( !st ) return st;
				p++;
				s = p;
			}break;
			default: p++;
		}
	}

	st = dst.appendWithLength( s, p-s );		if( !st ) return st;
	return 0;
}

axStatus ax_decodeURI( axIStringA &dst, const char*		src ) { return _ax_decodeURI(dst,src); }
axStatus ax_decodeURI( axIStringW &dst, const wchar_t*	src ) { return _ax_decodeURI(dst,src); }

