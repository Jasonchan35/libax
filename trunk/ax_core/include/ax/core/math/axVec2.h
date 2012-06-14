#ifndef __axVec2_h__
#define __axVec2_h__

#include "../../base/common/ax_common.h"
#include "../../base/string/ax_string.h"

//! \addtogroup math
//@{

template<class T> class axVec2;

typedef axVec2<uint8_t>	axVec2b;
typedef axVec2<int>		axVec2i;
typedef axVec2<float>	axVec2f;
typedef axVec2<double>	axVec2d;
typedef axVec2<size_t>	axVec2size;

template<class T> class axMatrix4;

template <class T>
class axVec2 {
public:
	typedef	T	TYPE;
	T	x,y;

	enum { kElementCount = 2 };

	axVec2() {}
	axVec2( T xx, T yy ) :x(xx), y(yy) {}


		  T*	asPointer	()							{ return &x; }
	const T*	asPointer	() const					{ return &x; }

	void		set			( T xx, T yy )				{ x=xx; y=yy; }
	void		set			( axVec2<T> v )				{ x=v.x; y=v.y; }
	
	void		setAll		( T v )						{ x=y=v; }

	      T&	operator[]	( axSize i )				{ return (&x)[i]; }
	const T&	operator[]	( axSize i ) const			{ return (&x)[i]; }

	axVec2		operator+	( T v ) const				{ return axVec2( x + v, y + v ); }
	axVec2		operator-	( T v ) const				{ return axVec2( x - v, y - v ); }
	axVec2		operator*	( T v ) const				{ return axVec2( x * v, y * v ); }
	axVec2		operator/	( T v ) const				{ return axVec2( x / v, y / v ); }

	axVec2		operator+	( const axVec2 &v ) const		{ return axVec2( x + v.x, y + v.y ); }
	axVec2		operator-	( const axVec2 &v ) const		{ return axVec2( x - v.x, y - v.y ); }
	axVec2		operator*	( const axVec2 &v ) const		{ return axVec2( x * v.x, y * v.y ); }
	axVec2		operator/	( const axVec2 &v ) const		{ return axVec2( x / v.x, y / v.y ); }

	axVec2		operator-	() const					{ return axVec2( -x,-y ); }

	void		operator+=	( const axVec2 &v )			{ x+=v.x; y+=v.y; }
	void		operator-=	( const axVec2 &v )			{ x-=v.x; y-=v.y; }
	void		operator*=	( const axVec2 &v )			{ x*=v.x; y*=v.y; }
	void		operator/=	( const axVec2 &v )			{ x/=v.x; y/=v.y; }

	void		operator+=	( T v )						{ x+=v; y+=v; }
	void		operator-=	( T v )						{ x-=v; y-=v; }
	void		operator*=	( T v )						{ x*=v; y*=v; }
	void		operator/=	( T v )						{ x/=v; y/=v; }
	
	bool		operator==	( const axVec2 &v ) const		{ return ( x == v.x && y == v.y ); }
	bool		operator!=	( const axVec2 &v ) const		{ return ( x != v.x || y != v.y ); }
	bool		operator<	( const axVec2 &v ) const		{ return ( x <  v.x && y <  v.y ); }
	bool		operator<=	( const axVec2 &v ) const		{ return ( x <= v.x && y <= v.y ); }
	bool		operator>	( const axVec2 &v ) const		{ return ( x >  v.x && y >  v.y ); }
	bool		operator>=	( const axVec2 &v ) const		{ return ( x >= v.x && y >= v.y ); }

	bool		isAll		( const T v ) const				{ return (x==v && y==v); }
	bool		isAny		( const T v ) const				{ return (x==v || y==v); }

	//magnitude
	T			mag			() const						{ return sqrt( magSq() ); }
	T			magSq		() const						{ return ( x*x + y*y ); }

	T			distance	( const axVec2 &v ) const		{ return sqrt ( distanceSq( v ) ); }
	T			distanceSq	( const axVec2 &v ) const		{ return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y);	}

	//! cross product in 2D should return scaler, not vector
	T			operator^	( const axVec2 &v ) const		{ return x*v.y - y*v.x; }

	bool		isInsideTriangle( const axVec2 &v0, const axVec2 &v1, const axVec2 &v2 );
	
	axVec2		perpendicular()								{ return axVec2d(-y,x); }

	axVec2		normalize	() const						{ T r = magSq(); return r ? (*this/ax_sqrt(r)) : *this; }
	void		normalizeIt	()								{ *this = normalize(); }	
	
	void		roundX()									{ x = ax_round(x); }
	void		roundY()									{ y = ax_round(y); }	
	void		round()										{ roundX(); roundY(); }

	axStatus	onTake( axVec2<T> &b )				{ *this = b; return 0; }

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;

#if axOS_iOS 
	void set( const CGPoint &src )					{ set( src.x,     src.y ); }
	void set( const CGSize  &src )					{ set( src.width, src.height ); }

	void to (       CGPoint &out ) const			{ out.x = x;     out.y = y; }
	void to (       CGSize  &out ) const			{ out.width = x; out.height = y; }
	
	axVec2( const CGPoint &src )					{ set(src); }
	CGPoint to_CGPoint() const						{ CGPoint t; to(t); return t; }

	axVec2( const CGSize &src )						{ set(src); }
	CGSize to_CGSize() const						{ CGSize t; to(t); return t; }
#endif //axOS_iOS
	
