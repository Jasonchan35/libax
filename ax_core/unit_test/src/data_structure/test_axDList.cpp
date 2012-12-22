//
//  test_axDList.cpp
//  ax_core_unit_test
//
//  Created by Jason on 2012-12-23.
//  Copyright (c) 2012 Jason. All rights reserved.
//

#include <ax/ax_core.h>

class Obj : public axDListNode<Obj,true> {
public:
	Obj( int value ) {
		v = value;
	}
	
	
	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?}", v );
	}
	int v;
};



axStatus test_axDList() {
	axDList<Obj>	list;
	
	for( int i=0; i<5; i++ ) {
		list.append( new Obj( i ) );
	}

	list.insert( new Obj(99), list.getNodeByIndex(2) );

	ax_log_var( list );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = test_axDList();
	
	
    ax_print("==== return {?} ====\n", st );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}