#pragma once
#ifndef __ax_math_template_h__
#define __ax_math_template_h__

#include "axTypeOf.h"

//! \ingroup base_common
//@{

template<class T> inline void ax_swap( T &a, T &b )			{ T tmp; ax_takeOwnership(tmp,a); ax_takeOwnership(a,b); ax_takeOwnership(b,tmp); }

template<class T> inline T ax_sign( T a )					{ if( a == 0 ) return 0; return ax_lessThan0(a) ? -1: 1; }

//-- abs for unsigned
#define	axTYPE_LIST(T)\
	inline T	ax_abs( T  a ) { return a; }
//-----
	axTYPE_LIST( uint8_t )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )
	axTYPE_LIST( uint64_t )
#undef axTYPE_LIST

template<class T> inline T ax_abs ( T a );

template<class T> inline T ax_min ( T a, T b )						{ return (a<b)?a:b; }
template<class T> inline T ax_max ( T a, T b )						{ return (a>b)?a:b; }

template<class T> inline void ax_min_it		( T& a, const T& b )	{ if( b<a ) a=b; }
template<class T> inline void ax_max_it		( T& a, const T& b )	{ if( b>a ) a=b; }

//! x clamped to the range [a,b]
template<class T> inline T	  ax_clamp		( T  x, T a, T b )		{ if( x < a ) return a; return x > b ? b : x; }
template<class T> inline void ax_clamp_it	( T &x, T a, T b )		{ if( x < a ) x=a; else if( x > b ) x=b; }

//! align multiple
template<class T> inline T ax_align_multiple( T n, T a ) {  
	T r = n % a;	
	if( n > 0 ) { 
		return n + ax_abs(a) -r; 
	}else{ 
		return n - ax_abs(a) -r; 
	} 
}

inline	int  ax_align_pow_of_2( int v ) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4;	v |= v >> 8; v |= v >> 16; v++;	return v; }

//@}
//@}

#endif //__ax_math_template_h__

