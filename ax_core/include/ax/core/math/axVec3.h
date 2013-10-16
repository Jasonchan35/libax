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
template<class T> class axQuaternion;

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

	axVec3		mul4x4		( const axMatrix4<T> &m ) const;
	void		mul4x4_it	( const axMatrix4<T> &m )		{ *this = this->mul4x4(m); }

	//!< no projection
	axVec3		mul4x3		( const axMatrix4<T> &m ) const;	
	void		mul4x3_it	( const axMatrix4<T> &m )		{ *this = this->mul4x3(m); }

	//!< no translate and projection
	axVec3		mul3x3		( const axMatrix4<T> &m ) const;	
	void		mul3x3_it	( const axMatrix4<T> &m )		{ *this = this->mul3x3(m); }

	axVec3		operator*	( const axQuaternion<T> &q ) const;
	void		operator*=	( const axQuaternion<T> &q )	{ *this = *this * q; }

	void		operator+=	( const T v )				{ x+=v; y+=v; z+=v; }
	void		operator-=	( const T v )				{ x-=v; y-=v; z-=v; }
	void		operator*=	( const T v )				{ x*=v; y*=v; z*=v; }
	void		operator/=	( const T v )				{ x/=v; y/=v; z/=v; }

	axVec3		cross		( const axVec3 &v ) const		{ return axVec3( y*v.z - z*v.y, z*v.x - x*v.z,x*v.y - y*v.x ); }
	T			dot			( const axVec3 &v ) const		{ return (x*v.x) + (y*v.y) + (z*v.z); }

	bool		operator==	( const axVec3 &v ) const		{ return ( x == v.x && y == v.y && z == v.z ); }
	bool		operator!=	( const axVec3 &v ) const		{ return ( x != v.x || y != v.y || z != v.z ); }

	bool		isAll		( const T v ) const				{ return (x==v && y==v && z==v); }
	bool		isAny		( const T v ) const				{ return (x==v || y==v || z==v); }

	axVec3		reflect		( const axVec3 &normal, T f=(T)2 ) const	{ return *this - ( normal * ( dot( normal ) * f ) ); }

	//! magnitude
	T			mag			() const						{ return sqrt( magSq() ); }
	T			magSq		() const						{ return (x*x + y*y + z*z); }

	//! distance in square
	T			distanceSq	( const axVec3 &v ) const		{ return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z); }
	T			distance	( const axVec3 &v ) const		{ return sqrt( distanceSq( v ) ); }
	axVec3		direction	( const axVec3 &v ) const		{ return (*this - v).normalize(); }


	axVec3		normalize	() const						{ T r = magSq(); return r ? (*this/ax_sqrt(r)) : *this; }
	void		normalizeIt	()								{ *this = normalize(); }


	bool		isParallel          ( const axVec3 &v, T tolerance = ax_epsilon<T>() ) const { return ax_math_equals( ax_abs( normalize().dot( v.normalize() ) ), (T)1, tolerance ); }

	//! angle between this and v
	T			angle		        ( const axVec3 &v );
	bool		isInTriangle		( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2 ) const;

	template<class D>	axVec3<D>	to_Vec3 () const { return axVec3<D>( (D)x, (D)y, (D)z ); }

	template<class S>	axStatus	serialize_io_bin( S &s );
						axStatus	serialize_io	( axSerializer	 &s ) { return serialize_io_bin(s); }
						axStatus	serialize_io	( axDeserializer &s ) { return serialize_io_bin(s); }

	template<class S>	axStatus	serialize_io	( S &s );
	template<class S>	axStatus	db_io			( S &s ) { return serialize_io(s); }
						
						axStatus	toStringFormat	( axStringFormat &f ) const;
						axStatus	onTake			( axVec3<T> &b )			{ *this = b; return 0; }
};

#define axTYPE_LIST(T) \
	template<> class axTypeOf<T> { \
	public: \
		static	const	bool	isPOD = true; \
		static	const	bool	rawSerializable = true; \
	};\
//----
	axTYPE_LIST( axVec3i )
	axTYPE_LIST( axVec3f )
	axTYPE_LIST( axVec3d )
#undef axTYPE_LIST

template<class T> inline
axVec3<T>	ax_tri_front( const axVec3<T> &v0, const axVec3<T> &v1, const axVec3<T> &v2 ) {
	return (v1-v0).cross(v2-v0);
}

