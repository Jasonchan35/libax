#ifndef __axRay3_h__
#define __axRay3_h__

#include "axVec2.h"
#include "axVec3.h"
#include "axRect2.h"
#include "axMatrix4.h"
#include "axPlane3.h"

template<class T>
class axRay3 {
public:
	class HitResult;

	axRay3() {}
	axRay3( const axVec3<T> &origin, const axVec3<T> &direction ) : o(origin), v(direction) {}

	void	set			( const axVec3<T> &origin, const axVec3<T> &direction ) { o=origin;  v=direction; }

	bool	unprojectFromMatrix			( const axVec2<T>    &pointInView,		 const axRect2<T>   &view,
										  const axMatrix4<T> &projection,		 const axMatrix4<T> &modelView );
	
	bool	unprojectFromInverseMatrix	( const axVec2<T>	 &pointInView,		 const axRect2<T>	&view,
										  const axMatrix4<T> &projectionInverse, const axMatrix4<T> &modelViewInvsere );

	bool	hitTest			( HitResult &result, const axPlane3<T> &plane, bool test_double_side ) const;
	bool	hitTestTriangle ( HitResult &result, const axVec3<T> &p0, const axVec3<T> &p1, const axVec3<T> &p2, bool test_double_side ) const;
	
	axRay3		mul4x4		( const axMatrix4<T> &m )		{ return axRay3( o.mul4x4(m), v.mul4x4(m).normalize() ); }
	void		mul4x4_it	( const axMatrix4<T> &m )		{ *this = this->mul4x4(m); }

	axVec3<T>	getPoint( T distance ) const {  return o + v * distance; }

	axVec3<T>	o;	//!< origin
	axVec3<T>	v;	//!< direction

	class HitResult {
	public:
		HitResult()		{ reset(); }
		void reset()	{ hit_count = 0; }
	
		axSize			hit_count;
		T				distance;
		axVec3<T>		pt;			//!< hit point
		axVec3<T>		normal;		//!< this normal on the hit surface
		
		bool _set_result( T _distance, const axVec3<T> &_pt, const axVec3<T> &_normal );
	};

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;
};


typedef axRay3<float>	axRay3f;
typedef axRay3<double>	axRay3d;


template<class T> inline 
axStatus axRay3<T>::toStringFormat( axStringFormat &f ) const{
	return f.format( "<{?}, {?}>", o, v );
}

template<class T>
template<class S> inline
axStatus axRay3<T>::serialize_io( S &s ) {
	axStatus st;
	st = s.io( o );	if( !st ) return st;
	st = s.io( v );	if( !st ) return st;
	return 0;
}


#endif //__axRay3_h__
