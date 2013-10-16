#ifndef __axVec2_h__
#define __axVec2_h__

#include "../common/ax_common.h"
#include "../string/ax_string.h"
#include "../other/axJson.h"

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

	axVec2		operator*	( const axMatrix4<T> &m ) const { return mul3x2(m);	 }
	void		operator*=	( const axMatrix4<T> &m )		{ *this = this->mul3x2(m); }
	
	axVec2		mul3x2		( const axMatrix4<T> &m ) const;
	
	
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


	T			dot			( const axVec2 &v ) const		{ return (x * v.x + y * v.y) ; }
	//magnitude
	T			mag			() const						{ return sqrt( magSq() ); }
	T			magSq		() const						{ return ( x*x + y*y ); }

	T			distance	( const axVec2 &v ) const		{ return sqrt ( distanceSq( v ) ); }
	T			distanceSq	( const axVec2 &v ) const		{ return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y);	}

	T			cross		( const axVec2 &v ) const		{ return x*v.y - y*v.x; }

	bool		isInTriangle( const axVec2 &v0, const axVec2 &v1, const axVec2 &v2 );
	
	axVec2		perpendicular()								{ return axVec2(-y,x); }

	axVec2		normalize	() const						{ T r = magSq(); return r ? (*this/ax_sqrt(r)) : *this; }
	void		normalizeIt	()								{ *this = normalize(); }	

	T			closetPointAtLineSegment	( const axVec2<T> &line0, const axVec2<T> &line1 ) const;

	axVec2<T>	closetPointAtLine			( const axVec2<T> &line0, const axVec2<T> &line1, T & out_segment ) const;
	axVec2<T>	closetPointAtLine			( const axVec2<T> &line0, const axVec2<T> &line1 ) const 	{ T s; return closetPointAtLine( line0, line1, s ); }
	

	axVec2		dir			( const axVec2 &v ) const		{ return (*this - v).normalize(); }
	
	template<class D>	axVec2<D>	to_Vec2 () const { return axVec2<D>( (D)x, (D)y ); }
	
						axStatus	onTake	( axVec2<T> &b )				{ *this = b; return 0; }

	template<class S>	axStatus	serialize_io_bin( S &s );
						axStatus	serialize_io	( axSerializer	 &s ) { return serialize_io_bin(s); }
						axStatus	serialize_io	( axDeserializer &s ) { return serialize_io_bin(s); }

	template<class S>	axStatus	serialize_io	( S &s );
	template<class S>	axStatus	db_io			( S &s ) { return serialize_io(s); }
	
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

template<class T> inline axVec2<T> operator+ ( T a, const axVec2<T> & b ) { return axVec2<T>( a + b.x, a + b.y ); }
template<class T> inline axVec2<T> operator- ( T a, const axVec2<T> & b ) { return axVec2<T>( a - b.x, a - b.y ); }
template<class T> inline axVec2<T> operator* ( T a, const axVec2<T> & b ) { return axVec2<T>( a * b.x, a * b.y ); }
template<class T> inline axVec2<T> operator/ ( T a, const axVec2<T> & b ) { return axVec2<T>( a / b.x, a / b.y ); }

	
template<class T> inline axVec2i to_axVec2i( const axVec2<T>& v ) { return v.template to_Vec2<int>(); }
template<class T> inline axVec2f to_axVec2f( const axVec2<T>& v ) { return v.template to_Vec2<float>(); }
template<class T> inline axVec2d to_axVec2d( const axVec2<T>& v ) { return v.template to_Vec2<double>(); }

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
axStatus axVec2<T>::serialize_io_bin( S &s ) {
	#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
		return s.io_raw( this, sizeof(x)*kElementCount );
	#else
		return serialize_io(s);
	#endif
}

template<class T>
template<class S> inline 
axStatus axVec2<T>::serialize_io( S &s ) {
	axStatus st;
	ax_io(x);
	ax_io(y);
	return 0;
}

template< class T > inline axStatus ax_json_serialize_value( axJsonWriter &s, axVec2<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 2 ); }
template< class T > inline axStatus ax_json_serialize_value( axJsonParser &s, axVec2<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 2 ); }


#define axTYPE_LIST(T) \
	template<> class axTypeOf<T> { \
	public: \
		static 	const 	bool	isPOD = true; \
		static	const	bool	rawSerializable = true; \
	};\
//----
    axTYPE_LIST( axVec2b )
    axTYPE_LIST( axVec2i )
    axTYPE_LIST( axVec2f )
    axTYPE_LIST( axVec2d )
#undef axTYPE_LIST


// 1 arg
#define axVec_FUNC( FUNC )	\
	template<class T> inline axVec2<T> FUNC( const axVec2<T> &v ) { \
		return axVec2<T>( FUNC(v.x), FUNC(v.y) ); \
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
	template<class T> inline axVec2<T> FUNC( const axVec2<T> &a, const axVec2<T> &b ) { \
		return axVec2<T>( FUNC( a.x, b.x ), FUNC( a.y, b.y ) ); \
	} \
//----
	axVec_FUNC( ax_min )
	axVec_FUNC( ax_max )
#undef axVec_FUNC
    
template<class T> inline
axVec2<T> ax_clamp ( const axVec2<T> &v, const axVec2<T> a,  const axVec2<T> b ) {
	return axVec2<T>( ax_clamp( v.x, a.x, b.x ),
					  ax_clamp( v.y, a.y, b.y ) );
}

template<class T> inline
axVec2<T> ax_lerp ( const axVec2<T> &a, const axVec2<T> b, T  weight ) {
	return axVec2<T>( ax_lerp( a.x, b.x, weight ),
                      ax_lerp( a.y, b.y, weight ) );
}

template<class T> inline
axVec2i	ax_round_to_int( const axVec2<T> & a ) {
	return axVec2i( ax_round_to_int(a.x), ax_round_to_int(a.y) );
}

//@}

//--------------------------

template <class T> inline
bool axVec2<T>::isInTriangle( const axVec2 &v0, const axVec2 &v1, const axVec2 &v2 ) {
	if( (v2 - v0).cross(v1 - v0) < 0 ) {
		if( (*this - v0).cross(v1 - v0) > 0 ) return false;
		if( (*this - v1).cross(v2 - v1) > 0 ) return false;
		if( (*this - v2).cross(v0 - v2) > 0 ) return false;
	}else{
		if( (*this - v0).cross(v1 - v0) < 0 ) return false;
		if( (*this - v1).cross(v2 - v1) < 0 ) return false;
		if( (*this - v2).cross(v0 - v2) < 0 ) return false;
	}
	return true;
}


#endif //__axVec2_h__
