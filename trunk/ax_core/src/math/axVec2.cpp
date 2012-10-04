//
//  axVec2.cpp
//  ax
//
//  Created by Jason on 14/06/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/math/axVec2.h>
#include <ax/core/math/axMatrix4.h>
 
//---------- Matrix * Vec 2----------------

template<class T>
axVec2<T>	axVec2<T>::mul3x2( const axMatrix4<T> &m ) const {
	return axVec2<T>(	x*m.cx.x + y*m.cy.x + m.cw.x,
						x*m.cx.y + y*m.cy.y + m.cw.y );
}




//---- The explicit instantiation ----
template class axVec2<float>;
template class axVec2<double>;

