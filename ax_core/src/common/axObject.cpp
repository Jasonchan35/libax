//
//  axType.cpp
//  ax_core
//
//  Created by Jason on 2013-09-28.
//
//

#include "axObject.h"


class axObject::Type : public axType, public axSingleton< axObject::Type > {
	static	const char* staticName	() 			{ return "axObject"; }
	virtual	const char* name		() const	{ return staticName(); }
};

const	axType &	axObject::staticType	() {
	return Type::instance();
}

bool	axObject::isKindOf	( const axType & type ) const {
	if( &type == &( staticType() ) ) return true;
	return false;
}

axObject::axObject() {
}

axObject::~axObject() {
}


