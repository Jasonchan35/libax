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


template<class T> class axMatrix4;

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

	axVec4		operator*	( const axMatrix4<T> &m ) const;
	void		operator*=	( const axMatrix4<T> &m );

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

	axVec3<T>	to_Vec3		();

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;

						axStatus	onTake( axVec4<T> &b )				{ *this = b; return 0; }
};

#define axTYPE_LIST(T) \
	template<> class axTypeOf<T> { \
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

template< class T > inline axVec4i to_axVec4i( const axVec4<T>& v ) { return axVec4i( (int   )v.x, (int   )v.y, (int   )v.z, (int   )v.w ); }
template< class T > inline axVec4f to_axVec4f( const axVec4<T>& v ) { return axVec4f( (float )v.x, (float )v.y, (float )v.z, (float )v.w ); }
template< class T > inline axVec4d to_axVec4d( const axVec4<T>& v ) { return axVec4d( (double)v.x, (double)v.y, (double)v.z, (double)v.w ); }


template<class T> inline axVec4<T> ax_abs( const axVec4<T> &v ) { return axVec4<T>( ax_abs(v.x), ax_abs(v.y), ax_abs(v.z), ax_abs(v.w) ); }

template<class T> inline axVec4<T> ax_min ( const axVec4<T> &a, const axVec4<T> &b ) { return axVec4<T>( ax_min(a.x,b.x), ax_min(a.y,b.y), ax_min(a.z,b.z), ax_min(a.w,b.w) );  }
template<class T> inline axVec4<T> ax_max ( const axVec4<T> &a, const axVec4<T> &b ) { return axVec4<T>( ax_max(a.x,b.x), ax_max(a.y,b.y), ax_max(a.z,b.z), ax_max(a.w,b.w) );  }

template<class T> inline void ax_min_it( axVec4<T> &a, const axVec4<T> &b ) { 
    ax_min_it( a.x, b.x );
    ax_min_it( a.y, b.y );
    ax_min_it( a.z, b.z );
    ax_min_it( a.w, b.w );
}

template<class T> inline void ax_max_it( axVec4<T> &a, const axVec4<T> &b ) { 
    ax_max_it( a.x, b.x );
    ax_max_it( a.y, b.y );
    ax_max_it( a.z, b.z );    
    ax_max_it( a.w, b.w );    
}

template<class T> inline axVec4<T> ax_clamp( const axVec4<T> &v, const axVec4<T> a,  const axVec4<T> b ) {
	return axVec4<T>(ax_clamp( v.x, a.x, b.x ),
                         ax_clamp( v.y, a.y, b.y ),
                         ax_clamp( v.z, a.z, b.z ),
                         ax_clamp( v.w, a.w, b.w ) );
}

template<class T> inline void ax_clamp_it ( axVec4<T>  &o,  const axVec4<T> a,  const axVec4<T> b )	{ 
	ax_clamp_it( o.x, a.x, b.x );
	ax_clamp_it( o.y, a.y, b.y );
	ax_clamp_it( o.z, a.z, b.z );
	ax_clamp_it( o.w, a.w, b.w );
}

template<class T> inline
axVec3<T>	 axVec4<T>::to_Vec3() {
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
axStatus axVec4<T> :: toStringFormat( axStringFormat &f ) const {
	return f.format( "<{?}, {?}, {?}, {?}>", x, y, z, w );
}	

template<class T>
template<class S> inline
axStatus axVec4<T>::serialize_io( S &s ) {
	#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
		return s.io_raw( this, sizeof(x) * kElementCount );
	#else	
		axStatus st;
		st = s.io( x );	if( !st ) return st;
		st = s.io( y );	if( !st ) return st;
		st = s.io( z );	if( !st ) return st;
		st = s.io( w );	if( !st ) return st;
		return 0;
	#endif
}

#endif //__axVec4_h__
