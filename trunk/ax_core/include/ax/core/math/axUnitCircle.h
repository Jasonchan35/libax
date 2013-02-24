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

template< class T, size_t N >
class axUnitCircle2 {
public:
	axUnitCircle2() {
		T r = ax_deg_to_rad( (T)360.0 / N );
		for( size_t i=0; i<N; i++ ) {
			T ri = r * i;
			vtx[i].set( ax_sin( ri ), ax_cos( ri ) );
		}
	}
	axVec2<T> vtx[ N ];
};

#endif //__ax_axUnitCircle_h__
