#include <ax/core/math/axIntersect3.h>

template< class T>
bool axIntersect3<T>::ray_plane( axVec3<T> &hitPoint, T& distance, const axRay3<T>& ray, const axPlane3<T>& plane, bool doubleSide ) {
	T	pd = plane.d();

	T	s = ray.v.dot( plane.n );
	if( ax_math_equals( s, (T)0 ) ) return false;

	T	t = -( ray.o.dot( plane.n ) + pd ) / s;

	if( !doubleSide && s > 0 ) return false;

	if( t < 0 ) return false;

	hitPoint = ray.o + ( ray.v * t );
	distance = t;
	return true;
}


template< class T>
bool axIntersect3<T>::ray_triangle( axVec3<T> &hitPoint, T& distance, const axRay3<T>& ray, 
									const axVec3<T>& v0, const axVec3<T>& v1, const axVec3<T>& v2, bool doubleSide ) {
	axVec3<T> pn = ax_tri_normal( v0, v1, v2 );
	T pd = v0.dot( pn );

	T s = ray.v.dot( pn );
	if( ax_math_equals( s, (T) 0 ) ) return false; // parallel to the plane

	if( !doubleSide && s > 0 ) return false; //in back face

	T t = -( ray.o.dot( pn ) + pd ) / s;
	if( t < 0 ) return false;

	axVec3<T>	hp = ray.o + ( ray.v * t );

	axVec3<T> e0 = v0 - hp;
	axVec3<T> e1 = v1 - hp;
	axVec3<T> e2 = v2 - hp;

	if( s > 0 ) {
		axVec3<T>	n0 = e1.cross(e0).normalize();
		if( ( hp.dot( n0 ) - ray.o.dot( n0 )  ) > 0 ) return false;

		axVec3<T>	n1 = e2.cross(e1).normalize();
		if( ( hp.dot( n1 ) - ray.o.dot( n1 )  ) > 0 ) return false;

		axVec3<T>	n2 = e0.cross(e2).normalize();
		if( ( hp.dot( n2 ) - ray.o.dot( n2 )  ) > 0 ) return false;
	}else{
		axVec3<T>	n0 = e1.cross(e0).normalize();
		if( ( hp.dot( n0 ) - ray.o.dot( n0 )  ) < 0 ) return false;

		axVec3<T>	n1 = e2.cross(e1).normalize();
		if( ( hp.dot( n1 ) - ray.o.dot( n1 )  ) < 0 ) return false;

		axVec3<T>	n2 = e0.cross(e2).normalize();
		if( ( hp.dot( n2 ) - ray.o.dot( n2 )  ) < 0 ) return false;
	}

	distance = t;
	hitPoint = hp;
	return true;
}


//---- The explicit instantiation ----
template class axIntersect3<float>;
template class axIntersect3<double>;
