#pragma once
#ifndef __ax_math_template_h__
#define __ax_math_template_h__

#include "axTypeOf.h"

//! \ingroup base_common
//@{

template<class T> inline void	ax_swap( T &a, T &b )		{ T tmp; ax_take(tmp,a); ax_take(a,b); ax_take(b,tmp); }

template<class T> inline T ax_sign( T a )					{ if( a == 0 ) return 0; return ax_less_than0(a) ? -1: 1; }

//-- abs for unsigned
#define	axTYPE_LIST(T)\
	inline T	ax_abs( T  a ) { return a; }
//-----
	axTYPE_LIST( uint8_t  )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )
	axTYPE_LIST( uint64_t )
#undef axTYPE_LIST

template<class T> inline T ax_abs ( T a )       { return a>0 ? a : -a ; } 

template<class T> inline T ax_min ( T a, T b )						{ return (a<b)?a:b; }
template<class T> inline T ax_max ( T a, T b )						{ return (a>b)?a:b; }

template<class T> inline void ax_min_it		( T& a, const T& b )	{ if( b<a ) a=b; }
template<class T> inline void ax_max_it		( T& a, const T& b )	{ if( b>a ) a=b; }

//! x clamped to the range [a,b]
template<class T> inline T      ax_clamp	( T  x, T a, T b )		{ if( x < a ) return a; return x > b ? b : x; }
template<class T> inline void   ax_clamp_it	( T &x, T a, T b )		{ if( x < a ) x=a; else if( x > b ) x=b; }

inline	int  ax_align_pow2( int v ) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4;	v |= v >> 8; v |= v >> 16; v++;	return v; }
inline	bool ax_is_pow2( int v )	{ return !(v & (v - 1)) && v ; }


template<class T>	T ax_fp_epsilon();

template<>	inline double	ax_fp_epsilon<double>() { return 1.0e-12; }
template<>	inline float	ax_fp_epsilon<float>()  { return 1.0e-5f; }

const  double ax_math_PI = 3.14159265358979323846;

//Splits a floating-point value into fractional and integer parts
inline float  ax_modf( float  n, float  *i ) { return ::modff( n, i ); }
inline double ax_modf( double n, double *i ) { return ::modf ( n, i ); }

inline float  ax_modf( float  n )	{ float  i; return ::modff( n, &i ); }
inline double ax_modf( double n )	{ double i; return ::modf ( n, &i ); }

inline float  ax_sqrt( float  n )	{ return ::sqrtf( n ); }
inline double ax_sqrt( double n )	{ return ::sqrt ( n ); }

//! reciprocal square root
inline float  ax_rsqrt( float  n )	{ return 1.0f/::sqrtf( n ); }
inline double ax_rsqrt( double n )	{ return 1.0 /::sqrt ( n ); }

inline float  ax_ceil( float  a )	{ return ::ceilf(a); }
inline double ax_ceil( double a )	{ return ::ceil(a); }

inline float  ax_floor( float  a )	{ return ::floorf(a); }
inline double ax_floor( double a )	{ return ::floor(a); }

//round up
inline float  ax_round( float a  )	{ return ax_floor( a > 0 ? a+0.5f : a-0.5f ); }
inline double ax_round( double a )	{ return ax_floor( a > 0 ? a+0.5  : a-0.5  ); }

//! Equivalent
template<class T>
inline bool ax_math_equals( T  a, T  b, T ep=ax_fp_epsilon<T>() )  { return ( ax_abs(a-b) < ep) ; }

//! increase but not over the limit
template<class T>
inline void ax_step_inc_to( T &value, T limit, T step ) {
	if( value < limit ) {
		T v = value + step;
		value = v > limit ? limit : v;
	}
	if( value > limit ) {
		T v = value - step;
		value = v < limit ? limit : v;
	}
}

//! linear interpolation out = a+w*(b-a)

