//
//  axCubicBezierCurve.h
//  ax_core
//
//  Created by tony on 31/10/12.
//
//

#ifndef __axCubicBezierCurve_h__
#define __axCubicBezierCurve_h__

#include "axVec2.h"


template< size_t DIV >
class axCubicBezierCurve2f {
public:

	axCubicBezierCurve2f() { };
	
	axCubicBezierCurve2f( const axVec2f &p0, const axVec2f &p1,
							const axVec2f &p2,  const axVec2f &p3 ) {

		set( p0, p1, p2, p3 );
	}
	
	void set( const axVec2f &p0, const axVec2f &p1,
							const axVec2f &p2,  const axVec2f &p3 ) {
		vtx.resizeToCapacity();
		for( size_t i=0; i<=DIV; i++ ) {
			vtx[ i ] = ax_bezier( p0, p1, p2, p3, (float)i / (float) DIV );
		}

	}
	
	
	axStatus onTake( axCubicBezierCurve2f &src ) {
		return ax_take( vtx, src.vtx );
	}
	
	axArray< axVec2f, DIV+1 > vtx;
};



#endif //__axCubicBezierCurve_h__
