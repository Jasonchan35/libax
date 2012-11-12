//
//  axHumanString.h
//  ax_core
//
//  Created by Jason on 2012-11-13.
//
//

#ifndef ax_core_axHumanString_h
#define ax_core_axHumanString_h

#include "axString.h"

class axHumanString_Byte {
public:
	axHumanString_Byte( uint64_t s ) : s_(s) {}

	const char*	c_str() const {
		     if( s_ >= (1ULL<<40) ) str_.format("{?}T", s_ / (1ULL<<40) );
		else if( s_ >= (1ULL<<30) ) str_.format("{?}G", s_ / (1ULL<<30) );
		else if( s_ >= (1ULL<<20) ) str_.format("{?}M", s_ / (1ULL<<20) );
		else if( s_ >= (1ULL<<10) ) str_.format("{?}k", s_ / (1ULL<<10) );
		                       else str_.format("{?}B", s_ );
		return str_;
	}
private:
	uint64_t	s_;
	mutable axStringA_<64>	str_;
};


#endif
