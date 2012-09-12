#pragma once
#ifndef __axStringFormat_out_Imp_h__
#define __axStringFormat_out_Imp_h__

#include "axStringFormat.h"
#include "ax_str_to.h"

//! \ingroup base_string
//@{

inline axStatus	axStringFormat_out( axStringFormat &f, axStatus value ) { return f.format("({?}:{?})", value.code(), value.c_str() ); }

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

template< class T > axStatus axStringFormat_out_NumberT( axStringFormat &f, T value );

//pointer
inline
axStatus axStringFormat_out( axStringFormat &f, const void* p ) {
#ifdef axCPU_LP32
	f.opt.set( "08X" );
	return axStringFormat_out_NumberT( f, (uint32_t)(uintptr_t)p );
#elif axCPU_LP64
	f.opt.set( "016X" );
	return axStringFormat_out_NumberT( f, (uint64_t)(uintptr_t)p );
#else
	#error
#endif
}

//C string
template< class T >
inline axStatus axStringFormat_out_c_str( axStringFormat &f, const T*  value ) {
	if( ! value ) return 0;
	
	size_t	n = ax_strlen( value );
	if( f.opt.isEmpty() ) {
		f.put( value );			
	}else{
		bool  align_left = false;
		const char* opt = f.opt.c_str();
		if( opt[0] == '-' ) {
			opt++; 
			align_left = true; 
		}
		axSize	padding = 0;
		ax_str_to( opt, padding );
		
		if( !align_left && padding > n ) f.repeat( ' ', padding-n );		
		f.put( value );	
		if( align_left  && padding > n ) f.repeat( ' ', padding-n );
	}	
	return 0;
}

inline axStatus axStringFormat_out( axStringFormat &f, const char*		value ) { return axStringFormat_out_c_str( f, value ); }
inline axStatus axStringFormat_out( axStringFormat &f, const wchar_t*	value ) { return axStringFormat_out_c_str( f, value ); }

template< class T > inline 
axStatus axStringFormat_out_char( axStringFormat &f, T value ) {
	size_t	n = 1;
	if( f.opt.isEmpty() ) {
		f.put( value );			
	}else{
		bool  align_left = false;
		const char* opt = f.opt.c_str();
		if( opt[0] == '-' ) {
			opt++; 
			align_left = true;
		}
		axSize	padding = 0;
		ax_str_to( opt, padding );
		
		if( !align_left && padding > n ) f.repeat( ' ', padding-n );		
		f.put( value );	
		if( align_left  && padding > n ) f.repeat( ' ', padding-n );
	}		
	return 0;
}

inline axStatus axStringFormat_out( axStringFormat &f, wchar_t	value ) { return axStringFormat_out_char( f, value ); }
inline axStatus axStringFormat_out( axStringFormat &f, char		value ) { return axStringFormat_out_char( f, value ); }



//==== signed int
#define	axTYPE_LIST(T)	\
	inline axStatus axStringFormat_out( axStringFormat &f, T   value ) { return axStringFormat_out_NumberT( f, value ); } \
	inline int axStringFormat_to_digi( char* buf, const int max_digi, T  value, int base, bool exponent, int precision, const char *table ) { \
					return axStringFormat_to_digi_signed( buf, max_digi, value, base, exponent, precision, table ); } \
	//------

#include "../common/axTypeList_int.h"
#undef axTYPE_LIST

//======== unsigned int
#define	axTYPE_LIST(T)	\
	inline axStatus axStringFormat_out( axStringFormat &f, T   value ) { return axStringFormat_out_NumberT( f, value ); } \
	inline int axStringFormat_to_digi( char* buf, const int max_digi, T  value, int base, bool exponent, int precision, const char *table ) { \
		return axStringFormat_to_digi_unsigned( buf, max_digi, value, base, exponent, precision, table ); \
	} \
	//---------
#include "../common/axTypeList_uint.h"
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
	return (value) ? f.put("true") : f.put("false");
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
	const char *table = ax_HEX_chars();
    
	const char* opt = f.opt.c_str();
	axSize len = ax_strlen(opt);
	if( len ) {
		//check last char
		char last_ch = opt[ len-1 ];
		switch( last_ch ) {
			case 'c': len--; base = 0;  break; //character
			case 'b': len--; base = 2;  break; //binary
			case 'o': len--; base = 8;  break; //octer
			case 'd': len--; base = 10; break; //decmial
			case 'X': len--; base = 16; break; //hex
			case 'x': len--; base = 16; break; table =  ax_hex_chars(); break; //lower HEX
				
			case 'e': len--; exponent = true; break; //for floating point only
		}
		
		const char *p = opt;
		switch( *p ) {
			case '+': p++; len--; plus_sign  = true; break;
			case '-': p++; len--; align_left = true; break;
			case '0': p++; len--; fill_ch = '0';	 break;
		}
		
		axStringA_<64>	tmp;
		st = tmp.setWithLength( p, len );			if( !st ) return st;
		
		char* dot = (char*) ax_strchr( tmp.c_str(), '.' );
		if( dot ) {
			*dot = 0;
			st = ax_str_to( dot+1, precision );	if( !st ) return st;
		}
		
		ax_str_to( tmp.c_str(), padding );
	}
    
	//printf("opt plus_sign=%s, padding=%d, ch_base=%c\n", plus_sign?"true":"false", padding, ch_base );
	char ch[ max_digi+1 ];
	ch[ max_digi ] = 0;
	int digi = 0;
	int space = 0;
    
	if( base == 0 ) {
		int utf8_len = ax_wchar_to_utf8( ch, max_digi, (wchar_t)value );
		if( utf8_len <= 0 ) { assert(false); return -1; }
		ch[utf8_len] = 0 ;
		plus_sign = false;
	}else{
		digi = axStringFormat_to_digi( ch, max_digi, value, base, exponent, precision, table );
		if( digi < 0 ) { assert(false); return -1; }        
		if( ! axTypeOf<T>::isUnsigned && ax_less_than0( value ) ) {
			plus_sign = false;
		}
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
		st = f.put( '+' );	if( !st ) return st;
	}
    
	st = f.put( ch,digi );	if( !st ) return st;
    
	if( align_left ) { //fill space at the end
		st = f.repeat( fill_ch, space );	if( !st ) return st;
	}
    
	//printf("\n str = [%ls] \n\n", str.sz() );
	return 0;
}

//@}

#endif //__axStringFormat_out_Imp_h__


