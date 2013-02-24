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

template<class T>
axVec2<T>	axVec2<T>::closetPointAtLine ( const axVec2<T> &line0, const axVec2<T> &line1, T & out_segment ) const {
	out_segment = closetPointAtLineSegment( line0, line1 );
	if( out_segment <= 0 ) return line0;
	if( out_segment >= 1 ) return line1;

	return ax_lerp( line0, line1, out_segment );
}

template<class T>
T	axVec2<T>::closetPointAtLineSegment	( const axVec2<T> &line0, const axVec2<T> &line1 ) const {
	axVec2<T> vAP = *this - line0;
	axVec2<T> vAB = line1 - line0;
	
	T mAB = vAB.magSq();
	if( mAB == 0 ) return 0;
		
	T dot_p = vAP.dot( vAB );
	T segment = dot_p / mAB;
	
	return ax_clamp01( segment );
}



//---- The explicit instantiation ----
template class axVec2<float>;
template class axVec2<double>;

