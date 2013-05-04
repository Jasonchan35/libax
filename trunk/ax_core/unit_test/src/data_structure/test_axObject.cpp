//
//  test_axObject.cpp
//  ax_core_unit_test
//
//  Created by Jason on 2013-05-04.
//  Copyright (c) 2013 Jason. All rights reserved.
//

#include <ax/ax_core.h>
#include <ax/core/data_structure/axObjectRef.h>

//==============================

class MyClass_Data : public axObject_Data {
public:
	MyClass_Data() {
		axPRINT_FUNC_NAME;
	}
	
	~MyClass_Data() {
		axPRINT_FUNC_NAME;
	}

	int value;
};
typedef	axObjectRef< MyClass_Data >	MyClass;

class MySubClass_Data : public MyClass_Data {
public:
	MySubClass_Data() {
		axPRINT_FUNC_NAME;
	}
	
	~MySubClass_Data() {
		axPRINT_FUNC_NAME;
	}
};
typedef	axObjectRef< MySubClass_Data >	MySubClass;

class OtherClass_Data : public axObject_Data {
};
typedef	axObjectRef< OtherClass_Data >	OtherClass;


axStatus	test_axObject() {
	axStatus st;
	MySubClass	c(st);		if( !st ) return st;
//	MySubClass	b(c);
	
	c->value = 100;

	{
	MyClass	a(c);
		//a = c;
	//	OtherClass	o(st);		if( !st ) return st;
	//	a = o;
		ax_log_var( a->value );
	}
	
	c.unref();

	ax_log("func exit");

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;
	//========================================

	axUTestCase( test_axObject() );

	//========================================

    ax_print("==== end of program ====" );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}