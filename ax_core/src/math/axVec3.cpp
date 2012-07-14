//
//  axVec3.cpp
//  ax
//
//  Created by Jason on 14/06/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/math/axMatrix4.h>


//---------- Matrix * Vec 3----------------

template<class T>
axVec3<T>	axVec3<T>::mul3x3( const axMatrix4<T> &m ) const {
	return axVec3<T>(	x*m.cx.x + y*m.cy.x + z*m.cz.x,
						x*m.cx.y + y*m.cy.y + z*m.cz.y,
						x*m.cx.z + y*m.cy.z + z*m.cz.z );
}

template<class T>
axVec3<T> axVec3<T>::mul4x3 ( const axMatrix4<T> &m ) const {
	return axVec3<T>(	x*m.cx.x + y*m.cy.x + z*m.cz.x + m.cw.x,
						x*m.cx.y + y*m.cy.y + z*m.cz.y + m.cw.y,
						x*m.cx.z + y*m.cy.z + z*m.cz.z + m.cw.z );
}

template<class T>
axVec3<T> axVec3<T>::mul4x4 ( const axMatrix4<T> &m ) const {
	return  ( axVec4<T>( *this, 1 ) * m ).to_Vec3();
}

template<class T> 
void axVec3<T>::operator*= ( const axMatrix4<T> &m ) {
	*this = *this * m;
}

template <class T>
T axVec3<T>::angle( const axVec3 &v ) {
	T m = mag() * v.mag();
	if( !m ) return 0.0;
	T cosine = dot(v)/m;
	if( cosine >= 1.0 )	return 0.0;
	if( cosine <= -1.0 )return (T)ax_math_PI;

	return acos( cosine );
}

template <class T>
bool axVec3<T>::isInsideTriangle ( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2 ) const {
	axVec3 c;
	axVec3 normal;
	normal.faceNormal( v0, v1, v2 );

	c = (v1-v0) ^ (*this-v0);
	if ( c.dot( normal ) < 0.0 ) return false;

	c = (v2-v1) ^ (*this-v1);
	if ( c.dot( normal ) < 0.0 ) return false;

	c = (v0-v2) ^ (*this-v2);
	if ( c.dot( normal ) < 0.0 ) return false;

	return true;
}

template <class T>
bool axVec3<T>::isInsideTriangle ( const axVec3 &v0, const axVec3 &v1, const axVec3 &v2, const axVec3 &normal ) const {
	axVec3 c;
	
	c = (v1-v0) ^ (*this-v0);
	if ( c.dot( normal ) < 0.0 ) return false;

	c = (v2-v1) ^ (*this-v1);
	if ( c.dot( normal ) < 0.0 ) return false;

	c = (v0-v2) ^ (*this-v2);
	if ( c.dot( normal ) < 0.0 ) return false;

	return true;
}

template <class T>
bool axVec3<T>::isParallel( const axVec3 &v, T tolerance ) const{
	return ax_math_equals( ax_abs( normalize().dot( v.normalize() ) ), (T)1, tolerance );
}


//---- The explicit instantiation ----
template class axVec3<float>;
template class axVec3<double>;
