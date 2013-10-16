#ifndef __axRect2_h__
#define __axRect2_h__

#include "axVec2.h"
#include "axMatrix4.h"
#include "axMargin2.h"

//! \addtogroup math
//@{
template <class T> class axRect2;
template <class T> class axMatrix4;

typedef axRect2<int>	axRect2i;
typedef axRect2<float>	axRect2f;
typedef axRect2<double>	axRect2d;

//! Rectangle 
template <class T>
class axRect2 {
public:
	T x, y, w, h;

	axRect2() { x=0; y=0; w=0; h=0; }
	axRect2( T xx, T yy, T ww, T hh ) : x(xx),y(yy),w(ww),h(hh) {}
	axRect2( const axVec2<T> &pos, const axVec2<T> &size ) { setPos ( pos ); setSize( size );	}

		  T*	asPointer()			{ return &x; }
	const T*	asPointer() const	{ return &x; }

	T	right()  const	{ return x+w; }
	T	bottom() const	{ return y+h; }

		  axVec2<T>& pos	()				{ return *(axVec2<T>*)&x; }
		  axVec2<T>& size	()				{ return *(axVec2<T>*)&w; }

	const axVec2<T>& pos	() const		{ return *(axVec2<T>*)&x; }
	const axVec2<T>& size	() const		{ return *(axVec2<T>*)&w; }

	
	void		set			( const axVec2<T> &pos, const axVec2<T> &size )		{ setPos( pos); setSize( size ); } 
	void		setPos		( const T xx, const T yy )		{ x=xx; y=yy; }
	void		setPos		( const axVec2<T> &v )			{ setPos( v.x, v.y ); }

	void		setSize		( const T ww, const T hh )		{ w=ww; h=hh; }
	void		setSize		( const axVec2<T> &v )			{ setSize( v.x, v.y); }

	void		setBottomRight ( const axVec2<T> &v )		{ w = v.x - x;  h = v.y - y; }


	axVec2<T>	topLeft		() const						{ return axVec2<T>( x,   y ); }
	axVec2<T>	topRight	() const						{ return axVec2<T>( x+w, y ); }
	
	axVec2<T>	centerLeft	() const						{ return axVec2<T>( x,   y+h/2 ); }
	axVec2<T>	centerRight	() const						{ return axVec2<T>( x+w, y+h/2 ); }
		
	axVec2<T>	bottomLeft	() const						{ return axVec2<T>( x,   y+h ); }
	axVec2<T>	bottomRight	() const						{ return axVec2<T>( x+w, y+h ); }

	axVec2<T>	center		() const						{ return axVec2<T>( centerX(), centerY() ); }
	T			centerX		() const						{ return x + w / (T)2; }
	T			centerY		() const						{ return y + h / (T)2; }

	axRect2<T>	expand		( T d )	 const					{ return axRect2<T>( x-d,  y-d,  w+d*2,  h+d*2  ); }
	axRect2<T>	expand		( T dx, T dy )	const			{ return axRect2<T>( x-dx, y-dy, w+dx*2, h+dy*2 ); }

	void		unionRect( T xx, T yy, T ww, T hh );
	
	void		unionRect( const axRect2<T> &v ) {
		unionRect( v.x, v.y, v.w, v.h );
	}
	

	axRect2<T>	absSize		() const;
	
	void		set			( T xx, T yy, T ww, T hh )		{ x=xx; y=yy; w=ww; h=hh; }
	void		setAll		( T v )							{ x=v; y=v; w=v; h=v; }

	bool		isAll		( T v ) const					{ return( x==v && y==v && w==v && h==v ); }

	bool		isInside	( const axVec2<T>  &p ) const	{ return ( p.x >= x && p.x <= right() && p.y >= y && p.y <= bottom() ); }

	bool		operator == ( const axRect2<T> &v ) const	{ return( x == v.x && y == v.y && w == v.w && h == v.h ); }
	bool		operator != ( const axRect2<T> &v ) const	{ return( x != v.x || y != v.y || w != v.w || h != v.h ); }

