//
//  axCopyOnWrite.h
//  libax
//
//  Created by Jason Chan on 2011-11-15.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef libax_axCopyOnWrite_h
#define libax_axCopyOnWrite_h

#include "../thread/axAtomicInt.h"

template<class T>
class axCopyOnWrite {
public:

	const T* operator->() const		{ return  ptr(); }
	const T& operator* () const		{ return *ptr(); }
	operator const	T* () const		{ return  ptr(); }

	const T* 	ptr() const			{ return  p_.ptr(); }
	void		ref( const T* p )	{ p_.ref( const_cast<T*>(p) ); }
		
	T* getMutable() 				{ if( ! p_.unshare() ) return NULL; return p_.ptr(); }

private:
	axSharedPtr<T>	p_;
};

#endif
