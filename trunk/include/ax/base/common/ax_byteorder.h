#ifndef __ax_byteorder_h__
#define __ax_byteorder_h__

#include "axTypeOf.h"

inline uint8_t  ax_byte_swap( uint8_t  x ) { return x; }
inline uint16_t ax_byte_swap( uint16_t x ) { return (x>>8) | (x<<8); }
inline uint32_t ax_byte_swap( uint32_t x ) {
	x= ((x<< 8)&0xFF00FF00UL) | ((x>> 8)&0x00FF00FFUL);
	return (x>>16) | (x<<16);
}

inline uint64_t ax_byte_swap( uint64_t x ) {
	x= ((x<< 8)&0xFF00FF00FF00FF00ULL) | ((x>> 8)&0x00FF00FF00FF00FFULL);
	x= ((x<<16)&0xFFFF0000FFFF0000ULL) | ((x>>16)&0x0000FFFF0000FFFFULL);
	return (x>>32) | (x<<32);
}

inline int8_t   ax_byte_swap( int8_t  x ) { return (int8_t ) ax_byte_swap( (uint8_t )x ); }
inline int16_t  ax_byte_swap( int16_t x ) { return (int16_t) ax_byte_swap( (uint16_t)x ); }
inline int32_t  ax_byte_swap( int32_t x ) { return (int32_t) ax_byte_swap( (uint32_t)x ); }
inline int64_t  ax_byte_swap( int64_t x ) { return (int64_t) ax_byte_swap( (uint64_t)x ); }

inline float    ax_byte_swap( float   x ) { 
	float *p = &x;
	uint32_t t = ax_byte_swap( *reinterpret_cast<uint32_t*>(p) );
	uint32_t *q = &t;
	return *reinterpret_cast<float*>(q);
}

inline double   ax_byte_swap( double  x ) { 
	double *p = &x;
	uint64_t t = ax_byte_swap( *reinterpret_cast<uint64_t*>(p) ); 
	uint64_t *q = &t;
	return *reinterpret_cast<double*>(q); 
}

#ifdef axCPU_LITTLE_ENDIAN
	#define ax_char4( a,b,c,d ) ( a | b<<8 | c<<16 | d<<24 )
	#define	axBYTE_ORDER	4321
	#define axTYPE_LIST(T) \
		inline T ax_host_to_be( T v ) { return ax_byte_swap(v); } \
		inline T ax_host_to_le( T v ) { return v; } \
		inline T ax_be_to_host( T v ) { return ax_byte_swap(v); } \
		inline T ax_le_to_host( T v ) { return v; } \
	//--
		axTYPE_LIST( int8_t )
		axTYPE_LIST( int16_t )
		axTYPE_LIST( int32_t )
		axTYPE_LIST( int64_t )
		axTYPE_LIST( uint8_t )
		axTYPE_LIST( uint16_t )
		axTYPE_LIST( uint32_t )
		axTYPE_LIST( uint64_t )
		axTYPE_LIST( float )
		axTYPE_LIST( double )
	#undef axTYPE_LIST
#endif //axCPU_LITTLE_ENDIAN

#ifdef axCPU_BIG_ENDIAN
	#define ax_char4( a,b,c,d ) ( a<<24 | b<<16 | c<<8 | d )
	#define	axBYTE_ORDER	1234
	#define axTYPE_LIST(T) \
		inline T ax_host_to_be( T v ) { return v; } \
		inline T ax_host_to_le( T v ) { return ax_byte_swap(v); } \
		inline T ax_be_to_host( T v ) { return v; } \
		inline T ax_le_to_host( T v ) { return ax_byte_swap(v); } \
	//--
		axTYPE_LIST( int8_t )
		axTYPE_LIST( int16_t )
		axTYPE_LIST( int32_t )
		axTYPE_LIST( int64_t )
		axTYPE_LIST( uint8_t )
		axTYPE_LIST( uint16_t )
		axTYPE_LIST( uint32_t )
		axTYPE_LIST( uint64_t )
		axTYPE_LIST( float )
		axTYPE_LIST( double )
	#undef axTYPE_LIST
#endif //axCPU_LITTLE_ENDIAN


#endif //__ax_byteorder_h__
