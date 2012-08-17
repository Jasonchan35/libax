#ifndef __axVec3_h__
#define __axVec3_h__

#include "axVec2.h"

//! \addtogroup math
//@{


template<class D> class axVec3;

typedef axVec3<uint8_t>	axVec3b;
typedef axVec3<int>		axVec3i;
typedef axVec3<float>	axVec3f;
typedef axVec3<double>	axVec3d;


template<class T> class axMatrix4;

template <class T>
class axVec3 {
public:
	typedef	T	TYPE;
	T x, y, z;
	enum { kElementCount = 3 };

	axVec3() {}
	axVec3( T xx,  T yy,		T zz ) : x(xx),  y(yy),  z(zz) {}
	axVec3( const axVec2<T> &v, T zz ) : x(v.x), y(v.y), z(zz) {}

		  T*	asPointer()								{ return &x; }
	const T*	asPointer() const						{ return &x; }

	void		set			( T xx, T yy, T zz )		{ x=xx; y=yy; z=zz; }
	void		setAll		( T v )						{ x = y = z = v; }

	      T&	operator[] ( axSize i )					{ return (&x)[i]; }
	const T&	operator[] ( axSize i ) const			{ return (&x)[i]; }

	axVec3		operator+	( T v ) const				{ return axVec3( x + v, y + v, z + v ); }
	axVec3		operator-	( T v ) const				{ return axVec3( x - v, y - v, z - v ); }
	axVec3		operator*	( T v ) const				{ return axVec3( x * v, y * v, z * v ); }
	axVec3		operator/	( T v ) const				{ return axVec3( x / v, y / v, z / v ); }

	axVec3		operator+	( const axVec3 &v ) const		{ return axVec3( x + v.x, y + v.y, z + v.z ); }
	axVec3		operator-	( const axVec3 &v ) const		{ return axVec3( x - v.x, y - v.y, z - v.z ); }
	axVec3		operator*	( const axVec3 &v ) const		{ return axVec3( x * v.x, y * v.y, z * v.z ); }
	axVec3		operator/	( const axVec3 &v ) const		{ return axVec3( x / v.x, y / v.y, z / v.z ); }

	axVec3		operator-	() const					{ return axVec3( -x, -y, -z ); }

	void		operator+=	( const axVec3 &v )			{ x+=v.x; y+=v.y; z+=v.z; }
	void		operator-=	( const axVec3 &v )			{ x-=v.x; y-=v.y; z-=v.z; }
	void		operator*=	( const axVec3 &v )			{ x*=v.x; y*=v.y; z*=v.z; }
	void		operator/=	( const axVec3 &v )			{ x/=v.x; y/=v.y; z/=v.z; }

	axVec3		operator*	( const axMatrix4<T> &m ) const;
	void		operator*=	( const axMatrix4<T> &m );

	axVec3		mul3x3		( const axMatrix4<T> &m ) const;	//!< no translate and projection
	axVec3		mul4x3		( const axMatrix4<T> &m ) const;	//!< no projection

	void		operator+=	( const T v )				{ x+=v; y+=v; z+=v; }
	void		operator-=	( const T v )				{ x-=v; y-=v; z-=v; }
	void		operator*=	( const T v )				{ x*=v; y*=v; z*=v; }
	void		operator/=	( const T v )				{ x/=v; y/=v; z/=v; }

	//! cross product
	axVec3		operator^	( const axVec3 &v ) const		{ return axVec3( y*v.z - z*v.y, z*v.x - x*v.z,x*v.y - y*v.x ); }
	T			dot			( const axVec3 &v ) const		{ return (x*v.x) + (y*v.y) + (z*v.z); }

	bool		operator==	( const axVec3 &v ) const		{ return ( x == v.x && y == v.y && z == v.z ); }
	bool		operator!=	( const axVec3 &v ) const		{ return ( x != v.x || y != v.y || z != v.z ); }

	bool		isAll		( const T v ) const				{ return (x==v && y==v && z==v); }
	bool		isAny		( const T v ) const				{ return (x==v || y==v || z==v); }

	axVec3		reflect		( const axVec3 &normal ) const	{ return *this - ( normal * ( dot( normal ) * 2 ) ); }
	axVec3		reflectHalf ( const axVec3 &normal ) const	{ return *this - ( normal * ( dot( normal )     ) ); }

	//! magnitude
	T			mag			() const						{ return sqrt( magSq() ); }
	T			magSq		() const						{ return (x*x + y*y + z*z); }

	T			distance	( const axVec3 &v ) const		{ return sqrt( distanceSq( v ) ); }

