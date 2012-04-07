#ifndef __axFrustum3_h__
#define __axFrustum3_h__

#include "axMatrix4.h"
#include "axBBox3.h"
#include "axPlane3.h"

template<class T>
class axFrustum3 {
public:
	axFrustum3();
	axFrustum3( const axMatrix4<T>&	projection );

	void set( const axMatrix4<T>&	projection );

	bool	isInside	( const axVec3<T>&  pt );
	bool	isIntersect	( const axBBox3<T>& bb );

	bool		valid;
	axVec3<T>		pt[8];

	axFrustum3<T> operator * ( const axMatrix4<T>& m );
	void operator *= ( const axMatrix4<T>& m );

	const axPlane3<T>*	planes() const; //near, far, top, bottom, left, right

	template<class S>	axStatus	serialize_io	( S &se );

private:
	axPlane3<T>	_plane[6];
	bool		plane_dirty;
	void _ctor();
};

typedef axFrustum3<float>	axFrustum3f;
typedef axFrustum3<double>	axFrustum3d;


template<class T> inline
bool axFrustum3<T>::isInside( const axVec3<T>& pt ) {
	const axPlane3<T>	*p = planes();
	if( ! p[0].isFront( pt ) ) return false;
	if( ! p[1].isFront( pt ) ) return false;
	if( ! p[2].isFront( pt ) ) return false;
	if( ! p[3].isFront( pt ) ) return false;
	if( ! p[4].isFront( pt ) ) return false;
	if( ! p[5].isFront( pt ) ) return false;
	return true;
}

template<class T> inline
bool axFrustum3<T>::isIntersect( const axBBox3<T>& bb ) {
// http://www.lighthouse3d.com/opengl/viewfrustum/index.php?gatest3
	if( bb.isEmpty() ) return false;

	const axPlane3<T> *p = planes();
	if( ! p[0].isFront( bb ) ) return false;
	if( ! p[1].isFront( bb ) ) return false;
	if( ! p[2].isFront( bb ) ) return false;
	if( ! p[3].isFront( bb ) ) return false;
	if( ! p[4].isFront( bb ) ) return false;
	if( ! p[5].isFront( bb ) ) return false;

	return true;
}

#endif //__axFrustum3_h__