#if  axOS_MacOSX
	void set( const NSPoint &src )					{ set( src.x,     src.y ); }
	void set( const NSSize  &src )					{ set( src.width, src.height ); }
	
	void to (       NSPoint &out ) const			{ out.x = x;     out.y = y; }
	void to (       NSSize  &out ) const			{ out.width = x; out.height = y; }
	
	axVec2( const NSPoint &src )					{ set(src); }
	NSPoint to_NSPoint() const						{ NSPoint t; to(t); return t; }
	
	axVec2( const NSSize &src )						{ set(src); }
	NSSize to_NSSize() const						{ NSSize t; to(t); return t; }	
#endif //axOS_MacOSX	

};
	
template< class T > inline axVec2i to_axVec2i( const axVec2<T>& v ) { return axVec2i( (int   )v.x, (int   )v.y ); }
template< class T > inline axVec2f to_axVec2f( const axVec2<T>& v ) { return axVec2f( (float )v.x, (float )v.y ); }
template< class T > inline axVec2d to_axVec2d( const axVec2<T>& v ) { return axVec2d( (double)v.x, (double)v.y ); }

template<class T> inline
axStatus axVec2<T> :: toStringFormat( axStringFormat &f ) const {
    return f.format( "<{?}, {?}>", x, y );
}	

#ifdef axOS_iOS

inline axStatus axStringFormat_out  ( axStringFormat &f, const ::CGPoint &value ) {
    return axStringFormat_out( f, axVec2f(value) );
}

inline axStatus axStringFormat_out  ( axStringFormat &f, const ::CGSize &value ) {
    return axStringFormat_out( f, axVec2f(value) );
}

#endif //axOS_iOS

template<class T>
template<class S> inline 
axStatus axVec2<T>::serialize_io( S &s ) {
	#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
		return s.io_raw( this, sizeof(x)*kElementCount );
	#else
		axStatus st;
		st = s.io( x );	if( !st ) return st;
		st = s.io( y );	if( !st ) return st;
		return 0;
	#endif
}



#define axTYPE_LIST(T) \
	template<> inline bool axTypeOf<T>::isPOD() { return true; }

    axTYPE_LIST( axVec2b )
    axTYPE_LIST( axVec2i )
    axTYPE_LIST( axVec2f )
    axTYPE_LIST( axVec2d )
#undef axTYPE_LIST
    
    
template<class T> inline axVec2<T> ax_abs( const axVec2<T> &v ) { return axVec2<T>( ax_abs(v.x), ax_abs(v.y) ); }

template<class T> inline axVec2<T> ax_min ( const axVec2<T> &a, const axVec2<T> &b ) { return axVec2<T>( ax_min(a.x,b.x), ax_min(a.y,b.y) ); }
template<class T> inline axVec2<T> ax_max ( const axVec2<T> &a, const axVec2<T> &b ) { return axVec2<T>( ax_max(a.x,b.x), ax_max(a.y,b.y) ); }

template<class T> inline void ax_min_it ( axVec2<T> &a, const axVec2<T> &b ) { ax_min_it(a.x,b.x); ax_min_it(a.y,b.y); }
template<class T> inline void ax_max_it ( axVec2<T> &a, const axVec2<T> &b ) { ax_max_it(a.x,b.x); ax_max_it(a.y,b.y); }


template<class T> inline axVec2<T> ax_floor( const axVec2<T> &a ) { axVec2<T>( ax_floor(a.x), ax_floor(a.y) ); }
template<class T> inline axVec2<T> ax_ceil ( const axVec2<T> &a ) { axVec2<T>( ax_ceil (a.x), ax_ceil (a.y) ); }

template<class T> inline axVec2<T> ax_clamp ( const axVec2<T> &v, const axVec2<T> a,  const axVec2<T> b ) {
	return axVec2<T>( ax_clamp( v.x, a.x, b.x ), 
                          ax_clamp( v.y, a.y, b.y ) );
}

template<class T> inline void ax_clamp_it ( axVec2<T>  &o,  const axVec2<T> a,  const axVec2<T> b )	{ 
	ax_clamp_it( o.x, a.x, b.x );
	ax_clamp_it( o.y, a.y, b.y );
}

template<class T> inline
axVec2<T> ax_lerp ( const axVec2<T> &a, const axVec2<T> b, T  weight ) {
	return axVec2<T>( ax_lerp( a.x, b.x, weight ),
                        ax_lerp( a.y, b.y, weight ) );
}

//@}

//--------------------------

template <class T> inline
bool axVec2<T>::isInsideTriangle( const axVec2 &v0, const axVec2 &v1, const axVec2 &v2 ) {
	if( (v2.y - v0.y)*(v1.x - v0.x) - (v2.x - v0.x)*(v1.y - v0.y) < 0 ) {
		if( (y - v0.y)*(v1.x - v0.x) - (x - v0.x)*(v1.y - v0.y) > 0 ) return false;
		if( (y - v1.y)*(v2.x - v1.x) - (x - v1.x)*(v2.y - v1.y) > 0 ) return false;
		if( (y - v2.y)*(v0.x - v2.x) - (x - v2.x)*(v0.y - v2.y) > 0 ) return false;
	}else {
		if( (y - v0.y)*(v1.x - v0.x) - (x - v0.x)*(v1.y - v0.y) < 0 ) return false;
		if( (y - v1.y)*(v2.x - v1.x) - (x - v1.x)*(v2.y - v1.y) < 0 ) return false;
		if( (y - v2.y)*(v0.x - v2.x) - (x - v2.x)*(v0.y - v2.y) < 0 ) return false;
	}
	return true;
}


#endif //__axVec2_h__
