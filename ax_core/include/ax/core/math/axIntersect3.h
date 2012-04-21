#ifndef __axIntersect3_h__
#define __axIntersect3_h__

#include "axVec3.h"
#include "axRay3.h"
#include "axPlane3.h"

template< class T>
class axIntersect3 {
public:
	static bool ray_plane	(	axVec3<T> &hitPoint, T& distance, const axRay3<T>& ray, 
								const axPlane3<T>& plane, bool doubleSide );

	static bool ray_triangle(	axVec3<T> &hitPoint, T& distance, const axRay3<T>& ray, 
								const axVec3<T>& v0, const axVec3<T>& v1, const axVec3<T>& v2, bool doubleSide );
};

typedef axIntersect3<float>		Intersect3f;
typedef axIntersect3<double>	Intersect3d;

#endif //__axIntersect3_h__

