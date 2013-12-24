#include <ax/core/math/axRay3.h>

template<class T>
bool axRay3<T>::unprojectFromMatrix( const axVec2<T>    &pointInView, const axRect2<T>   &view, 
									 const axMatrix4x4<T> &projection,  const axMatrix4x4<T> &modelView ) 
{
	if( view.w == 0 || view.h == 0 ) return false;	
	return unprojectFromInverseMatrix( pointInView, view, projection.inverse(), modelView.inverse() );
}

template<class T>
bool axRay3<T>::unprojectFromInverseMatrix ( const axVec2<T>    &pointInView,		const axRect2<T>   &view,
											 const axMatrix4x4<T> &projectionInverse, const axMatrix4x4<T> &modelViewInvsere ) 
{
	if( view.w == 0 || view.h == 0 ) return false;
	
	//Convert the point's coords from screen space (0 to wholeViewport width/height) to proj space (-1 to 1)
	axVec4<T> p0( ( pointInView.x - view.x) / view.w * 2 - 1, 
				 -((pointInView.y - view.y) / view.h * 2 - 1), 
				 0, 1 );
	
	axVec4<T>	p1( p0.x, p0.y, 1, 1 );
	
	p0 *= projectionInverse;
	p0 *= modelViewInvsere;	
	
	p1 *= projectionInverse;
	p1 *= modelViewInvsere;
	
	if( p0.w == 0 ) return false;
	if( p1.w == 0 ) return false;
	
	o = p0.to_Vec3();
	v = (p1.to_Vec3() - o).normalize();
	return true;
}


template<class T>
bool	axRay3<T>::hitTest( HitResult &result, const axPlane3<T> &plane, bool test_double_side ) const
{
	T	pd = plane.d();

	T	s = v.dot( plane.n );
	if( ax_math_equals( s, (T)0 ) ) return false;

	bool back_face =  (s > 0);
	if( ! test_double_side && back_face ) return false;

	T	t = -( o.dot( plane.n ) + pd ) / s;
	if( t < 0 ) return false;

	return result._set_result( t, o + ( v * t ), back_face ? -plane.n : plane.n );
}

template<class T>
bool	axRay3<T>::hitTestTriangle( HitResult &result, const axVec3<T> &p0, const axVec3<T> &p1, const axVec3<T> &p2, bool test_double_side ) const
{
	axVec3<T> face_n = ax_tri_normal( p0, p1, p2 );
	T pd = -p0.dot( face_n );

	T s = v.dot( face_n );
	if( ax_math_equals( s, (T) 0 ) ) return false; // parallel to the plane

	bool back_face =  (s > 0);

	if( !test_double_side && back_face ) return false;

	T t = -( o.dot( face_n ) + pd ) / s;
	if( t < 0 ) return false;

	axVec3<T>	hp = o + ( v * t );

	axVec3<T> e0 = p0 - hp;
	axVec3<T> e1 = p1 - hp;
	axVec3<T> e2 = p2 - hp;

//in triangle test
	if( s > 0 ) {
		axVec3<T>	n0 = e1.cross(e0).normalize();
		if( ( hp.dot( n0 ) - o.dot( n0 )  ) > 0 ) return false;

		axVec3<T>	n1 = e2.cross(e1).normalize();
		if( ( hp.dot( n1 ) - o.dot( n1 )  ) > 0 ) return false;

		axVec3<T>	n2 = e0.cross(e2).normalize();
		if( ( hp.dot( n2 ) - o.dot( n2 )  ) > 0 ) return false;
	}else{
		axVec3<T>	n0 = e1.cross(e0).normalize();
		if( ( hp.dot( n0 ) - o.dot( n0 )  ) < 0 ) return false;

		axVec3<T>	n1 = e2.cross(e1).normalize();
		if( ( hp.dot( n1 ) - o.dot( n1 )  ) < 0 ) return false;

		axVec3<T>	n2 = e0.cross(e2).normalize();
		if( ( hp.dot( n2 ) - o.dot( n2 )  ) < 0 ) return false;
	}
	
	return result._set_result( t, hp, back_face ? -face_n : face_n );	
}

template<class T>
bool axRay3<T> :: HitResult :: _set_result( T _distance, const axVec3<T> &_pt, const axVec3<T> &_normal ) {
	if( ! hit_count || _distance < distance ) {
		distance = _distance;
		pt = _pt;
		normal = _normal;
		hit_count++;
		return true;
	}else{
		hit_count++;
		return false;
	}
}

//---- The explicit instantiation ----
template class axRay3<float>;
template class axRay3<double>;
