#include <ax/ax_core.h>
#include <ax/core/data_structure/axCopyOnWrite.h>

class Data : public axSharedPte{
public:
	int value;

	~Data() {
		axPRINT_FUNC_NAME
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("&{?} value={?} ", this, value );
	}
	
	Data* onClone() const {
		return new Data( *this );
	}
};

typedef	axCopyOnWrite<Data>	CowData;

class TestApp : public axCommandLineApp {
public:

	axStatus test_axCopyOnWrite() {
		CowData	a, b, c;
		a.ref( new Data );
		
		{	Data *p = a.getMutable();
			if( p ) p->value = 10;
		}
		
		b = a;
		c = b;
		
		ax_log("#1 a={?}, b={?}, c={?}", *a, *b, *c );
		
		{	Data *p = b.getMutable();
			if( p ) p->value = 10;
		}
		ax_log("#2 a={?}, b={?}, c={?}", *a, *b, *c );

		{	Data *p = a.getMutable();
			if( p ) p->value = 10;
		}
		ax_log("#3 a={?}, b={?}, c={?}", *a, *b, *c );
	
		return 0;
	}
		
	virtual	axStatus	onRun() {
		axStatus	st;
		st = test_axCopyOnWrite();

		
#if axOS_WIN
		ax_print("=== program ended with return {?} press any key to exit ====", st );
		_getch();
#endif
		return 0;
	}
};

axCommandLineApp_program_main( "axTest_App", TestApp );
