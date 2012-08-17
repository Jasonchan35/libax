
#ifndef __axQuaternion_h__
#define __axQuaternion_h__

#include "axVec3.h"
#include "axVec4.h"

//! \addtogroup math
//!@{

template<class T> class axMatrix4;
template<class T> class axEulerRotation3;

template<class T>
class axQuaternion {
public:
	axVec4<T> c;	//!< the coeff.
	axQuaternion() {}
	axQuaternion( const axQuaternion& v ) : c(v.c) {}
	axQuaternion( const axVec4<T> &cc ) : c(cc)   {}
	axQuaternion( T xx, T yy, T zz, T ww ) : c(xx,yy,zz,ww) {}

	axQuaternion( const axVec3<T> &src, const axVec3<T> &dst)	{ setAlign(src, dst); }
	axQuaternion( T rad, const axVec3<T> &axis)					{ setRotate( rad, axis ); }

	axQuaternion	operator+	( const axQuaternion &v ) const	{ return axQuaternion( c + v.c ); }
	axQuaternion	operator-	( const axQuaternion &v ) const	{ return axQuaternion( c - v.c ); }
	axQuaternion	operator*	( const axQuaternion &v ) const;

	void		operator+=	( const axQuaternion &val )		{ c+=val.c; }
	void		operator-=	( const axQuaternion &val )		{ c-=val.c; }
	void		operator*=	( const axQuaternion &val )		{ (*this) = (*this) * val; }

	bool		operator==	( const axQuaternion &val ) const { return c==val.c; }
	bool		operator!=	( const axQuaternion &val ) const { return c!=val.c; }

	T			norm		() const						{ return sqrt( norm_sq() ); }
	T			norm_sq		() const						{ return c.w*c.w+c.x*c.x+c.y*c.y+c.z*c.z; }

	axQuaternion	normal		() const;
	void			normalize	()								{ *this = normal(); }

	axQuaternion	conjugate	() const						{ return axQuaternion( -c.x, -c.y, -c.z, c.w ); }
	axQuaternion	inv			() const;

	void		setRotateX( T rad )						{ c.w = cos(rad/2); c.x = sin(rad/2); c.y = c.z = 0; }
	void		setRotateY( T rad )						{ c.w = cos(rad/2); c.y = sin(rad/2); c.x = c.z = 0; }
	void		setRotateZ( T rad )						{ c.w = cos(rad/2); c.z = sin(rad/2); c.x = c.y = 0; }

	void		setRotate	( T angle,  const axVec3<T> &axis);
	bool		getRotate	( T &angle, axVec3<T> &axis) const;

	axVec3<T>		rotate		( const axVec3<T> &v ) const;

	void		setAlign			( const axVec3<T> &src,    const axVec3<T> &dst);
	void		setAlignOnPlane		( const axVec3<T> &normal, const axVec3<T> &src,
								      const axVec3<T> &dst,    T epsilon = ax_epsilon<T>() );

	//! Spherical linear interpolation
	axQuaternion		sphericalLerp ( const axQuaternion &q, T t ) const;

	axMatrix4<T>		to_Matrix4() const;
	axEulerRotation3<T>	to_EulerRotation3() const;

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;

};

typedef axQuaternion<float>		axQuaternionf;
typedef axQuaternion<double>	axQuaterniond;


template<class T>
template<class S>
inline axStatus axQuaternion<T>::serialize_io( S &se ) {
	return se.io( c );
}

template<class T> inline 
axStatus axQuaternion<T>::toStringFormat( axStringFormat &p ) const{
	return p.out( c );
}

//!@}

#endif //__axQuaternion_h__
