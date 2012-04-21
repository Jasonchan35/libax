
#ifndef __axEulerRotation_h__
#define __axEulerRotation_h__

#include "axVec3.h"

//! \addtogroup math
//!@{

template<class T> class axMatrix4;
template<class T> class axQuaternion;

template<class T>
class axEulerRotation3 {
public:
	enum {
		o_xyz = 0,
		o_yzx,
		o_zxy,
		o_xzy,
		o_yxz,
		o_zyx
	};

	int o;			//!< rotation order
	axVec3<T> a;	//!< rotation angle.

	axEulerRotation3( int oo=o_xyz ) : o(oo), a(0, 0, 0) {}
	axEulerRotation3( const axVec3<T>  &aa, int oo=o_xyz ) : o(oo), a(aa) {}

	void			set( T x, T y, T z, int oo=o_xyz )			{ a.set(x,y,z); o = oo; }
	void			set( const axVec3<T> &aa, int oo=o_xyz )	{ a = aa; o = oo; }
	void			set( const axMatrix4<T> &m );

	axMatrix4<T>	to_Matrix4		() const;
	axQuaternion<T>	to_Quaternion	() const;

	const char*		order_name		() const;

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;
};

typedef axEulerRotation3<float>		axEulerRotation3f;
typedef axEulerRotation3<double>	axEulerRotation3d;

template<class T> template<class S> inline
axStatus	axEulerRotation3<T>::serialize_io( S &se ) {
	axStatus st;
	st = se.io( o );	if( !st ) return st;
	st = se.io( a );	if( !st ) return st;
	return 0;
}

template< class T> inline 
axStatus axEulerRotation3<T>::toStringFormat( axStringFormat &f ) const{
	return f.out( a );
}

//!@}

#endif //__axEulerRotation_h__
