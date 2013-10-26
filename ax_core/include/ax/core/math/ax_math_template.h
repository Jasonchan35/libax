#pragma once
#ifndef __ax_math_template_h__
#define __ax_math_template_h__

#include "../common/axSize.h"

//! \ingroup base_common
//@{

template<class T> inline void	ax_swap( T &a, T &b )		{ T tmp; ax_take(tmp,a); ax_take(a,b); ax_take(b,tmp); }

template<class T> inline T ax_sign( T a )					{ if( a == 0 ) return 0; return ax_less_than0(a) ? -1: 1; }

//-- abs for unsigned
#define	axTYPE_LIST(T)\
	inline T	ax_abs( T  a ) { return a; }
//-----
	#include "../common/axTypeList_uint.h"
#undef axTYPE_LIST

template<class T>	inline T		ax_div			( const T & a, const T & b ) { return ( b==0 ) ? 0 : a/b; }
template<class T>	inline void		ax_div_it		( 	    T & a, const T & b ) {    a = ( b==0 ) ? 0 : a/b; }

template<class T>	inline T		ax_abs			( T a )					{ return a>0 ? a : -a ; } 
template<class T>	inline void		ax_abs_it		( T &a )				{ a = ax_abs(a); }

template<class T>	inline T		ax_min			( T a, T b )			{ return (a<b)?a:b; }
template<class T>	inline T		ax_max			( T a, T b )			{ return (a>b)?a:b; }

template<class T>	inline void		ax_min_it		( T& a, const T& b )	{ a = ax_min(a,b); }
template<class T>	inline void		ax_max_it		( T& a, const T& b )	{ a = ax_max(a,b); }


template<>			inline bool		ax_less_than0	( axSize  value )		{ return false; }
//template<>		inline axSize	ax_min			( axSize a, axSize b )	{ return (a<b)?a:b; }
//template<>		inline axSize	ax_max			( axSize a, axSize b )	{ return (a>b)?a:b; }
template<>			inline axSize	ax_abs			( axSize a )			{ return a; } //unsigned will always be positive

//! x clamped to the range [a,b]
template<class T>	inline T		ax_clamp		( T x, T a, T b )		{ if( x < a ) return a; return x > b ? b : x; }
template<class T>	inline T		ax_clamp01		( T x )					{ return ax_clamp(x, (T)0, (T)1); }

template<class T>	inline void		ax_clamp_it		( T &x, T a, T b )		{ x = ax_clamp(x,a,b); }
template<class T>	inline void		ax_clamp01_it	( T &x )				{ ax_clamp_it(x, (T)0, (T)1); }

inline	int  ax_next_pow2	( int v )	{ v--; v |= v >> 1; v |= v >> 2; v |= v >> 4;	v |= v >> 8; v |= v >> 16; v++;	return v; }
inline	bool ax_is_pow2		( int v )	{ return !(v & (v - 1)) && v ; }

// Not a Number
//template<class T>	T ax_nan();
//template<>	inline double	ax_nan<double>() { return std::quiet_NaN<double>; }
//template<>	inline float	ax_nan<float> () { return std::quiet_NaN<float>; }

//template<class T>	T ax_isnan();
//template<>	inline double	ax_isnan<double>() { return std::nan<double>(); }
//template<>	inline float	ax_isnan<float> () { return std::nan<float>(); }

// Infinity

template <class T> T	ax_inf	()			{ return std::numeric_limits<T>::infinity(); }
template <class T> T	ax_isinf( T value )	{ return std::numeric_limits<T>::has_infinity && value == ax_inf<T>(); }


template<class T>	T		ax_epsilon();
template<>	inline double	ax_epsilon<double>() { return 1.0e-12; }
template<>	inline float	ax_epsilon<float> () { return 1.0e-5f; }

const  double ax_math_PI = 3.14159265358979323846;

//Splits a floating-point value into fractional and integer parts
inline float  ax_modf		( float  n, float  *i ) { return ::modff( n, i ); }
inline double ax_modf		( double n, double *i ) { return ::modf ( n, i ); }

inline float  ax_modf		( float  n )	{ float  i; return ::modff( n, &i ); }
inline double ax_modf		( double n )	{ double i; return ::modf ( n, &i ); }

inline float  ax_sqrt		( float  n )	{ return ::sqrtf( n ); }
inline double ax_sqrt		( double n )	{ return ::sqrt ( n ); }

//! reciprocal square root
inline float  ax_rsqrt		( float  n )	{ return 1.0f/::sqrtf( n ); }
inline double ax_rsqrt		( double n )	{ return 1.0 /::sqrt ( n ); }

inline float  ax_ceil		( float  a )	{ return ::ceilf(a); }
inline double ax_ceil		( double a )	{ return ::ceil (a); }

inline int    ax_ceil_int	( float  a  )	{ return (int)ax_ceil(a); }
inline int	  ax_ceil_int	( double a )	{ return (int)ax_ceil(a); }

inline float  ax_floor		( float  a )	{ return ::floorf(a); }
inline double ax_floor		( double a )	{ return ::floor (a); }

inline int    ax_floor_int	( float  a  )	{ return (int)ax_floor(a); }
inline int	  ax_floor_int	( double a )	{ return (int)ax_floor(a); }

