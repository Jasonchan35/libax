//
//  ax_linear_algebra.h
//  ax_core
//
//  Created by tony on 5/11/12.
//
//

#ifndef __ax_linear_algebra_h__
#define __ax_linear_algebra_h__

#include "axVec2.h"

template< class T >
axVec2<T> ax_line_closest_point( T &out_w, const axVec2<T> &line0, const axVec2<T> &line1, const axVec2<T> &p ) {

	axVec2<T> vAP = p - line0;
	axVec2<T> vAB = line1 - line0;
	
	T mAB = vAB.magSq();
	if( mAB == 0 ) return line0;
	
	T dot_p = vAP.dot( vAB );
	out_w = dot_p / mAB;
	
	if( out_w <= 0 ) return line0;
	if( out_w >= 1 ) return line1;
	
	return line0 + vAB * out_w;
}

#endif