template<class T> T inline _ax_int_lerp( T a, T b, float  w ) { return (T)( a+w*( b - a) + 0.5f ); }
template<class T> T inline _ax_int_lerp( T a, T b, double w ) { return (T)( a+w*( b - a) + 0.5f ); }

inline uint8_t	ax_lerp( uint8_t  a, uint8_t  b, float  w ) { return _ax_int_lerp(a,b,w); }
inline uint16_t	ax_lerp( uint16_t a, uint16_t b, float  w ) { return _ax_int_lerp(a,b,w); }
inline uint32_t	ax_lerp( uint32_t a, uint32_t b, float  w ) { return _ax_int_lerp(a,b,w); }
inline uint64_t	ax_lerp( uint64_t a, uint64_t b, float  w ) { return _ax_int_lerp(a,b,w); }

inline int8_t	ax_lerp( int8_t   a, int8_t   b, float  w ) { return _ax_int_lerp(a,b,w); }
inline int16_t	ax_lerp( int16_t  a, int16_t  b, float  w ) { return _ax_int_lerp(a,b,w); }
inline int32_t	ax_lerp( int32_t  a, int32_t  b, float  w ) { return _ax_int_lerp(a,b,w); }
inline int64_t	ax_lerp( int64_t  a, int64_t  b, float  w ) { return _ax_int_lerp(a,b,w); }

inline float	ax_lerp( float    a, float    b, float  w ) { return (a+w*(b-a)); }
inline double	ax_lerp( double   a, double   b, double w ) { return (a+w*(b-a)); }

inline float	ax_step( float 	a, float  x ) 	{ return x>=a?1:0; }
inline double	ax_step( double a, double x ) 	{ return x>=a?1:0; }

inline float	ax_ease_in ( float  a ) { return a*a; }	
inline double	ax_ease_in ( double a ) { return a*a; }	
inline float	ax_ease_out( float  a ) { a=1-a; return 1-a*a; }	
inline double	ax_ease_out( double a ) { a=1-a; return 1-a*a; }	
	
inline float	ax_smooth_step( float  w ) { return (-2.0f*w*w*w + 3.0f*w*w); }
inline double	ax_smooth_step( double w ) { return (-2.0 *w*w*w + 3.0 *w*w); }

template<class T> inline T ax_smooth_step( const T &a, const T &b, float  w ) { return ax_lerp(a,b,ax_smooth_step(w) ); }
template<class T> inline T ax_smooth_step( const T &a, const T &b, double w ) { return ax_lerp(a,b,ax_smooth_step(w) ); }


//! align multiple
template<class T> inline T ax_align_multiple( T n, T a ) {  
	T r = n % a;
	if( r == 0 ) return n;
	if( n > 0 ) { 
		return n + ax_abs(a) -r; 
	}else{ 
		return n - ax_abs(a) -r; 
	} 
}

//! floating-point align
inline
float  ax_align_multiple( float   n, float   a ) {
	float i = ax_floor( n / a ) * a;
	if( i == n ) return i;
	return (n > 0) ? i+a : i-a;
}

inline
double ax_align_multiple( double  n, double  a ) {
	double i = ax_floor( n / a ) * a;
	if( i == n ) return i;
	return (n > 0) ? i+a : i-a;
}

inline float  ax_remainder( float  a, float  b ) { return ( a - ax_floor(a/b) * b ); }
inline double ax_remainder( double a, double b ) { return ( a - ax_floor(a/b) * b ); }

inline float  ax_cos( float  a ) { return ::cosf(a); }
inline double ax_cos( double a ) { return ::cos (a); }

inline float  ax_sin( float  a ) { return ::sinf(a); }
inline double ax_sin( double a ) { return ::sin (a); }

inline float  ax_atan2( float  a, float  b ) { return ::atan2f(a,b); }
inline double ax_atan2( double a, double b ) { return ::atan2 (a,b); }

//@}

#endif //__ax_math_template_h__