	axRect2<T>	operator *  ( T s )	const					{ return axRect2<T>(x*s, y*s, w*s, h*s); }
	axRect2<T>	operator *  ( const axVec2<T> & s )	const	{ return axRect2<T>(x*s.x, y*s.y, w*s.x, h*s.y); }
	void		operator *= ( T s )							{ *this = *this * s; }
	void		operator *= ( const axVec2<T> & s )			{ *this = *this * s; }

	axRect2<T>	operator /  ( T s )	const					{ return axRect2<T>(x/s, y/s, w/s, h/s); }
	axRect2<T>	operator /  ( const axVec2<T> & s )	const	{ return axRect2<T>(x/s.x, y/s.y, w/s.x, h/s.y); }
	void		operator /= ( T s )							{ *this = *this / s; }
	void		operator /= ( const axVec2<T> & s )			{ *this = *this / s; }

	void		operator *= ( const axMatrix4<T> &m )		{ *this = *this * m; }
	axRect2<T>	operator *  ( const axMatrix4<T> &m	) const;
	
	
	axRect2<T>	operator -  ( const axMargin2<T> &m ) const	{ return axRect2<T>( pos() + m.topLeft(), size() - m.both() ); };
	
	bool		isOverlap	( const axRect2<T> &r ) const;
	bool		intersectOf	( const axRect2<T> &a, const axRect2<T> &b );


	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;

#ifdef axOS_WIN
	inline void set( const RECT &src ) {
		set( (float)src.left, (float)src.top, (float)(src.right-src.left), (float)(src.bottom-src.top) );
	}

	inline void to( RECT &out ) const {
		out.left   = x;		out.right  = r();
		out.top    = y;		out.bottom = b();
	}

	axRect2( const RECT &src )			{ set(src ); }
	RECT to_RECT() const				{ RECT t; to(t); return t; }
#endif //axOS_WIN

#ifdef axOS_iOS
	inline void set( const CGRect &src ) {
		set( src.origin.x, src.origin.y, src.size.width, src.size.height );
	}

	inline void to( CGRect	&out ) const {
		out.origin.x = x;		out.origin.y = y;
		out.size.width = w;		out.size.height = h;
	}

	axRect2( const CGRect &src )			{ set(src ); }
	CGRect to_CGRect() const			{ CGRect t; to(t); return t; }
#endif

#ifdef axOS_MacOSX
	inline void set( const NSRect &src ) {
		set( src.origin.x, src.origin.y, src.size.width, src.size.height );
	}

	inline void to( NSRect	&out ) const {
		out.origin.x = x;		out.origin.y = y;
		out.size.width = w;		out.size.height = h;
	}

	axRect2( const NSRect &src )			{ set(src ); }
	NSRect to_NSRect() const			{ NSRect t; to(t); return t; }
#endif 

};

template< class T > inline axRect2i to_axRect2i( const axRect2<T>& v ) { return axRect2i( (int   )v.x, (int   )v.y, (int   )v.w, (int   )v.h ); }
template< class T > inline axRect2f to_axRect2f( const axRect2<T>& v ) { return axRect2f( (float )v.x, (float )v.y, (float )v.w, (float )v.h ); }
template< class T > inline axRect2d to_axRect2d( const axRect2<T>& v ) { return axRect2d( (double)v.x, (double)v.y, (double)v.w, (double)v.h ); }


//@}

#ifdef axOS_iOS
inline
axStatus axStringFormat_out( axStringFormat &f, const ::CGRect &value ) {
	return axStringFormat_out( f, axRect2f( value ) );
}
	
inline CGRect to_CGRect( const axRect2f &rc ) {
	return CGRectMake( rc.x, rc.y, rc.w, rc.h );
}	
#endif

#ifdef axOS_MacOSX
inline
axStatus axStringFormat_out( axStringFormat &f, const ::NSRect &value ) {
	return axStringFormat_out( f, axRect2f( value ) );
}
#endif


