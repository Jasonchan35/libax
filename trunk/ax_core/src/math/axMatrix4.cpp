/*
 *  axMatrix4f.cpp
 *
 *  Created by jason on 04/Apr/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <ax/core/math/axMatrix4.h>
#include <ax/core/math/axEulerRotation3.h>
#include <ax/core/math/axQuaternion.h>


template<> const axMatrix4f axMatrix4f::kIdentity( 	1,0,0,0,
													0,1,0,0,
													0,0,1,0,
													0,0,0,1 );

template<> const axMatrix4d axMatrix4d::kIdentity( 	1,0,0,0,
													0,1,0,0,
													0,0,1,0,
													0,0,0,1);

template<class T>
void axMatrix4<T>::rotate ( const axEulerRotation3<T>  &er )	{ 
	operator*=( er.to_Matrix4() ); 
}

template<class T>
void axMatrix4<T>::rotate ( const axQuaternion<T>     &qu )	{ 
	operator*=( qu.to_Matrix4() ); 
}

template<class T>
axMatrix4<T> axMatrix4<T>::inverse() const {
   T wtmp[4][8];
   T m0, m1, m2, m3, s;
   T *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = e(0,0);
   r0[1] = e(0,1);
   r0[2] = e(0,2);
   r0[3] = e(0,3);
   r0[4] = 1.0;
   r0[5] = r0[6] = r0[7] = 0.0;

   r1[0] = e(1,0);
   r1[1] = e(1,1);
   r1[2] = e(1,2);
   r1[3] = e(1,3);
   r1[5] = 1.0;
   r1[4] = r1[6] = r1[7] = 0.0;

   r2[0] = e(2,0);
   r2[1] = e(2,1);
   r2[2] = e(2,2);
   r2[3] = e(2,3);
   r2[6] = 1.0;
   r2[4] = r2[5] = r2[7] = 0.0;

   r3[0] = e(3,0);
   r3[1] = e(3,1);
   r3[2] = e(3,2);
   r3[3] = e(3,3);
   r3[7] = 1.0;
   r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if ( ax_abs(r3[0]) > ax_abs(r2[0]) ) ax_swap(r3, r2);
   if ( ax_abs(r2[0]) > ax_abs(r1[0]) ) ax_swap(r2, r1);
   if ( ax_abs(r1[0]) > ax_abs(r0[0]) ) ax_swap(r1, r0);
   if ( 0.0 == r0[0] ) return kIdentity;

   /* eliminate first variable     */
   m1 = r1[0] / r0[0];
   m2 = r2[0] / r0[0];
   m3 = r3[0] / r0[0];
   s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
   s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
   s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
   s = r0[4]; if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r0[5]; if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r0[6]; if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r0[7]; if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if ( ax_abs(r3[1]) > ax_abs(r2[1]) ) ax_swap(r3, r2);
   if ( ax_abs(r2[1]) > ax_abs(r1[1]) ) ax_swap(r2, r1);
   if (0.0 == r1[1])  return kIdentity;

   /* eliminate second variable */
   m2 = r2[1]/r1[1];
   m3 = r3[1]/r1[1];
   r2[2] -= m2 * r1[2];
   r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3];
   r3[3] -= m3 * r1[3];
   s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if ( ax_abs(r3[2]) > ax_abs(r2[2]) ) ax_swap(r3, r2);
   if (0.0 == r2[2])  return kIdentity;

   /* eliminate third variable */
   m3 = r3[2]/r2[2];
   r3[3] -= m3 * r2[3];
   r3[4] -= m3 * r2[4],
   r3[5] -= m3 * r2[5];
   r3[6] -= m3 * r2[6],
   r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3]) return kIdentity;

   s = 1.0f/r3[3];             /* now back substitute row 3 */
   r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

   m2 = r2[3];                 /* now back substitute row 2 */
   s  = 1.0f/r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
   r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1;
   r1[5] -= r3[5] * m1;
   r1[6] -= r3[6] * m1;
   r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0;
   r0[5] -= r3[5] * m0;
   r0[6] -= r3[6] * m0;
   r0[7] -= r3[7] * m0;

   m1 = r1[2];                 /* now back substitute row 1 */
   s  = 1.0f/r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1);
   r1[5] = s * (r1[5] - r2[5] * m1);
   r1[6] = s * (r1[6] - r2[6] * m1);
   r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0;
   r0[5] -= r2[5] * m0;
   r0[6] -= r2[6] * m0;
   r0[7] -= r2[7] * m0;

   m0 = r0[1];                 /* now back substitute row 0 */
   s  = 1.0f/r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0);
   r0[5] = s * (r0[5] - r1[5] * m0);
   r0[6] = s * (r0[6] - r1[6] * m0);
   r0[7] = s * (r0[7] - r1[7] * m0);

   return axMatrix4( r0[4], r0[5], r0[6], r0[7],
				   r1[4], r1[5], r1[6], r1[7],
				   r2[4], r2[5], r2[6], r2[7],
				   r3[4], r3[5], r3[6], r3[7] );
}

