#include <ax/core/math/axEulerRotation3.h>
#include <ax/core/math/axMatrix4x4.h>
#include <ax/core/math/axQuaternion.h>

template<class T>
const char* axEulerRotation3<T>::orderName() const {
	switch( o ) {
		case o_xyz: return"xyz";
		case o_yzx: return"yzx";
		case o_zxy: return"zxy";
		case o_xzy: return"xzy";
		case o_yxz: return"yxz";
		case o_zyx: return"zyx";
	}
	return "unknown";
}

template<class T>
axMatrix4x4<T> axEulerRotation3<T>::to_Matrix4() const {
	axMatrix4x4<T> m;
	if( a.isAll(0) ) {
		m.setIdentity();
		return m;
	}

#if 1
	T cx = ax_cos(a.x), cy = ax_cos(a.y), cz = ax_cos(a.z);
	T sx = ax_sin(a.x), sy = ax_sin(a.y), sz = ax_sin(a.z);

	switch (o) {
		case o_xyz: {
			m.cx.set(cy*cz,				cy*sz,			-sy,		0);
			m.cy.set(sx*sy*cz-cx*sz,	cx*cz+sx*sy*sz,	sx*cy,		0);
			m.cz.set(sx*sz+cx*sy*cz,	cx*sy*sz-sx*cz,	cx*cy,		0);
			m.cw.set(0,					0,				0,			1);
		}break;
		case o_yzx: {
			m.cx.set(cy*cz,	cx*cy*sz+sx*sy,	sx*cy*sz-cx*sy,			0);
			m.cy.set(-sz,	cx*cz,			sx*cz,					0);
			m.cz.set(sy*cz,	cx*sy*sz-sx*cy,	sx*sy*sz+cx*cy,			0);
			m.cw.set(0,					0,		0,					1);
		}break;
		case o_zxy: {
			m.cx.set(cy*cz+sx*sy*sz,	cx*sz,	-sy*cz+sx*cy*sz,	0);
			m.cy.set(-cy*sz+sx*sy*cz,	cx*cz,	sy*sz+sx*cy*cz,		0);
			m.cz.set(cx*sy,				-sx,	cx*cy,				0);
			m.cw.set(0,					0,		0,					1);
		}break;
		case o_xzy: {
			m.cx.set(cy*cz,				sz,		-sy*cz,				0);
			m.cy.set(-cx*cy*sz+sx*sy,	cx*cz,	cx*sy*sz+sx*cy,		0);
			m.cz.set(sx*cy*sz+cx*sy,	-sx*cz,	-sx*sy*sz+cx*cy,	0);
			m.cw.set(0,					0,		0,					1);
		}break;
		case o_yxz: {
			m.cx.set(cy*cz-sx*sy*sz,	cy*sz+sx*sy*cz,	-cx*sy,		0);
			m.cy.set(-cx*sz,			cx*cz,			sx,			0);
			m.cz.set(sy*cz+sx*cy*sz,	sy*sz-sx*cy*cz,	cx*cy,		0);
			m.cw.set(0,					0,				0,			1);
		}break;
		case o_zyx: {
			m.cx.set(cy*cz,		sx*sy*cz+cx*sz,	sx*sz-cx*sy*cz,		0);
			m.cy.set(-cy*sz,	cx*cz-sx*sy*sz,	cx*sy*sz+sx*cz,		0);
			m.cz.set(sy,		-sx*cy,			cx*cy,				0);
			m.cw.set(0,			0,				0,					1);
		}break;
		default: {
			assert(0 && "[axEulerRotation3::to_matrix] Invalid rotation order!");
			m.setIdentity();
		}
	}
#else

	axMatrix4x4<T> mx, my, mz;
	mx.set_rotate_x(a.x);
	my.set_rotate_y(a.y);
	mz.set_rotate_z(a.z);
	switch (o) {
		case o_xyz: m = mz * my * mx; break;
		case o_yzx: m = mx * mz * my; break;
		case o_zxy: m = my * mx * mz; break;
		case o_xzy: m = my * mz * mx; break;
		case o_yxz: m = mz * mx * my; break;
		case o_zyx: m = mx * my * mz; break;
		default: assert(false);
	}
#endif

	return m;
}

