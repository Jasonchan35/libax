//
//  axUUID.cpp
//  ax_core
//
//  Created by Jason on 2013-11-14.
//
//

#include "axUUID.h"

axUUID::axUUID () {
	memset( bytes_, 0, kByteSize );
}

axStatus axUUID::toStringFormat( axStringFormat & f ) const {
	axStatus st;
	axStringA_<128>	tmp;
	for( size_t i=0; i<kByteSize; i++ ) {
		if( i==4 || i==6 || i==8 || i==10 ) {
			st = tmp.append("-");		if( !st ) return st;
		}
		st = tmp.appendFormat( "{?:02X}", bytes_[i] );		if( !st ) return st;
	}
	
	return f.put( tmp );
}

#if axOS_Win


#else //=====================================

#include <uuid/uuid.h>

void axUUID::generate	() {
	uuid_generate( bytes_ );
}

#endif