template<class T> inline
axVec3<T>	ax_tri_normal( const axVec3<T> &v0, const axVec3<T> &v1, const axVec3<T> &v2 ) {
	return ax_tri_front(v0,v1,v2).normalize();
}


template<class T> inline axVec3<T> operator+ ( T a, const axVec3<T> & b ) { return axVec3<T>( a + b.x, a + b.y, a + b.z ); }
template<class T> inline axVec3<T> operator- ( T a, const axVec3<T> & b ) { return axVec3<T>( a - b.x, a - b.y, a - b.z ); }
template<class T> inline axVec3<T> operator* ( T a, const axVec3<T> & b ) { return axVec3<T>( a * b.x, a * b.y, a * b.z ); }
template<class T> inline axVec3<T> operator/ ( T a, const axVec3<T> & b ) { return axVec3<T>( a / b.x, a / b.y, a / b.z ); }

template<class T> inline axVec3i to_axVec3i( const axVec3<T>& v ) { return v.template to_Vec3<int>(); }
template<class T> inline axVec3f to_axVec3f( const axVec3<T>& v ) { return v.template to_Vec3<float>(); }
template<class T> inline axVec3d to_axVec3d( const axVec3<T>& v ) { return v.template to_Vec3<double>(); }

// 1 arg
#define axVec_FUNC( FUNC )	\
	template<class T> inline axVec3<T> FUNC( const axVec3<T> &v ) { \
		return axVec3<T>( FUNC(v.x), FUNC(v.y), FUNC(v.z) ); \
	} \
//----
	axVec_FUNC( ax_abs )
	axVec_FUNC( ax_deg_to_rad )
	axVec_FUNC( ax_rad_to_deg )
	axVec_FUNC( ax_floor )
	axVec_FUNC( ax_ceil )
	axVec_FUNC( ax_round )
#undef axVec_FUNC

// 2 arg
#define axVec_FUNC( FUNC )	\
	template<class T> inline axVec3<T> FUNC( const axVec3<T> &a, const axVec3<T> &b ) { \
		return axVec3<T>( FUNC( a.x, b.x ), FUNC( a.y, b.y ), FUNC( a.z, b.z ) ); \
	} \
//----
	axVec_FUNC( ax_min )
	axVec_FUNC( ax_max )
#undef axVec_FUNC

template<class T> inline axVec3<T> ax_clamp ( const axVec3<T> &v, const axVec3<T> a,  const axVec3<T> b ) {
	return axVec3<T>( ax_clamp( v.x, a.x, b.x ),
			  ax_clamp( v.y, a.y, b.y ),
			  ax_clamp( v.z, a.z, b.z ) );
}

template<class T> inline
axVec3<T> ax_lerp ( const axVec3<T> &a, const axVec3<T> b, T  weight ) {
	return axVec3<T>( ax_lerp( a.x, b.x, weight ),
			  ax_lerp( a.y, b.y, weight ),
			  ax_lerp( a.z, b.z, weight ) );
}

template<class T> inline
axVec3i	ax_round_to_int( const axVec3<T> & a ) {
	return axVec3i( ax_round_to_int(a.x), ax_round_to_int(a.y), ax_round_to_int(a.z) );
}


template<class T> inline
axStatus axVec3<T> :: toStringFormat( axStringFormat &f ) const {
	return f.format( "<{?}, {?}, {?}>", x, y, z );
}	

template<class T>
template<class S> inline 
axStatus axVec3<T>::serialize_io_bin( S &s ) {
	#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
		return s.io_raw( this, sizeof(x)*kElementCount );
	#else
		return serialize_io(s);
	#endif
}

template<class T>
template<class S> inline 
axStatus axVec3<T>::serialize_io( S &s ) {
	axStatus st;
	ax_io(x);
	ax_io(y);
	ax_io(z);
	return 0;
}


template< class T > inline axStatus ax_json_serialize_value( axJsonWriter &s, axVec3<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 3 ); }
template< class T > inline axStatus ax_json_serialize_value( axJsonParser &s, axVec3<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 3 ); }

inline axVec3d ax_toVec3d( const axVec3f& v ) { return axVec3d(v.x,v.y,v.z); }

//@}


#endif //__axVec3_h__