template<class T>
axQuaternion<T> axEulerRotation3<T>::to_Quaternion() const {
	axQuaternion<T> q;

	T c1, c2, c3, s1, s2, s3;
	switch (o) {
		case o_xyz: {
			c1 = ax_cos(a.x*(T)0.5); c2 = ax_cos(a.y*(T)0.5); c3 = ax_cos(a.z*(T)0.5);
			s1 = ax_sin(a.x*(T)0.5); s2 = ax_sin(a.y*(T)0.5); s3 = ax_sin(a.z*(T)0.5);
		}break;
		case o_yzx: {
			c1 = ax_cos(a.y*(T)0.5); c2 = ax_cos(a.z*(T)0.5); c3 = ax_cos(a.x*(T)0.5);
			s1 = ax_sin(a.y*(T)0.5); s2 = ax_sin(a.z*(T)0.5); s3 = ax_sin(a.x*(T)0.5);
		}break;
		case o_zxy: {
			c1 = ax_cos(a.z*(T)0.5); c2 = ax_cos(a.x*(T)0.5); c3 = ax_cos(a.y*(T)0.5);
			s1 = ax_sin(a.z*(T)0.5); s2 = ax_sin(a.x*(T)0.5); s3 = ax_sin(a.y*(T)0.5);
		}break;
		case o_xzy: {
			c1 = ax_cos(a.x*(T)0.5); c2 = ax_cos(a.z*(T)0.5); c3 = ax_cos(a.y*(T)0.5);
			s1 = ax_sin(a.x*(T)0.5); s2 = ax_sin(a.z*(T)0.5); s3 = ax_sin(a.y*(T)0.5);
		}break;
		case o_yxz: {
			c1 = ax_cos(a.y*(T)0.5); c2 = ax_cos(a.x*(T)0.5); c3 = ax_cos(a.z*(T)0.5);
			s1 = ax_sin(a.y*(T)0.5); s2 = ax_sin(a.x*(T)0.5); s3 = ax_sin(a.z*(T)0.5);
		}break;
		case o_zyx: {
			c1 = ax_cos(a.z*(T)0.5); c2 = ax_cos(a.y*(T)0.5); c3 = ax_cos(a.x*(T)0.5);
			s1 = ax_sin(a.z*(T)0.5); s2 = ax_sin(a.y*(T)0.5); s3 = ax_sin(a.x*(T)0.5);
		}break;
		default: {
            c1= c2= c3= s1= s2= s3 =0; //to fix the linux warning
			assert(false);
		}
	}

	return axQuaternion<T>(	s1*c2*c3 - c1*s2*s3,
							c1*s2*c3 + s1*c2*s3,
							c1*c2*s3 - s1*s2*c3,
							c1*c2*c3 + s1*s2*s3 );
}

template<class T>
void axEulerRotation3<T>::set( const axMatrix4x4<T> &m ) {
	T pole = (T)1.0 - ax_epsilon<T>();

	o = o_xyz;
	if ( m.cx.y > pole ) { // singularity at north pole
		a.x = ax_atan2( m.cy.x, m.cz.x );
		a.y = (T)(ax_math_PI / 2);
		a.z = 0;
	} else if ( m.cx.y < -pole ) { // singularity at south pole
		a.x = ax_atan2( m.cy.x, m.cz.x );
		a.y = (T)(-ax_math_PI / 2);
		a.z = 0;
	}else{
		a.x = ax_atan2( m.cy.z, m.cz.z );
		a.y = ax_math_equals( m.cx.z, (T)0 ) ? 0 : -asin( m.cx.z );
		a.z = ax_atan2( m.cx.y, m.cx.x );
	}
}


//---- The explicit instantiation ----
template class axEulerRotation3<float>;
template class axEulerRotation3<double>;