	//! distance in square
	T			distanceSq	( const axVec3 &v ) const		{ return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z); }

	axVec3		normalize	() const						{ T r = magSq(); return r ? (*this/ax_sqrt(r)) : *this; }
	void		normalizeIt	()								{ *this = normalize(); }

	axVec3		dir			( const axVec3 &v ) const		{ return (*this - v).normalize(); }

	bool		isParallel          ( const axVec3 &v, T tolerance = ax_fp_epsilon<T>() ) const;
	void		faceNormal			( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2 );

	//! angle between this and v
	T			angle		        ( const axVec3 &v );
	bool		isInsideTriangle	( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2 ) const;
	bool		isInsideTriangle	( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2, const axVec3 &normal ) const;

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;
	axStatus	onTake( axVec3<T> &b )				{ *this = b; return 0; }
};

#define axTYPE_LIST(T) \
	template<> class axTypeOf<T> { \
	public: \
		static const bool isPOD = true; \
	};\
//----
	axTYPE_LIST( axVec3b )
	axTYPE_LIST( axVec3i )
	axTYPE_LIST( axVec3f )
	axTYPE_LIST( axVec3d )
#undef axTYPE_LIST

template< class T > inline axVec3i to_axVec3i( const axVec3<T>& v ) { return axVec3i( (int   )v.x, (int   )v.y, (int   )v.z ); }
template< class T > inline axVec3f to_axVec3f( const axVec3<T>& v ) { return axVec3f( (float )v.x, (float )v.y, (float )v.z ); }
template< class T > inline axVec3d to_axVec3d( const axVec3<T>& v ) { return axVec3d( (double)v.x, (double)v.y, (double)v.z ); }


template<class T> inline axVec3<T> ax_abs( const axVec3<T> &v ) { return axVec3<T>( ax_abs(v.x), ax_abs(v.y), ax_abs(v.z) ); }

template<class T> inline axVec3<T> ax_min ( const axVec3<T> &a, const axVec3<T> &b ) { return axVec3<T>( ax_min(a.x,b.x), ax_min(a.y,b.y), ax_min(a.z,b.z) ); }
template<class T> inline axVec3<T> ax_max ( const axVec3<T> &a, const axVec3<T> &b ) { return axVec3<T>( ax_max(a.x,b.x), ax_max(a.y,b.y), ax_max(a.z,b.z) ); }

template<class T> inline void ax_min_it( axVec3<T> &a, const axVec3<T> &b ) { 
    ax_min_it( a.x, b.x );
    ax_min_it( a.y, b.y );
    ax_min_it( a.z, b.z );
}

template<class T> inline void ax_max_it( axVec3<T> &a, const axVec3<T> &b ) { 
    ax_max_it( a.x, b.x );
    ax_max_it( a.y, b.y );
    ax_max_it( a.z, b.z );    
}

template<class T> inline axVec3<T> ax_clamp ( const axVec3<T> &v, const axVec3<T> a,  const axVec3<T> b ) {
	return axVec3<T>( ax_clamp( v.x, a.x, b.x ),
			  ax_clamp( v.y, a.y, b.y ),
			  ax_clamp( v.z, a.z, b.z ) );
}

template<class T> inline void ax_clamp_it ( axVec3<T>  &o,  const axVec3<T> a,  const axVec3<T> b )	{ 
	ax_clamp_it( o.x, a.x, b.x );
	ax_clamp_it( o.y, a.y, b.y );
	ax_clamp_it( o.z, a.z, b.z );
}

template<class T> inline
axVec3<T> ax_lerp ( const axVec3<T> &a, const axVec3<T> b, T  weight ) {
	return axVec3<T>( ax_lerp( a.x, b.x, weight ),
			  ax_lerp( a.y, b.y, weight ),
			  ax_lerp( a.z, b.z, weight ) );
}

template <class T> inline
void axVec3<T>::faceNormal( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2 ) {
	*this = ( (v1-v0)^(v2-v0) ).normalize();
}

template<class T> inline
axStatus axVec3<T> :: toStringFormat( axStringFormat &f ) const {
	return f.format( "<{?}, {?}, {?}>", x, y, z );
}	

template<class T>
template<class S> inline 
axStatus axVec3<T>::serialize_io( S &s ) {
	#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
		return s.io_raw( this, sizeof(x)*kElementCount );
	#else
		axStatus st;
		st = s.io( x );	if( !st ) return st;
		st = s.io( y );	if( !st ) return st;
		st = s.io( z );	if( !st ) return st;
		return 0;
	#endif
}

inline axVec3d ax_toVec3d( const axVec3f& v ) { return axVec3d(v.x,v.y,v.z); }

//@}


#endif //__axVec3_h__

