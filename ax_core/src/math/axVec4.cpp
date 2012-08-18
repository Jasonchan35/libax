//
//  axVec4.cpp
//  ax
//
//  Created by Jason on 14/06/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/math/axMatrix4.h>

//---------- Matrix * Vec 4 ----------------

template<class T>
axVec4<T> axVec4<T>::operator* ( const axMatrix4<T> &m ) const {
	return axVec4<T>(	x*m.cx.x + y*m.cy.x + z*m.cz.x + w*m.cw.x,
						x*m.cx.y + y*m.cy.y + z*m.cz.y + w*m.cw.y,
						x*m.cx.z + y*m.cy.z + z*m.cz.z + w*m.cw.z,
						x*m.cx.w + y*m.cy.w + z*m.cz.w + w*m.cw.w );
}

template<class T>
void axVec4<T>::operator*= ( const axMatrix4<T> &m ) {
	*this = *this * m;
}

//---- The explicit instantiation ----
template class axVec4<float>;
template class axVec4<double>;

