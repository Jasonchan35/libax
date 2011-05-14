#pragma once
#ifndef __axStringFormat_out_Imp_h__
#define __axStringFormat_out_Imp_h__

#include "axStringFormat_Imp.h"
#include "ax_str_to.h"

//! \ingroup base_string
//@{

template< class T> inline
int axStringFormat_to_digi_signed( char* buf, const int max_digi, T value, int base, bool exponent, int precision, const char *table ) {
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

	char* out = buf + n - 1;

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
int axStringFormat_to_digi_unsigned( char* buf, const int max_digi, T value, int base, bool exponent, int precision, const char *table ) {
	T v = value;
	T b = (T)base;
	int i;

	for( i=0; i<max_digi; ) {
		v /= b;
		i++;
		if( v == 0 ) break;
	}
	char* out = buf + i - 1;

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

inline static
const char * axStringFormat_hexTable( bool upper ) {
	if( upper ) {
		return "0123456789ABCDEF";
	}else{
		return "0123456789abcdef";
	}
}


template< class T > inline
axStatus axStringFormat_out_NumberT( axStringFormat &f, T value ) {
	const int max_digi = 400;

	axStatus st;
	//printf("to_str(int) value=%X opt=%ls\n", value, opt );
	bool	plus_sign	= false;
	bool	align_left  = false;
	bool	exponent	= false;
	int		padding		= 0;
	int		base		= 10; //default is dec
	int		precision   = -1;
	char	fill_ch		= ' ';
	const char *table = axStringFormat_hexTable(false);

	const char* opt = f.opt.c_str();
	if( opt ) {
		axSize len = ax_strlen(opt);
		if( len ) {
			//check last char
			bool done = false;
			do {
				char last_ch = opt[ len-1 ];
				switch( last_ch ) {
					case 'c': f.out( (wchar_t)value ); return 0; // character
					case 'b': len--; base = 2;  break; //binary
					case 'o': len--; base = 8;  break; //octer
					case 'd': len--; base = 10; break; //decmial
					case 'x': len--; base = 16; break; //hex
					case 'X': len--; base = 16; table =  axStringFormat_hexTable(true); break; //upper HEX

					case 'e': len--; exponent = true; break; //for floating point only
					default : done = true;
				}
			}while( !done );

			const char *p = opt;
			switch( *p ) {
				case '+': p++; len--; plus_sign  = true; break;
				case '-': p++; len--; align_left = true; break;
				case '0': p++; len--; fill_ch = '0';	 break;
			}

			axStringA_<64>	tmp;
			st = tmp.set( p, len );			if( !st ) return st;

			char* dot = (char*) ax_strchr( tmp, L'.' );
			if( dot ) {
				*dot = 0;
				st = str_to( dot+1, precision );	if( !st ) return st;
			}

			str_to( tmp, padding );
		}
	}

	//printf("opt plus_sign=%s, padding=%d, ch_base=%c\n", plus_sign?"true":"false", padding, ch_base );
	char ch[ max_digi+1 ];
	ch[ max_digi ] = 0;
	int digi = 0;

	digi = axStringFormat_to_digi( ch, max_digi, value, base, exponent, precision, table );
	if( digi < 0 ) { assert(false); return -1; }

	int space = 0;

	if( ! axTypeOf<T>::isUnsigned() && ax_lessThan0( value ) ) {
		plus_sign = false;
	}

	int req_size = digi;
	if( plus_sign ) req_size++;
	if( req_size < padding ) {
		space = padding - req_size;
	}

	if( ! align_left ) { //fill space at the front
		st = f.repeat( fill_ch, space );	if( !st ) return st;
	}

	if( plus_sign ) {
		st = f.out( '+' );	if( !st ) return st;
	}

	st = f.out( ch,digi );	if( !st ) return st;

	if( align_left ) { //fill space at the end
		st = f.repeat( fill_ch, space );	if( !st ) return st;
	}

	//printf("\n str = [%ls] \n\n", str.sz() );
	return 0;
}



//pointer
inline
axStatus axStringFormat_out( axStringFormat &f, const void* p ) {
	f.opt.set( "X" );
#ifdef axCPU_LP32
	return axStringFormat_out_NumberT( f, (uint32_t)(uintptr_t)p );
#elif axCPU_LP64
	return axStringFormat_out_NumberT( f, (uint64_t)(uintptr_t)p );
#else
	#error
#endif
}

//const char* string
inline axStatus axStringFormat_out( axStringFormat &f, const char*  src ) {
	if( !src ) return 0;

	axStatus st;

	int ret;
	axSize src_len;
	st = ax_safe_assign( src_len, ax_strlen(src) );		if( !st ) return st;

	const int	buf_size = 128;
	wchar_t		buf[buf_size];
	axSize w=0, m=0;

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
inline axStatus axStringFormat_out( axStringFormat &f, const wchar_t*  value ) {
	f.out( value );
	return 0;
}

//==== signed int
#define	axTYPE_LIST(T)	\
	inline axStatus axStringFormat_out( axStringFormat &f, T   value ) { return axStringFormat_out_NumberT( f, value ); } \
	inline int axStringFormat_to_digi( char* buf, const int max_digi, T  value, int base, bool exponent, int precision, const char *table ) { \
					return axStringFormat_to_digi_signed( buf, max_digi, value, base, exponent, precision, table ); } \
	//------
	axTYPE_LIST( int8_t )
	axTYPE_LIST( int16_t )
	axTYPE_LIST( int32_t )
	axTYPE_LIST( int64_t )
#undef axTYPE_LIST

//======== unsigned int
#define	axTYPE_LIST(T)	\
	inline axStatus axStringFormat_out( axStringFormat &f, T   value ) { return axStringFormat_out_NumberT( f, value ); } \
	inline int axStringFormat_to_digi( char* buf, const int max_digi, T  value, int base, bool exponent, int precision, const char *table ) { \
		return axStringFormat_to_digi_unsigned( buf, max_digi, value, base, exponent, precision, table ); \
	} \
	//---------
	axTYPE_LIST( uint8_t )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )
	axTYPE_LIST( uint64_t )
#undef axTYPE_LIST

inline axStatus axStringFormat_out( axStringFormat &f, float   value ) { return axStringFormat_out_NumberT( f, value ); }
inline axStatus axStringFormat_out( axStringFormat &f, double  value ) { return axStringFormat_out_NumberT( f, value ); }

template< class T> inline
int axStringFormat_to_digi_floating( char* buf, const int max_digi, T value, int base, bool exponent, int precision, const char *table ) {
	if( base != 10 ) return -100;

	char	fmt[64 + 1];
	fmt[64] = 0;
	char type = exponent ? 'e' : 'f';

#ifdef axCOMPILER_VC
	if( precision < 0 ) {
		_snprintf( fmt, 64, "%%%c", type );
	}else{
		_snprintf( fmt, 64, "%%.%d%c", precision, type );
	}
	int ret = _snprintf( buf, max_digi, fmt, value );

#else
	if( precision < 0 ) {
		snprintf( fmt, 64, "%%%c", type );
	}else{
		snprintf( fmt, 64, "%%.%d%c", precision, type );
	}
	int ret = snprintf( buf, max_digi, fmt, value );

#endif

	if( ret >= max_digi ) return -10;
	return ret;
}

inline int axStringFormat_to_digi( char* buf, const int max_digi, float  value, int base, bool exponent, int precision, const char *table ) {
	return axStringFormat_to_digi_floating( buf, max_digi, value, base, exponent, precision, table );
}
inline int axStringFormat_to_digi( char* buf, const int max_digi, double  value, int base, bool exponent, int precision, const char *table ) {
	return axStringFormat_to_digi_floating( buf, max_digi, value, base, exponent, precision, table );
}

inline axStatus axStringFormat_out( axStringFormat &f, bool value ) {
	return (value) ? f.out("true") : f.out("false");
}

inline axStatus axStringFormat_out( axStringFormat &f, char		value ) { return f.out( value ); }
inline axStatus axStringFormat_out( axStringFormat &f, wchar_t	value ) { return f.out( value ); }

//@}

#endif //__axStringFormat_out_Imp_h__


