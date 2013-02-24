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
#include "axRect2.h"
#include "ax_linear_algebra.h"

template< class T, size_t DIV >
class axCubicBezierCurve2 {
public:

	axCubicBezierCurve2() { };
	axCubicBezierCurve2( const axVec2<T> &p0,
						 const axVec2<T> &p1,
						 const axVec2<T> &p2,
						 const axVec2<T> &p3 );
	
	void set ( 	const axVec2<T> &p0,
			 	const axVec2<T> &p1,
				const axVec2<T> &p2,
				const axVec2<T> &p3 );
				
	bool hitTest	( const axVec2<T> &pt, T inDistance, T &outDistance, T &out_w );

	axStatus onTake	( axCubicBezierCurve2 &src ) {
		return ax_take( vtx, src.vtx );
	}
	
	axRect2<T> getRect() const;

	
	axArray< axVec2<T>, DIV+1 > vtx;
};

template< class T, size_t DIV > inline
axCubicBezierCurve2<T,DIV>::axCubicBezierCurve2( const axVec2<T> &p0,
					 const axVec2<T> &p1,
					 const axVec2<T> &p2,
					 const axVec2<T> &p3 )
{
	set( p0, p1, p2, p3 );
}

template< class T, size_t DIV > inline
void axCubicBezierCurve2<T,DIV>::set ( 	const axVec2<T> &p0,
			const axVec2<T> &p1,
			const axVec2<T> &p2,
			const axVec2<T> &p3 )
{
	vtx.resizeToCapacity();
	for( size_t i=0; i<=DIV; i++ ) {
		vtx[ i ] = ax_bezier( p0, p1, p2, p3, (T)i / (T) DIV );
	}
}

template< class T, size_t DIV > inline
bool axCubicBezierCurve2<T,DIV>::hitTest( const axVec2<T> &pt, T inDistance, T &outDistance, T &out_w ) {
	bool b = false;
	T w;
	
	T min_dis = inDistance;
	
	for( size_t i=0; i<DIV; i++ ) {
		axVec2<T> p = pt.closetPointAtLine( vtx[i], vtx[i+1], w );
		T dis = ( pt - p ).mag();
		
		if( dis < min_dis ) {
			out_w = (i + w) / DIV;
			min_dis = dis;
			b = true;
		}
	}
	
	outDistance = min_dis;
	return b;
}


template< class T, size_t DIV > inline
axRect2<T> axCubicBezierCurve2<T,DIV>::getRect() const {
	axRect2<T> r;
	r.pos() = vtx[0];
	r.setBottomRight( vtx[DIV] );
	return r;
}


#endif //__axCubicBezierCurve_h__