template<class T>
void axMatrix4<T>::setAimZ_UpY ( const axVec3<T> &eye, const axVec3<T> &aim, const axVec3<T> &up, bool neg_z_aim ) {
	axVec3<T> f = ( eye - aim ).normalize();
	if( neg_z_aim ) f = -f;

	axVec3<T> s = (f ^ up).normalize();
	axVec3<T> u = s ^ f;

	cx.set(  s.x,  s.y,  s.z, 0.0 );
	cy.set(  u.x,  u.y,  u.z, 0.0 );
	cz.set(  f.x,  f.y,  f.z, 0.0 );
	cw.set(  eye, 1 );
}


template<class T>
void axMatrix4<T>::setLookAt ( const axVec3<T> &eye, const axVec3<T> &aim, const axVec3<T> &up ) {
	axVec3<T> f = ( aim - eye ).normalize();
	axVec3<T> s = (f ^ up).normalize();
	axVec3<T> u = s ^ f;

	cx.set(  s.x,  u.x, -f.x, 0.0 );
	cy.set(  s.y,  u.y, -f.y, 0.0 );
	cz.set(  s.z,  u.z, -f.z, 0.0 );
	cw.set(  0,    0,   0,    1 );

	translate( -eye );
}

template<class T>
void axMatrix4<T>::setOrtho( T left, T right, T bottom, T top, T zNear, T zFar ) {
	T w = right - left;
	T h = top - bottom;
	T d = zFar - zNear;

	if( w == 0 || h == 0 || d == 0 ) {
		setIdentity();
	}else{
		set( 2/w,   0,    0,  0,
			   0, 2/h,    0,  0,
			   0,   0, -2/d,  0,
		  -(right+left) / w, -(top+bottom) / h, -(zFar+zNear ) / d, 1 );
	}
}

template<class T>
void axMatrix4<T>::setPerspective( T fovy_rad, T aspect, T zNear, T zFar ) {
	T s, c, deltaZ;
	T fov = fovy_rad / 2;
	
	deltaZ = zFar - zNear;
	s = sin( fov );
	
	if ((deltaZ == 0) || (s == 0) || (aspect == 0)) {
		setIdentity();
		return;
	}
	c = cos(fov) / s;
	
	cx.set( c / aspect, 0,   0,                          0  );
	cy.set( 0,          c,   0,                          0  );
	cz.set( 0,          0,   -(zFar + zNear) / deltaZ,   -1 );
	cw.set( 0,          0,   -2 * zNear * zFar / deltaZ, 0  );
}


//------- inline ------------

template< class T>
axStatus axMatrix4<T>::toStringFormat( axStringFormat &f ) const {
    f.out("\n");
	int i;
	for( i=0; i<4; i++ ) {
		f.format( "[{?},{?},{?},{?}]\n", e(i,0), e(i,1), e(i,2), e(i,3) );
	}
	return 0;
}

