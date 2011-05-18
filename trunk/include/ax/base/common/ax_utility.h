#pragma once
#ifndef __ax_utility_h__
#define __ax_utility_h__

#include "ax_math_template.h"
#include "axSize.h"

//! \ingroup base_common
//@{

#define	axPRINT_FUNC_NAME	printf("FUNC [%s]\n", axFUNC_NAME );

inline void ax_toggle( bool &b ) { b = !b; }

//--- 32 bits ---

#define	axTYPE_LIST( T ) \
	inline void ax_set_bits		( T &value, T bits )			{ value |= bits; }	\
	inline void ax_unset_bits	( T &value, T bits )			{ value &= ~bits; }	\
	inline void ax_set_bits		( T &value, T bits, bool b )	{ if( b ) ax_set_bits ( value, bits ); else ax_unset_bits( value, bits ); }	\
	inline void ax_toggle_bits	( T &value, T bits )			{ value ^= bits; }	\
	inline bool ax_any_bit_on	( T value,  T bits )			{ return (value & bits) != 0;   }	\
	inline bool ax_all_bits_on	( T value,  T bits )			{ return (value & bits) == bits; }	\
//-----
	axTYPE_LIST( uint64_t );
	axTYPE_LIST( uint32_t );
	axTYPE_LIST( uint16_t );
	axTYPE_LIST( uint8_t  );
#undef axTYPE_LIST

template< class T> void ax_memset( T &o, int value ) { ::memset( &o, value, sizeof(o) ); }

inline
void ax_dumpBufferHex( const void* buf, axSize len, FILE* s = stdout ) {
	const char *non_print = "\t\n\r";

	axSize i;
	const uint8_t *c = (const uint8_t*)buf;

#ifdef axCPU_LP32
	fprintf( s, "----------- dump hex len=%d ------------", len.native() );
#elif axCPU_LP64
	fprintf( s, "----------- dump hex len=%lld ------------", len.native() );
#else
	#error
#endif

	while( len ) {
		axSize n = ax_min( len, axSize(16) );
//print address
		fprintf(s, "\n%p: ", c );
//print Hex
		for( i=0; i<16; i++ ) {
			if( i == 8 ) fprintf(s,"  ");
			if( i < n ) {
				fprintf(s, "%02X ", c[i] );
			}else{
				fprintf(s, "xx " );
			}
		}
		fprintf(s," ");
//print Character
		for( i=0; i<16; i++ ) {
			if( i < n && !strchr( non_print, c[i] ) ) {
				fprintf(s, "%c", c[i] );
			}else{
				fprintf(s, "." );
			}
		}
		c += n;
		len -= n;
	}
	fprintf(s, "\n\n");
}

inline void*	ax_malloc( size_t n )	{ return ::malloc(n); }
inline void		ax_free	( void* p  )	{ return ::free(p); }

#ifdef axOS_WIN
	inline void ax_sleep( uint32_t seconds )	{ Sleep( seconds * 1000 ); }
	inline void ax_sleep( double   seconds )	{ if( seconds <= 0 ) return; Sleep( (DWORD)(seconds * 1000.0 ) ); }

#endif //axOS_WIN

#ifdef axOS_UNIX
	inline void ax_sleep( uint32_t seconds )	{ sleep( seconds ); }
	inline void ax_sleep( double   seconds )	{
		if( seconds <= 0 ) return;
		if( seconds <= 10.0 ) {
			usleep( (useconds_t)( seconds * 1000000) );
		}else{
			double &int_part;
			double f = modf( seconds, int_part );
			sleep( int_part );
			usleep( (useconds_t)( f * 1000000) );
		}
	}
#endif //axOS_UNIX

#endif //__ax_utility_h__
