//
//  axRoundedRect.h
//  ax_core
//
//  Created by tony on 5/11/12.
//
//

#ifndef __axRoundedRect_h__
#define __axRoundedRect_h__

#include "axUnitCircle.h"

template< class T, size_t N >
class axRoundedRect2 {
public:

	axRoundedRect2() { };
	
	axRoundedRect2( T w, T h, T round ) {
		set( w, h, round );
	}
	
	
	void set( T w, T h, T round ) {
		static axUnitCircle2< T, N > _unitCircle;
		vtx.resizeToCapacity();
					
		size_t s  = N/4;
		size_t s2 = s*2;
		size_t s3 = s*3;
		
		for( size_t i=0; i<s; i++ ) {
			vtx[i   ] = _unitCircle.vtx[i   ] * round + axVec2<T>( w, h ) - round;							//Right bottom
			vtx[i+s ] = _unitCircle.vtx[i+s ] * round + axVec2<T>( w, 0 ) + axVec2<T>( -round,  round );	//Right top
			vtx[i+s2] = _unitCircle.vtx[i+s2] * round					  + round;							//Left top
			vtx[i+s3] = _unitCircle.vtx[i+s3] * round + axVec2<T>( 0, h ) + axVec2<T>(  round, -round );	//Left bottom
		}	
	}
	
	axArray< axVec2<T>, N > vtx;
};


#endif // __axRoundedRect_h__
