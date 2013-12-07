//
//  axObject.h
//  ax_core
//
//  Created by Jason on 2013-11-15.
//
//

#ifndef ax_core_axObject_h
#define ax_core_axObject_h

#include "../common/axTyped.h"
#include "../data_structure/axRef.h"

class axObject : public axReferred, public axTyped {
	axTypeDef( axObject, axTyped );
public:
	axObject() {}
};

#endif