//=================================================
template<class T>
axMatrix4<T>::axMatrix4( T v00, T v01, T v02, T v03,
						T v10, T v11, T v12, T v13,
						T v20, T v21, T v22, T v23,
						T v30, T v31, T v32, T v33 )
: cx( v00, v01, v02, v03 ),
cy( v10, v11, v12, v13 ),
cz( v20, v21, v22, v23 ),
cw( v30, v31, v32, v33 ) {}

template<class T>
void axMatrix4<T>::set( T v00, T v01, T v02, T v03,
					   T v10, T v11, T v12, T v13,
					   T v20, T v21, T v22, T v23,
					   T v30, T v31, T v32, T v33 )
{
	cx.set( v00, v01, v02, v03 );
	cy.set( v10, v11, v12, v13 );
	cz.set( v20, v21, v22, v23 );
	cw.set( v30, v31, v32, v33 );
}


template <class T>
void axMatrix4<T>::setTranslate ( const axVec3<T> &v )	{
	cx.set(   1,   0,   0,   0 );
	cy.set(   0,   1,   0,   0 );
	cz.set(   0,   0,   1,   0 );
	cw.set( v.x, v.y, v.z,   1 );
}


template <class T>
void axMatrix4<T>::translate ( const axVec3<T> &v ) {
	axMatrix4<T> t; t.setTranslate( v );
	operator*=(t);
}

template <class T>
void axMatrix4<T>::setRotateX ( T rad ) {
	T s = ax_sin ( rad ), c = ax_cos ( rad );
	cx.set( 1, 0, 0, 0 );
	cy.set( 0, c, s, 0 );
	cz.set( 0,-s, c, 0 );
	cw.set( 0, 0, 0, 1 );
}

template <class T>
void axMatrix4<T>::setRotateY ( T rad ) {
	T s = ax_sin ( rad ), c = ax_cos ( rad );
	cx.set( c, 0,-s, 0 );
	cy.set( 0, 1, 0, 0 );
	cz.set( s, 0, c, 0 );
	cw.set( 0, 0, 0, 1 );
}

template <class T>
void axMatrix4<T>::setRotateZ ( T rad ) {
	T s = ax_sin ( rad ), c = ax_cos ( rad );
	cx.set( c, s, 0, 0 );
	cy.set(-s, c, 0, 0 );
	cz.set( 0, 0, 1, 0 );
	cw.set( 0, 0, 0, 1 );
}

template <class T>
void axMatrix4<T>::setScale ( const axVec3<T> &v ) {
	cx.set( v.x,   0,   0, 0 );
	cy.set(   0, v.y,   0, 0 );
	cz.set(   0,   0, v.z, 0 );
	cw.set(   0,   0,   0, 1 );
}

template <class T>
void axMatrix4<T>::setShear ( const axVec3<T> &v ) {
	const T &xy = v.x;
	const T &xz = v.y;
	const T &yz = v.z;
	
	cx.set(  1, 0, 0, 0 );
	cy.set( xy, 1, 0, 0 );
	cz.set( xz,yz, 1, 0 );
	cw.set(  0, 0, 0, 1 );
}

template <class T>
axMatrix4<T> axMatrix4<T>::transpose () const  {
	return axMatrix4( cx.x, cy.x, cz.x, cw.x,
					 cx.y, cy.y, cz.y, cw.y,
					 cx.z, cy.z, cz.z, cw.z,
					 cx.w, cy.w, cz.w, cw.w );
}




