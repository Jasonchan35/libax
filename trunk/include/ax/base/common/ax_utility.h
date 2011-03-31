#pragma once
#ifndef __ax_utility_h__
#define __ax_utility_h__

#include "ax_math_template.h"
#include "axSize.h"

//! \ingroup base_common
//@{

#define	axPRINT_FUNC_NAME	printf("FUNC [%s]\n", axFUNC_NAME );

inline void toggle_bool( bool &b ) { b = !b; }

//--- 32 bits ---
inline bool has_any_bit	( uint32_t value, uint32_t bit )			{ return (value & bit) != 0;   }
inline bool has_all_bit	( uint32_t value, uint32_t bit )			{ return (value & bit) == bit; }

inline void set_bit		( uint32_t &value, uint32_t bit )			{ value |= bit; }
inline void unset_bit	( uint32_t &value, uint32_t bit )			{ value &= ~bit; }
inline void toggle_bit	( uint32_t &value, uint32_t bit )			{ value ^= bit; }
inline void set_bit		( uint32_t &value, uint32_t bit, bool b )	{ if( b ) set_bit ( value, bit ); else unset_bit( value, bit ); }

//--- 16 bits ---
inline bool has_any_bit	( uint16_t value, uint16_t bit )			{ return (value & bit) != 0;   }
inline bool has_all_bit	( uint16_t value, uint16_t bit )			{ return (value & bit) == bit; }

inline void set_bit		( uint16_t &value, uint16_t bit )			{ value |= bit; }
inline void unset_bit	( uint16_t &value, uint16_t bit )			{ value &= ~bit; }
inline void toggle_bit	( uint16_t &value, uint16_t bit )			{ value ^= bit; }
inline void set_bit		( uint16_t &value, uint16_t bit, bool b )	{ if( b ) set_bit ( value, bit ); else unset_bit( value, bit ); }

//--- 8 bits ---
inline bool has_any_bit	( uint8_t value, uint8_t bit )				{ return (value & bit) != 0;   }
inline bool has_all_bit	( uint8_t value, uint8_t bit )				{ return (value & bit) == bit; }

inline void set_bit		( uint8_t &value, uint8_t bit )				{ value |= bit; }
inline void unset_bit	( uint8_t &value, uint8_t bit )				{ value &= ~bit; }
inline void toggle_bit	( uint8_t &value, uint8_t bit )				{ value ^= bit; }
inline void set_bit		( uint8_t &value, uint8_t bit, bool b )		{ if( b ) set_bit ( value, bit ); else unset_bit( value, bit ); }


using ::memset;
template< class T> void memset( T &o, int value ) { ::memset( &o, value, sizeof(o) ); }

inline
void dumpHexRaw( const void* buf, axSize len, FILE* s = stdout ) {
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


inline	void*	ax_malloc( size_t n )	{ return ::malloc(n); }
inline	void	ax_free	( void* p  )	{ return ::free(p); }

#endif //__ax_utility_h__
