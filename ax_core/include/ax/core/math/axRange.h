//
//  axRange.h
//  ax_core
//
//  Created by Jason on 2012-11-08.
//
//

#ifndef ax_core_axRange_h
#define ax_core_axRange_h

#include "../string/ax_string.h"

template<class T>
class axRange {
public:
	axRange() : start(0), count(0) {}
	axRange( T _start, T _count ) : start(_start), count(_count) {}

	T	last	() const	{ return start + count -1; }
	T	end		() const	{ return start + count; }

	T	start;
	T	count;
	
	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("start={?} count={?}", start, count );
	}
};

#endif
