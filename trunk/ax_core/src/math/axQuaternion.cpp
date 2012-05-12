#include <ax/core/math/axQuaternion.h>
#include <ax/core/math/axEulerRotation3.h>
#include <ax/core/math/axMatrix4.h>

template<class T>
axQuaternion<T>	axQuaternion<T>::operator*( const axQuaternion<T> &rq ) const {
	// the constructor takes its arguments as (x, y, z, w)
	return axQuaternion(c.w * rq.c.x + c.x * rq.c.w + c.y * rq.c.z - c.z * rq.c.y,
	                  c.w * rq.c.y + c.y * rq.c.w + c.z * rq.c.x - c.x * rq.c.z,
	                  c.w * rq.c.z + c.z * rq.c.w + c.x * rq.c.y - c.y * rq.c.x,
	                  c.w * rq.c.w - c.x * rq.c.x - c.y * rq.c.y - c.z * rq.c.z);

}

template<class T> void axQuaternion<T>::setRotate( T angle, const axVec3<T> &axis) {
	axVec3<T> vn = axis.normalize();
 	angle *= 0.5;
	T sinAngle = sin(angle);
	c.x = (vn.x * sinAngle);
	c.y = (vn.y * sinAngle);
	c.z = (vn.z * sinAngle);
	c.w = cos(angle);
}


template<class T> void axQuaternion<T>::setAlign( const axVec3<T> &src, const axVec3<T> &dst) {
	// reference: http://www.essentialmath.com/Quaternions.pps
	T src_dot_dst = src.dot( dst );
	if ( src_dot_dst <= -1 ) {
		// any perpendicular vector can be the rotation axis
		if ( src.x == 0 ) { c.set( 1, 0, 0, 0 ); return; }
		if ( src.y == 0 ) { c.set( 0, 1, 0, 0 ); return; }
		if ( src.z == 0 ) { c.set( 0, 0, 1, 0 ); return; }
		c.set( -src.y, src.x, 0, 0 );
	} else if ( src_dot_dst >= 1 ) {
		c.set( 0, 0, 0, 1 );	return;
	} else {
		//double s = sqrt( 2 * ( 1 + src_dot_dst ) );
		//axVec3<T> r;	r.cross( src, dst );
		//c.set( r.x / s, r.y / s, r.z / s, 0.5f * s );
		setRotate( acos( src_dot_dst ), ( src ^ dst ).normalize() );
	}
	normalize();
}

template<class T> axQuaternion<T> axQuaternion<T>::inv() const {
	T n = norm_sq();
	if ( n == 0 ) { return axQuaternion(0,0,0,0); }
	return axQuaternion( -c.x / n, -c.y / n, -c.z / n, c.w / n );
}

template<class T> axQuaternion<T> axQuaternion<T>::normal() const {
	T n = norm();
	if ( n == 0 ) { return axQuaternion(0,0,0,0); }
	return axQuaternion( c / n );
}

template<class T> axQuaternion<T> axQuaternion<T>::sphericalLerp( const axQuaternion &q, T t) const {
	T theta = acos( c.dot( q.c ) );
	T sin_theta = sin(theta);

	if( theta == 0 ) return *this;

	return axQuaternion(( c   * (sin(theta * (1-t)) / sin_theta) )
					+ ( q.c * (sin(theta *   t  ) / sin_theta) ) );
}

template<class T> bool axQuaternion<T>::getRotate(T &angle, axVec3<T> &axis) const {
	if ( c.w == 1 ) {
		angle = 0;
		axis = axVec3<T>(c.x, c.y, c.z).normalize();
		return false;
	}
	angle = 2 * acos(c.w);
	axis.set(c.x, c.y, c.z);
	axis /= sqrt( 1-c.w*c.w );
	return true;
}

template<class T>
axMatrix4<T> axQuaternion<T>::to_Matrix4() const{
	T x2 = c.x * c.x;
	T y2 = c.y * c.y;
	T z2 = c.z * c.z;
	T xy = c.x * c.y;
	T xz = c.x * c.z;
	T yz = c.y * c.z;
	T wx = c.w * c.x;
	T wy = c.w * c.y;
	T wz = c.w * c.z;

	// This calculation would be a lot more complicated for non-unit length quaternions
	// Note: The constructor of axMatrix4 expects the Matrix in column-major format like expected by
	//   OpenGL
	return axMatrix4<T>(	1 -2*(y2+z2),	2*(xy-wz),		2*(xz+wy),		0,
		 				2*(xy+wz),		1 -2*(x2 + z2),	2*(yz-wx),		0,
						2*(xz-wy),		2*(yz + wx),	1 -2*(x2+y2),	0,
						0,				0,				0,				1);
}


template<class T>
axEulerRotation3<T> axQuaternion<T>::to_EulerRotation3() const {
	T sqw = c.w * c.w;
	T sqx = c.x * c.x;
	T sqy = c.y * c.y;
	T sqz = c.z * c.z;
	T p = 2 * ( c.y * c.w - c.x * c.z );

	axVec3<T> v;

	v.x = atan2( 2 * ( c.y * c.z + c.x * c.w ) , -sqx - sqy + sqz + sqw );
	if ( p >= 1 ) {
		v.y = (T)ax_math_PI * (T)0.5;
	}else if ( p <= -1 ) {
		v.y = (T)-ax_math_PI * (T)0.5;
	}else{
		v.y = asin( p );
	}

	v.z = atan2( 2 * ( c.x * c.y + c.z * c.w ) ,  sqx - sqy - sqz + sqw );

	return axEulerRotation3<T>( v );
}

template<class T> axVec3<T> axQuaternion<T>::rotate( const axVec3<T> &v ) const {
	T w2 = 2 * c.w;
	T p_mul = w2 * c.w - 1;
	
	axVec3<T>	n ( c.x, c.y, c.z );
	
	T v_mul = 2 * n.dot( v );
	axVec3<T> c_cross_v = n ^ v;
	
	return axVec3<T>( p_mul * v.x + v_mul * n.x + w2 * c_cross_v.x,
					p_mul * v.y + v_mul * n.y + w2 * c_cross_v.y,
					p_mul * v.z + v_mul * n.z + w2 * c_cross_v.z );
}

template<class T>
void axQuaternion<T>::setAlignOnPlane(	const axVec3<T> &normal,
										const axVec3<T> &src,
										const axVec3<T> &dst,
										T epsilon )
{
	if ( src.isParallel( normal, epsilon )
		|| dst.isParallel( normal, epsilon )
		|| normal.isAll(0) ) {
		c.set( 0, 0, 0, 1 );	return;
	}

	axVec3<T>	vector_s = (src - ( normal * src.dot(normal) ) ).normalize();
	axVec3<T>	vector_d = (dst - ( normal * dst.dot(normal) ) ).normalize() ;

	T sdotd = vector_s.dot(vector_d);

	if ( ax_abs( sdotd + 1.0 ) < epsilon ) {
		setRotate( (T)ax_math_PI, normal );
	} else {
		setAlign( vector_s, vector_d );
	}
}

//---- The explicit instantiation ----
template class axQuaternion<float>;
template class axQuaternion<double>;

