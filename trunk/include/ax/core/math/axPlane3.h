#ifndef __axPlane3_h__
#define __axPlane3_h__

#include "axMatrix4.h"
#include "axBBox3.h"

//! N dot P + d = 0
template< class T>
class axPlane3 {
public:
	axPlane3() {}
	axPlane3( const axVec3<T> &pp, const axVec3<T> &nn ) : p(pp), n(nn) {};

	void	set		( const axVec3<T> &pp, const axVec3<T> &nn )	{ p=pp; n=nn; }
	
	//! can be negative when pt behind the plane
	T		distance( const axVec3<T> & pt ) const					{ return n.dot(pt-p); }

	bool	isFront	( const axVec3<T> & pt ) const					{ return distance(pt) >= 0; }
	//! also include intersect
	bool	isFront	( const axBBox3<T>& bb ) const;

	//! d in "N dot P + d = 0"
	T	d() const { return -p.dot( n ); }

	void   operator*=	( const axMatrix4<T> &mat )	{ p*=mat; n*=mat; }
	axPlane3 operator*  ( const axMatrix4<T> &mat )	const { return axPlane3( p*mat, (n*mat).normal() ); }

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;

	axVec3<T>	p;	//!< point
	axVec3<T>	n;	//!< normal
};

typedef axPlane3<float>		axPlane3f;
typedef axPlane3<double>	axPlane3d;


template<class T> inline
bool axPlane3<T>::isFront( const axBBox3<T>& bb ) const {
	if( bb.isEmpty() ) return true;

	axVec3<T>	a, b;

	if( n.x >= 0 ) {
		a.x = bb.maxPoint().x;
		b.x = bb.minPoint().x;
	}else{
		a.x = bb.minPoint().x;
		b.x = bb.maxPoint().x;
	}

	if( n.y >= 0 ) {
		a.y = bb.maxPoint().y;
		b.y = bb.minPoint().y;
	}else{
		a.y = bb.minPoint().y;
		b.y = bb.maxPoint().y;
	}

	if( n.z >= 0 ) {
		a.z = bb.maxPoint().z;
		b.z = bb.minPoint().z;
	}else{
		a.z = bb.minPoint().z;
		b.z = bb.maxPoint().z;
	}

	if( (a - p).dot( n ) < 0 ) return false; //outside
//	if( (b - p).dot( n ) < 0 ) return true;  //intersect
	return true; //inside
}



#endif //__axPlane3_h__
