#ifndef __axMargin2_h__
#define __axMargin2_h__

#include "axMatrix4.h"
#include "axPlane3.h"
#include "axBBox3.h"

template<class T> class axMatrix4;

template<class T>
class axMargin2 {
public:
	axMargin2	() 			{ setAll(0); }
	axMargin2	( T _all ) 									: top(_all),  bottom(_all),    left(_all),  right(_all) {}
	axMargin2	( T _top, T _bottom, T _left, T _right ) 	: top(_top),  bottom(_bottom), left(_left), right(_right) {}

	void		set			( T	_top, T _bottom, T _left, T _right );
	void		setAll		( T v ) 							{ set( v,v,v,v ); }

	bool		isAll		( const T &v ) const				{ return ( top==v && bottom==v && left==v && right==v ); }

	T			horizontal	() const							{ return left+right; }
	T			vertical	() const							{ return top+bottom; }
	axVec2<T>	both		() const							{ return axVec2<T>( horizontal(), vertical() ); }

	axVec2<T>	leftTop		() const							{ return axVec2<T>( left, top ); }
	axVec2<T>	rightBottom	() const							{ return axVec2<T>( right, bottom ); }
	
	
	T	top, bottom, left, right;

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;
};

typedef axMargin2<float>	axMargin2f;
typedef axMargin2<int>		axMargin2i;

template<class T> inline 
void axMargin2<T> :: set( T	_top, T _bottom, T _left, T _right ) {
	top    = _top;
	bottom = _bottom;
	left   = _left;
	right  = _right;
}

template<class T> inline 
axStatus axMargin2<T> :: toStringFormat( axStringFormat& f ) const {
	return f.format(L"margin<{?}, {?}, {?}, {?}>", top, bottom, left, right );
}

#endif //__axMargin2_h__
