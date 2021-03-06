#ifndef __axVec4_h__
#define __axVec4_h__

#include "axVec3.h"

//! \addtogroup math
//@{

template<class D> class axVec4;


typedef axVec4<uint8_t>	axVec4b;
typedef axVec4<int>		axVec4i;
typedef axVec4<float>	axVec4f;
typedef axVec4<double>	axVec4d;


template<class T> class axMatrix4x4;

template <class T>
class axVec4 {
public:
	typedef	T	TYPE;
	T x,y,z,w;

	enum { kElementCount = 4 };

	axVec4() {}
	axVec4(  T xx,  T yy,		T zz, T ww ) : x(xx),  y(yy),  z(zz),  w(ww) {}
	axVec4( const axVec2<T> &v, T zz, T ww ) : x(v.x), y(v.y), z(zz),  w(ww) {}
	axVec4( const axVec3<T> &v,       T ww ) : x(v.x), y(v.y), z(v.z), w(ww) {}

		  T*	asPointer	()							{ return &x; }
	const T*	asPointer	() const					{ return &x; }

	void		set			( T xx, T yy, T zz, T ww )	{ x=xx; y=yy; z=zz; w=ww; }
	void		set			( const axVec3<T> &v, T ww )  { set(v.x,v.y,v.z, ww); }
	void		setAll		( T v )						{ x = y = z = w = v; }

		  T&	operator[]	( axSize i )				{ return (&x)[i]; }
	const T&	operator[]	( axSize i ) const			{ return (&x)[i]; }

	axVec4		operator+	( T v ) const				{ return axVec4( x + v, y + v, z + v, w + v ); }
	axVec4		operator-	( T v ) const				{ return axVec4( x - v, y - v, z - v, w - v ); }
	axVec4		operator*	( T v ) const				{ return axVec4( x * v, y * v, z * v, w * v ); }
	axVec4		operator/	( T v ) const				{ return axVec4( x / v, y / v, z / v, w / v ); }

	axVec4		operator+	( const axVec4 &v ) const		{ return axVec4( x + v.x, y + v.y, z + v.z, w + v.w ); }
	axVec4		operator-	( const axVec4 &v ) const		{ return axVec4( x - v.x, y - v.y, z - v.z, w - v.w ); }
	axVec4		operator*	( const axVec4 &v ) const		{ return axVec4( x * v.x, y * v.y, z * v.z, w * v.w ); }
	axVec4		operator/	( const axVec4 &v ) const		{ return axVec4( x / v.x, y / v.y, z / v.z, w / v.w ); }

	axVec4		operator-	() const					{ return axVec4( -x, -y, -z, -w ); }


	void		operator+=	( const axVec4 &v )			{ x+=v.x; y+=v.y; z+=v.z; w+=v.w; }
	void		operator-=	( const axVec4 &v )			{ x-=v.x; y-=v.y; z-=v.z; w-=v.w; }
	void		operator*=	( const axVec4 &v )			{ x*=v.x; y*=v.y; z*=v.z; w*=v.w; }
	void		operator/=	( const axVec4 &v )			{ x/=v.x; y/=v.y; z/=v.z; w/=v.w; }

	void		operator+=	( T v )						{ x+=v; y+=v; z+=v; w+=v; }
	void		operator-=	( T v )						{ x-=v; y-=v; z-=v; w-=v; }
	void		operator*=	( T v )						{ x*=v; y*=v; z*=v; w*=v; }
	void		operator/=	( T v )						{ x/=v; y/=v; z/=v; w/=v; }

	axVec4		operator*	( const axMatrix4x4<T> &m ) const;
	void		operator*=	( const axMatrix4x4<T> &m );

	bool		operator==	( const axVec4 &v ) const		{ return ( x == v.x && y == v.y && z == v.z && w == v.w ); }
	bool		operator!=	( const axVec4 &v ) const		{ return ( x != v.x || y != v.y || z != v.z || w != v.w ); }
	bool		operator<	( const axVec4 &v ) const		{ return ( x <  v.x && y <  v.y && z <  v.z && w <  v.w ); }
	bool		operator<=	( const axVec4 &v ) const		{ return ( x <= v.x && y <= v.y && z <= v.z && w <= v.w ); }
	bool		operator>	( const axVec4 &v ) const		{ return ( x >  v.x && y >  v.y && z >  v.z && w >  v.w ); }
	bool		operator>=	( const axVec4 &v ) const		{ return ( x >= v.x && y >= v.y && z >= v.z && w >= v.w ); }

	bool		isAll		( const T v ) const				{ return (x==v && y==v && z==v && w==v); }
	bool		isAny		( const T v ) const				{ return (x==v || y==v || z==v || w==v); }

	T			dot			( const axVec4 &v ) const		{ return (x*v.x) + (y*v.y) + (z*v.z) + (w*v.w); }
	T			average		() const						{ return (x + y + z + w) / (T)4; }

	template<class D>	axVec4<D>	to_Vec4	() const { return axVec4<D>( (D)x, (D)y, (D)z, (D)w ); }