axStatus ax_str_to( const char* sz, axRect2i &v );
axStatus ax_str_to( const char* sz, axRect2f &v );
axStatus ax_str_to( const char* sz, axRect2d &v );

axStatus ax_str_to( const wchar_t* sz, axRect2i &v );
axStatus ax_str_to( const wchar_t* sz, axRect2f &v );
axStatus ax_str_to( const wchar_t* sz, axRect2d &v );

template<class T>
inline axRect2<T> ax_lerp( const axRect2<T> &a, const axRect2<T> &b, float d ) {
	return axRect2<T> ( ax_lerp(a.x, b.x, d ),
                            ax_lerp(a.y, b.y, d ),
                            ax_lerp(a.w, b.w, d ),
                            ax_lerp(a.h, b.h, d ) );
}

//--------------- inline ---------------------
template <class T> inline axRect2i to_Rect2i( const axRect2<T> &v ) { return axRect2i( (int   )v.x, (int   )v.y, (int   )v.w, (int   )v.h );    }
template <class T> inline axRect2f to_Rect2f( const axRect2<T> &v ) { return axRect2f( (float )v.x, (float )v.y, (float )v.w, (float )v.h );  }
template <class T> inline axRect2d to_Rect2d( const axRect2<T> &v ) { return axRect2d( (double)v.x, (double)v.y, (double)v.w, (double)v.h ); }

template <class T>
inline bool axRect2<T>::isOverlap( const axRect2<T> &r ) const {
    if( x <= r.x ) {
            if( r.x >= x+w ) return false;
    }else{
            if( x >= r.x+r.w ) return false;
    }

    if( y <= r.y ) {
            if( r.y >= y+h ) return false;
    }else{
            if( y >= r.y+r.h ) return false;
    }
    return true;
}

template <class T>
inline bool axRect2<T>::intersectOf( const axRect2<T> &a, const axRect2<T> &b ) {
	if ( !a.isOverlap(b) ) {
		setAll(0); return false;
	}
	x = ax_max( a.x, b.x);
	y = ax_max( a.y, b.y);
	w = ax_min( a.x+a.w, b.x+b.w ) - x;
	h = ax_min( a.y+a.h, b.y+b.h ) - y;
	return true;
}

template< class T> inline
axRect2<T>  axRect2<T>::absSize ()	const {
	axRect2<T>	out = *this;
	if( w < 0 ) { out.x = x+w; out.w = -w; }
	if( h < 0 ) { out.y = y+h; out.h = -h; }
	return out; 
}

template<class T> inline 
axStatus axRect2<T>::toStringFormat( axStringFormat &f ) const {
	return f.format( "( {?}, {?}, {?}, {?} )", x,y,w,h );
}

template<class T>
template<class S> inline
axStatus axRect2<T>::serialize_io( S &s ) {
	axStatus st;
	st = s.io( x );	if( !st ) return st;
	st = s.io( y );	if( !st ) return st;
	st = s.io( w );	if( !st ) return st;
	st = s.io( h );	if( !st ) return st;
	return 0;
}

template<class T>
axRect2<T> axRect2<T>::operator *  ( const axMatrix4<T> &m	) const {
	axRect2<T> tmp;
	tmp.pos() = pos() * m;
	tmp.size() = bottomRight() * m - tmp.pos();
	return tmp;
}


template<class T>
void axRect2<T>::unionRect( T xx, T yy, T ww, T hh ) {
	if( w == 0 || h == 0 ) {
		set( xx, yy, ww, hh );
	}else {
	
		axVec2<T> lt = pos();
		ax_min_it( lt, axVec2<T>( xx, yy) );
		axVec2<T> br = bottomRight();
		ax_max_it( br, axVec2<T>( xx+ww, yy+hh) );
		
		setPos( lt );
		setSize( br - lt );
		
	}
}




#endif // __axRect2_h__

