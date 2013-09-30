#include <ax/ax_core.h>

class ClsB {
public:
	ClsB() : value(0) {}

	axRefNode( ClsB, InA );

	void print() {
		printf("ClsB %d\n", value );
	}

	int value;
};


class ClsA {
public:
	axRefList( ClsB, InA, list );
};

class TestApp : public axCommandLineApp {
public:

#define	axTEST( ex ) \
	{ \
		axStatus st = ex; \
		if( !st ) { ax_log("FAIL"); } \
	} \
//---------


	axStatus	test() {
		ClsA a;
		ClsB b;

		b.value = 10;

		a.list.append( &b );

		ClsB::InA* p = a.list.head();
		for( ; p; p=p->next() ) {
			p->obj().print();
		}

		return 0;
	}
	
	virtual	axStatus	onRun() {
		axStatus	st;
		st = test();
		
#if axOS_WIN
		ax_print("=== program ended with return {?} press any key to exit ====", st );
		_getch();
#endif
		return 0;
	}
};

axCommandLineApp_program_main( "axTest_App", TestApp );