						axVec3<T>	to_Vec3	() const;
				
	template<class S>	axStatus	serialize_io_bin( S &s );
						axStatus	serialize_io	( axSerializer	 &s ) { return serialize_io_bin(s); }
						axStatus	serialize_io	( axDeserializer &s ) { return serialize_io_bin(s); }

	template<class S>	axStatus	serialize_io	( S &s );
	template<class S>	axStatus	db_io			( S &s ) { return serialize_io(s); }
		
	
						axStatus	toStringFormat	( axStringFormat &f ) const;
						axStatus	onTake( axVec4<T> &b )				{ *this = b; return 0; }
};

#define axTYPE_LIST(T) \
	template<> class axTypeTrait<T> { \
	public: \
		static	const	bool	isPOD = true; \
		static	const	bool	rawSerializable = true; \
	};\
//----
    axTYPE_LIST( axVec4b )
    axTYPE_LIST( axVec4i )
    axTYPE_LIST( axVec4f )
    axTYPE_LIST( axVec4d )
#undef axTYPE_LIST

template<class T> inline axVec4<T> operator+ ( T a, const axVec4<T> & b ) { return axVec4<T>( a + b.x, a + b.y, a + b.z, a + b.w ); }
template<class T> inline axVec4<T> operator- ( T a, const axVec4<T> & b ) { return axVec4<T>( a - b.x, a - b.y, a - b.z, a - b.w ); }
template<class T> inline axVec4<T> operator* ( T a, const axVec4<T> & b ) { return axVec4<T>( a * b.x, a * b.y, a * b.z, a * b.w ); }
template<class T> inline axVec4<T> operator/ ( T a, const axVec4<T> & b ) { return axVec4<T>( a / b.x, a / b.y, a / b.z, a / b.w ); }

template< class T > inline axVec4i to_axVec4i( const axVec4<T>& v ) { return v.template to_Vec4<int>(); }
template< class T > inline axVec4f to_axVec4f( const axVec4<T>& v ) { return v.template to_Vec4<float>(); }
template< class T > inline axVec4d to_axVec4d( const axVec4<T>& v ) { return v.template to_Vec4<double>(); }

// 1 arg
#define axVec_FUNC( FUNC )	\
	template<class T> inline axVec4<T> FUNC( const axVec4<T> &v ) { \
		return axVec4<T>( FUNC(v.x), FUNC(v.y), FUNC(v.z), FUNC(v.w) ); \
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
	template<class T> inline axVec4<T> FUNC( const axVec4<T> &a, const axVec4<T> &b ) { \
		return axVec4<T>( FUNC( a.x, b.x ), FUNC( a.y, b.y ), FUNC( a.z, b.z ), FUNC( a.w, b.w ) ); \
	} \
//----
	axVec_FUNC( ax_min )
	axVec_FUNC( ax_max )
#undef axVec_FUNC

template<class T> inline axVec4<T> ax_clamp( const axVec4<T> &v, const axVec4<T> a,  const axVec4<T> b ) {
	return axVec4<T>(ax_clamp( v.x, a.x, b.x ),
                         ax_clamp( v.y, a.y, b.y ),
                         ax_clamp( v.z, a.z, b.z ),
                         ax_clamp( v.w, a.w, b.w ) );
}

template<class T> inline
axVec3<T>	 axVec4<T>::to_Vec3() const {
	if( w == 0 ) return axVec3<T>( 0,0,0 );
	if( w == 1 ) return axVec3<T>( x,y,z );
	return axVec3<T>( x/w,y/w,z/w );
}

template<class T> inline
axVec4<T> ax_lerp ( const axVec4<T> &a, const axVec4<T> b, T  weight ) {
	return axVec4<T>(   ax_lerp( a.x, b.x, weight ),
                            ax_lerp( a.y, b.y, weight ),
                            ax_lerp( a.z, b.z, weight ),
                            ax_lerp( a.w, b.w, weight ) );
}

template<class T> inline
axVec4i	ax_round_int( const axVec4<T> & a ) {
	return axVec4i( ax_round_int(a.x), ax_round_int(a.y), ax_round_int(a.z), ax_round_int(a.w) );
}


template<class T> inline
axStatus axVec4<T> :: toStringFormat( axStringFormat &f ) const {
	return f.format( "<{?}, {?}, {?}, {?}>", x, y, z, w );
}	

template<class T>
template<class S> inline
axStatus axVec4<T>::serialize_io_bin( S &s ) {
	#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
		return s.io_raw( this, sizeof(x) * kElementCount );
	#else	
		return serialize_io(s);
	#endif
}

template<class T>
template<class S> inline
axStatus axVec4<T>::serialize_io( S &s ) {
	axStatus st;
	ax_io(x);
	ax_io(y);
	ax_io(z);
	ax_io(w);
	return 0;
}


template< class T > inline axStatus ax_json_serialize_value( axJsonWriter &s, axVec4<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 4 ); }
template< class T > inline axStatus ax_json_serialize_value( axJsonParser &s, axVec4<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 4 ); }


#endif //__axVec4_h__
