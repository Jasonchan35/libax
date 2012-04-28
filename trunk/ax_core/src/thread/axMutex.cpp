//
//  axMutex.cpp
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/thread/axMutex.h>

#ifdef axUSE_PTHREAD

axMutex::RecursiveAttr* axMutex::RecursiveAttr::getStatic() {
	static RecursiveAttr s;
	return &s;
}

#endif //axUSE_PTHREAD



