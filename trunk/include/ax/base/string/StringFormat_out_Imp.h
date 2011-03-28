#pragma once
#ifndef __ax_StringFormat_out_Imp_h__
#define __ax_StringFormat_out_Imp_h__

#include "StringFormat_Imp.h"
namespace ax {
//! \addtogroup string
//@{


template< class T> inline
int StringFormat_to_digi_signed( wchar_t* buf, const int max_digi, T value, int base, bool exponent, int precision, const wchar_t *table ) {
	T v = value;
	T b = (T)base;
	int n;
	for( n=0; n<max_digi; ) {
		v /= b;
		n++;
		if( v == 0 ) break;
	}
	if( value < 0 ) n++;	
	if( n >= max_digi ) { assert( false ); return 0; }
	
	wchar_t* out = buf + n - 1;
	
	if( value < 0 ) buf[0] = L'-';
	
	v = value;
	int i;
	for( i=0; i<n; ) {
		T a = v % b;
		*out = (value < 0) ? table[-a] : table[a];
		out--;
		v /= b;
		i++;
		if( v == 0 ) break;
	}
	return n;
}

template< class T> inline
int StringFormat_to_digi_unsigned( wchar_t* buf, const int max_digi, T value, int base, bool exponent, int precision, const wchar_t *table ) {
	T v = value;
	T b = (T)base;
	int i;
	
	for( i=0; i<max_digi; ) {
		v /= b;
		i++;
		if( v == 0 ) break;
	}
	wchar_t* out = buf + i - 1;
	
	v = value;
	for( i=0; i<max_digi; ) {
		T a = v % b;
		*out = table[a];
		out--;
		v /= b;
		i++;
		if( v == 0 ) break;
	}
	return i;
}

const wchar_t	StringFormat_to_str_hex[] = L"0123456789abcdef";
const wchar_t	StringFormat_to_str_HEX[] = L"0123456789ABCDEF";

template< class T > inline
Status StringFormat_out_NumberT( StringFormat &f, T value ) {
	const int max_digi = 400;
	
	Status st;
	//printf("to_str(int) value=%X opt=%ls\n", value, opt );
	bool	plus_sign	= false;
	bool	align_left  = false;
	bool	exponent	= false;
	int		padding		= 0;
	int		base		= 10; //default is dec
	int		precision   = -1;
	wchar_t	fill_ch		= L' ';
	const wchar_t *table = StringFormat_to_str_hex;
	
	const wchar_t* opt = f.opt.c_str();
	if( opt ) {
		Size len = ax_strlen(opt);
		if( len ) {
			//check last char
			bool done = false;
			do {
				wchar_t last_ch = opt[ len-1 ];
				switch( last_ch ) {
					case 'c': f.out( (wchar_t)value ); return 0; // character
					case 'b': len--; base = 2;  break; //binary
					case 'o': len--; base = 8;  break; //octer
					case 'd': len--; base = 10; break; //decmial
					case 'x': len--; base = 16; break; //hex
					case 'X': len--; base = 16; table = StringFormat_to_str_HEX; break; //HEX
						
					case 'e': len--; exponent = true; break; //for floating point only
					default : done = true;
				}
			}while( !done );
			
			const wchar_t *p = opt;
			switch( *p ) {
				case '+': p++; len--; plus_sign  = true; break;
				case '-': p++; len--; align_left = true; break;
				case '0': p++; len--; fill_ch = '0';	 break;
			}
			
			StringW_<64>	tmp;
			st = tmp.set( p, len );			if( !st ) return st;
			
			wchar_t* dot = ( wchar_t*) ax_strchr( tmp, L'.' );
			if( dot ) {
				*dot = 0;
				st = str_to( dot+1, precision );	if( !st ) return st;
			}
			
			str_to( tmp, padding );
		}
	}
	
	//printf("opt plus_sign=%s, padding=%d, ch_base=%c\n", plus_sign?"true":"false", padding, ch_base );
	wchar_t ch[ max_digi+1 ];
	ch[ max_digi ] = 0;
	int digi = 0;
	
	digi = StringFormat_to_digi( ch, max_digi, value, base, exponent, precision, table );
	if( digi < 0 ) { assert(false); return -1; }
	
	int space = 0;
	
	if( ! TypeOf<T>::isUnsigned() && _lessThan0( value ) ) {
		plus_sign = false;
	}
	
	int req_size = digi;
	if( plus_sign ) req_size++;
	if( req_size < padding ) {
		space = padding - req_size;
	}
	
	if( ! align_left ) { //fill space at the front
		st = f.fill( fill_ch, space );	if( !st ) return st;
	}
	
	if( plus_sign ) {
		st = f.out( '+' );	if( !st ) return st;
	}
	
	st = f.out( ch,digi );	if( !st ) return st;
	
	if( align_left ) { //fill space at the end
		st = f.fill( fill_ch, space );	if( !st ) return st;
	}
	
	//printf("\n str = [%ls] \n\n", str.sz() );
	return 0;
}



//pointer
inline
Status StringFormat_out( StringFormat &f, const void* p ) {
	f.opt.set( L"X" );
#ifdef axCPU_LP32
	return StringFormat_out_NumberT( f, (uint32_t)(uintptr_t)p );
#elif axCPU_LP64
	return StringFormat_out_NumberT( f, (uint64_t)(uintptr_t)p );
#else
	#error
#endif
}

//const char* string
inline Status StringFormat_out( StringFormat &f, const char*  src ) { 
	if( !src ) return 0;

	Status st;

	int ret;
	Size src_len;	
	st = safe_assign( src_len, ax_strlen(src) );		if( !st ) return st;

	const int	buf_size = 128;
	wchar_t		buf[buf_size];
	Size w=0, m=0;

	for(;;) {
		ret = utf8_to_wchar( buf[w], src+m, src_len+1+1 );
		if( ret <= 0 ) {
			assert( false ); //corrupted_utf8
			break;
		}
		if( !buf[w] ) break; //zero end
		m += ret;
		w++;

		if( w >= buf_size ) {
			f.out( buf, w );
			w = 0;
		}
	}

	f.out( buf, w );
	return 0; 
}

//const wchar_t* string
inline Status StringFormat_out( StringFormat &f, const wchar_t*  value ) { 
	f.out( value );
	return 0; 
}

//==== signed int
#define	axTYPE_LIST(T)	\
	inline Status StringFormat_out( StringFormat &f, T   value ) { return StringFormat_out_NumberT( f, value ); } \
	inline int StringFormat_to_digi( wchar_t* buf, const int max_digi, T  value, int base, bool exponent, int precision, const wchar_t *table ) { \
					return StringFormat_to_digi_signed( buf, max_digi, value, base, exponent, precision, table ); } \
	//------
	#include "../common/TYPE_LIST_int.h"
#undef axTYPE_LIST

//======== unsigned int
#define	axTYPE_LIST(T)	\
	inline Status StringFormat_out( StringFormat &f, T   value ) { return StringFormat_out_NumberT( f, value ); } \
	inline int StringFormat_to_digi( wchar_t* buf, const int max_digi, T  value, int base, bool exponent, int precision, const wchar_t *table ) { \
					return StringFormat_to_digi_unsigned( buf, max_digi, value, base, exponent, precision, table ); } \
	//---------
	#include "../common/TYPE_LIST_uint.h"
#undef axTYPE_LIST



//@}
} //namespace ax

#endif //__ax_StringFormat_out_Imp_h__


