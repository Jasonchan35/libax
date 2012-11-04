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

template< size_t N >
class axRoundedRect2f {
public:

	axRoundedRect2f() { };
	
	axRoundedRect2f( float w, float h, float round ) {
		set( w, h, round );
	}
	
	
	void set( float w, float h, float round ) {
		static axUnitCircle2f< N > _unitCircle;
		vtx.resizeToCapacity();
					
		size_t s  = N/4;
		size_t s2 = s*2;
		size_t s3 = s*3;
		
		for( size_t i=0; i<s; i++ ) {
			vtx[i   ] = _unitCircle.vtx[i   ] * round + axVec2f( w, h )	- round;						//Right bottom
			vtx[i+s ] = _unitCircle.vtx[i+s ] * round + axVec2f( w, 0 ) + axVec2f( -round,  round );	//Right top
			vtx[i+s2] = _unitCircle.vtx[i+s2] * round					+ round;						//Left top
			vtx[i+s3] = _unitCircle.vtx[i+s3] * round + axVec2f( 0, h )	+ axVec2f(  round, -round );	//Left bottom
		}	
	}
	
	
	axArray< axVec2f, N > vtx;
};



#endif // __axRoundedRect_h__
