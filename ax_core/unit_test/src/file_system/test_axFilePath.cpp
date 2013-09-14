#include <ax/ax_core.h>

class TestApp : public axCommandLineApp {
public:

#define	axTEST( ex ) \
	{ \
		axStatus st = ex; \
		if( !st ) { ax_log("FAIL"); } \
	} \
//---------


	axStatus	test_all() {
		axStringA	str;

		const char* sample = "A/../../B\\/./C/D";
		axTEST( axFilePath::getNormalizePath( str, sample ) );
		ax_log( "getNormalize [{?}] => [{?}]", sample, str );

		axTEST( axFilePath::getParentDir( str, sample ) );
		ax_log( "getParentDir [{?}] => [{?}]", sample, str );

		return 0;
	}
	
	virtual	axStatus	onRun() {
		axStatus	st;
		st = test_all();
		
#if axOS_WIN
		ax_print("=== program ended with return {?} press any key to exit ====", st );
		_getch();
#endif
		return 0;
	}
};

axCommandLineApp_program_main( "axTest_App", TestApp );
