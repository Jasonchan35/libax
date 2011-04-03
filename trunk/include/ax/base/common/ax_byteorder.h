#ifndef __ax_byteorder_h__
#define __ax_byteorder_h__

#include "axTypeOf.h"

inline int8_t  ax_byte_swap( int8_t  x ) { return x; }
inline int16_t ax_byte_swap( int16_t x ) { return (x>>8) | (x<<8); }
inline int32_t ax_byte_swap( int32_t x ) { 
	x= ((x<< 8)&0xFF00FF00UL) | ((x>> 8)&0x00FF00FFUL); 
	return (x>>16) | (x<<16); 
}

inline int64_t ax_byte_swap( int64_t x ) {
	x= ((x<< 8)&0xFF00FF00FF00FF00ULL) | ((x>> 8)&0x00FF00FF00FF00FFULL);
	x= ((x<<16)&0xFFFF0000FFFF0000ULL) | ((x>>16)&0x0000FFFF0000FFFFULL);
	return (x>>32) | (x<<32);
}

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


#ifdef axCPU_LITTLE_ENDIAN
// ---- host to big endian
inline	int8_t		ax_host_to_be( int8_t   v ) { return ax_byte_swap(v); }
inline	int16_t		ax_host_to_be( int16_t  v ) { return ax_byte_swap(v); }
inline	int32_t		ax_host_to_be( int32_t  v ) { return ax_byte_swap(v); }
inline	int64_t		ax_host_to_be( int64_t  v ) { return ax_byte_swap(v); }

inline	uint8_t		ax_host_to_be( uint8_t  v ) { return ax_byte_swap(v); }
inline	uint16_t	ax_host_to_be( uint16_t v ) { return ax_byte_swap(v); }
inline	uint32_t	ax_host_to_be( uint32_t v ) { return ax_byte_swap(v); }
inline	uint64_t	ax_host_to_be( uint64_t v ) { return ax_byte_swap(v); }

// ---- host to little endian
inline	int8_t		ax_host_to_le( int8_t   v ) { return v; }
inline	int16_t		ax_host_to_le( int16_t  v ) { return v; }
inline	int32_t		ax_host_to_le( int32_t  v ) { return v; }
inline	int64_t		ax_host_to_le( int64_t  v ) { return v; }

inline	uint8_t		ax_host_to_le( uint8_t  v ) { return v; }
inline	uint16_t	ax_host_to_le( uint16_t v ) { return v; }
inline	uint32_t	ax_host_to_le( uint32_t v ) { return v; }
inline	uint64_t	ax_host_to_le( uint64_t v ) { return v; }

#endif //axCPU_LITTLE_ENDIAN


#ifdef axCPU_BIG_ENDIAN
// ---- host to big endian
inline	int8_t		ax_host_to_be( int8_t   v ) { return v; }
inline	int16_t		ax_host_to_be( int16_t  v ) { return v; }
inline	int32_t		ax_host_to_be( int32_t  v ) { return v; }
inline	int64_t		ax_host_to_be( int64_t  v ) { return v; }

inline	uint8_t		ax_host_to_be( uint8_t  v ) { return v; }
inline	uint16_t	ax_host_to_be( uint16_t v ) { return v; }
inline	uint32_t	ax_host_to_be( uint32_t v ) { return v; }
inline	uint64_t	ax_host_to_be( uint64_t v ) { return v; }

// ---- host to little endian
inline	int8_t		ax_host_to_le( int8_t   v ) { return ax_byte_swap(v); }
inline	int16_t		ax_host_to_le( int16_t  v ) { return ax_byte_swap(v); }
inline	int32_t		ax_host_to_le( int32_t  v ) { return ax_byte_swap(v); }
inline	int64_t		ax_host_to_le( int64_t  v ) { return ax_byte_swap(v); }

inline	uint8_t		ax_host_to_le( uint8_t  v ) { return ax_byte_swap(v); }
inline	uint16_t	ax_host_to_le( uint16_t v ) { return ax_byte_swap(v); }
inline	uint32_t	ax_host_to_le( uint32_t v ) { return ax_byte_swap(v); }
inline	uint64_t	ax_host_to_le( uint64_t v ) { return ax_byte_swap(v); }
#endif //axCPU_LITTLE_ENDIAN


#endif //__ax_byteorder_h__