template<class T> 
void axMatrix4<T>::setRotate ( T rad, const axVec3<T> &axis ) {
	axVec3<T>	a = axis.normalize();
	
	T c   = ax_cos( rad );
	T i_c = 1-c;
	T s   = ax_sin( rad );
	
	T &x= a.x;
	T &y= a.y;
	T &z= a.z;
	
	T xx = x*x, yy = y*y, zz = z*z;
	T xy = x*y, xz = x*z, yz = y*z;
	T xs = x*s, ys = y*s, zs = z*s;
	
	cx.set( (i_c * xx) + c,  (i_c * xy) + zs, (i_c * xz) - ys, 0 );
	cy.set( (i_c * xy) - zs, (i_c * yy) + c,  (i_c * yz) + xs, 0 );
	cz.set( (i_c * xz) + ys, (i_c * yz) - xs, (i_c * zz) + c,  0 );
	cw.set( 0,               0,               0,               1 );
}

template<class T>  axVec4<T> axMatrix4<T>::position() const    { return cw; }

template<class T>  axVec3<T> axMatrix4<T>::dirX() const       { return axVec3<T>( cx.x, cy.x, cz.x ).normalize(); }
template<class T>  axVec3<T> axMatrix4<T>::dirY() const       { return axVec3<T>( cx.y, cy.y, cz.y ).normalize(); }
template<class T>  axVec3<T> axMatrix4<T>::dirZ() const       { return axVec3<T>( -cx.z, -cy.z, -cz.z ).normalize(); }

template<class T>  axVec3<T> axMatrix4<T>::dirX_transpose() const   { return axVec3<T>( cx.x, cx.y, cx.z ).normalize(); }
template<class T>  axVec3<T> axMatrix4<T>::dirY_transpose() const   { return axVec3<T>( cy.x, cy.y, cy.z ).normalize(); }
template<class T>  axVec3<T> axMatrix4<T>::dirZ_transpose() const   { return axVec3<T>( -cz.x, -cz.y, -cz.z ).normalize(); }

template<class T> 
axMatrix4<T> axMatrix4<T>::operator* ( const axMatrix4<T> &v ) const{
	axMatrix4<T>	out;
	
	T e0,e1,e2,e3;
	e0=cx.x, e1=cy.x, e2=cz.x, e3=cw.x;
	out.cx.x = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	out.cy.x = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	out.cz.x = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	out.cw.x = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	e0=cx.y, e1=cy.y, e2=cz.y, e3=cw.y;
	out.cx.y = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	out.cy.y = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	out.cz.y = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	out.cw.y = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	e0=cx.z, e1=cy.z, e2=cz.z, e3=cw.z;
	out.cx.z = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	out.cy.z = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	out.cz.z = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	out.cw.z = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	e0=cx.w, e1=cy.w, e2=cz.w, e3=cw.w;
	out.cx.w = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	out.cy.w = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	out.cz.w = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	out.cw.w = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	return out;
}

template<class T>
void axMatrix4<T>::operator*=( const axMatrix4<T> &v ) {
	if( this == &v ) { 
		axMatrix4<T>	tmp = v;
		this->operator*=( tmp );
		return;
	}
	
	T e0,e1,e2,e3;
	
	e0=cx.x, e1=cy.x, e2=cz.x, e3=cw.x;
	cx.x = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	cy.x = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	cz.x = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	cw.x = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	e0=cx.y, e1=cy.y, e2=cz.y, e3=cw.y;
	cx.y = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	cy.y = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	cz.y = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	cw.y = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	e0=cx.z, e1=cy.z, e2=cz.z, e3=cw.z;
	cx.z = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	cy.z = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	cz.z = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	cw.z = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
	
	e0=cx.w, e1=cy.w, e2=cz.w, e3=cw.w;
	cx.w = e0*v.cx.x + e1*v.cx.y + e2*v.cx.z + e3*v.cx.w;
	cy.w = e0*v.cy.x + e1*v.cy.y + e2*v.cy.z + e3*v.cy.w;
	cz.w = e0*v.cz.x + e1*v.cz.y + e2*v.cz.z + e3*v.cz.w;
	cw.w = e0*v.cw.x + e1*v.cw.y + e2*v.cw.z + e3*v.cw.w;
}


//The explicit instantiation
template class axMatrix4<float>;
template class axMatrix4<double>;

