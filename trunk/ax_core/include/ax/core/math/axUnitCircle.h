//
//  axUnitCircle.h
//  ax_core
//
//  Created by tony on 5/11/12.
//
//

#ifndef __axUnitCircle_h__
#define __axUnitCircle_h__

#include "axVec2.h"

template< size_t N >
class axUnitCircle2f {
public:
	axUnitCircle2f() {
		float r = ax_deg_to_rad( 360.0f / N );
		for( size_t i=0; i<N; i++ ) {
			float ri = r * i;
			vtx[i].set( ax_sin( ri ), ax_cos( ri ) );
		}
	}
	axVec2f vtx[ N ];
};


#endif //__ax_axUnitCircle_h__