inline float  ax_round		( float  a  )	{ return ::roundf(a); }
inline double ax_round		( double a )	{ return ::round (a); }

inline int    ax_round_int	( float  a  )	{ return (int)ax_round(a); }
inline int	  ax_round_int	( double a )	{ return (int)ax_round(a); }



template<class T> inline void ax_round_it( T &v ) { v = ax_round(v); }
template<class T> inline void ax_floor_it( T &v ) { v = ax_floor(v); }
template<class T> inline void ax_ceil_it ( T &v ) { v = ax_ceil(v); }

//! Equivalent
template<class T>
inline bool ax_math_equals( T  a, T  b, T ep=ax_epsilon<T>() )  { return ( ax_abs(a-b) < ep) ; }

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

inline float	ax_lerp( float    a, float    b, float  w ) { return (1-w)*a + w*b; }
inline double	ax_lerp( double   a, double   b, double w ) { return (1-w)*a + w*b; }

inline float	ax_step( float 	a, float  x ) 	{ return x>=a ? 1.0f : 0.0f; }
inline double	ax_step( double a, double x ) 	{ return x>=a ? 1.0  : 0.0;  }

inline float	ax_ease_in ( float  a ) { return a*a; }	
inline double	ax_ease_in ( double a ) { return a*a; }	
inline float	ax_ease_out( float  a ) { a=1-a; return 1-a*a; }	
inline double	ax_ease_out( double a ) { a=1-a; return 1-a*a; }	
	
inline float	ax_smooth_step( float  w ) { return (-2.0f*w*w*w + 3.0f*w*w); }
inline double	ax_smooth_step( double w ) { return (-2.0 *w*w*w + 3.0 *w*w); }

template<class T> inline T ax_smooth_step( const T &a, const T &b, float  w ) { return ax_lerp(a,b,ax_smooth_step(w) ); }
template<class T> inline T ax_smooth_step( const T &a, const T &b, double w ) { return ax_lerp(a,b,ax_smooth_step(w) ); }


//! align multiple
template<class T> inline 
T _ax_align_multiple_int( T n, T a ) { 
	return n + a -(n%a);
}

inline int8_t	ax_align_multiple( int8_t  n, int8_t  a ) { return _ax_align_multiple_int(n,a); }
inline int16_t	ax_align_multiple( int16_t n, int16_t a ) { return _ax_align_multiple_int(n,a); }
inline int32_t	ax_align_multiple( int32_t n, int32_t a ) { return _ax_align_multiple_int(n,a); }
inline int64_t	ax_align_multiple( int64_t n, int64_t a ) { return _ax_align_multiple_int(n,a); }

template<class T> inline 
T _ax_align_multiple_uint( T n, T a ) {  
	T r = n % a;
	if( n > 0 ) { 
		return n + ax_abs(a) -r; 
	}else{ 
		return n - ax_abs(a) -r; 
	} 
}

inline uint8_t	ax_align_multiple( uint8_t  n, uint8_t  a ) { return _ax_align_multiple_uint(n,a); }
inline uint16_t	ax_align_multiple( uint16_t n, uint16_t a ) { return _ax_align_multiple_uint(n,a); }
inline uint32_t	ax_align_multiple( uint32_t n, uint32_t a ) { return _ax_align_multiple_uint(n,a); }
inline uint64_t	ax_align_multiple( uint64_t n, uint64_t a ) { return _ax_align_multiple_uint(n,a); }
inline axSize	ax_align_multiple( axSize   n, axSize   a ) { return _ax_align_multiple_uint(n,a); }


//! floating-point align
template<class T> inline
T  _ax_align_multiple_float( T   n, T   a ) {
	T i = ax_floor( n / a ) * a;
	if( i == n ) return i;
	return (n > 0) ? i+a : i-a;
}

inline float  ax_align_multiple( float  n, float  a ) { return _ax_align_multiple_float(n,a); }
inline double ax_align_multiple( double n, double a ) { return _ax_align_multiple_float(n,a); }


//get remainder
inline float  ax_fmod( float  a, float  b ) { return ::fmodf(a,b); }
inline double ax_fmod( double a, double b ) { return ::fmod (a,b); }

inline float  ax_cos( float  a ) { return ::cosf(a); }
inline double ax_cos( double a ) { return ::cos (a); }

inline float  ax_sin( float  a ) { return ::sinf(a); }
inline double ax_sin( double a ) { return ::sin (a); }

inline float  ax_atan2( float  a, float  b ) { return ::atan2f(a,b); }
inline double ax_atan2( double a, double b ) { return ::atan2 (a,b); }

template < class T, class W >
inline T ax_bezier( T p0, T p1, T p2, T p3, W w) {
/* 	R = p0 * (1-w)^3
	  + p1 * 3 * (1-w)^2 * w
	  + p2 * 3 * (1-w) * w^2
	  + p3 * w^3
*/

	W iw  = 1-w;
	W iw2 = iw*iw;
	W w2  = w*w;
	
	return	p0 * iw * iw2
		  + p1 * 3  * iw2 * w
		  + p2 * 3  * iw  * w2
		  + p3 * w2 * w;
}


//@}

#endif //__ax_math_template_h__

