#pragma once
#ifndef __ax_math_template_h__
#define __ax_math_template_h__

namespace ax {
//! \addtogroup common
//@{


template<class T> inline void swap( T &a, T &b )			{ T tmp; takeOwnership(tmp,a); takeOwnership(a,b); takeOwnership(b,tmp); }

template<class T> inline T sign( T a )						{ if( a == 0 ) return 0; return (a<0) ? -1: 1; }
template<class T> inline T abs ( T a )						{ return (a>0)?a:-a; }

template<class T> inline T min ( T a, T b )					{ return (a<b)?a:b; }
template<class T> inline T min ( T a, T b, T c )			{ return min( (a<b)?a:b, c ); }
template<class T> inline T max ( T a, T b )					{ return (a>b)?a:b; }
template<class T> inline T max ( T a, T b, T c )			{ return max( (a>b)?a:b, c ); }

template<class T> inline void minIt		( T& a, const T& b )	{ if( b<a ) a=b; }
template<class T> inline void maxIt		( T& a, const T& b )	{ if( b>a ) a=b; }

//! x clamped to the range [a,b]
template<class T> inline T	  clamp		( T  x, T a, T b )		{ if( x < a ) return a; return x > b ? b : x; }
template<class T> inline void clampIt	( T &x, T a, T b )		{ if( x < a ) x=a; else if( x > b ) x=b; }

//! align multiple
template<class T> inline T alignMultiple( T n, T a ) {  T r = n % a;	if( n > 0 ) { return n +abs(a) -r; }else{ return n -abs(a) -r; } }

inline	int  alignPowOf2( int v ) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4;	v |= v >> 8; v |= v >> 16; v++;	return v; }


//@}
} //namespace ax

#endif //__ax_math_template_h__