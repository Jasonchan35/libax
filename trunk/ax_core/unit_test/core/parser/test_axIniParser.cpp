#include <ax/ax_core.h>

#include <ax/core/parser/axIniParser.h>

class TestApp : public axCommandLineApp {
public:
	axStatus test_Ini() {
		axStatus	st;
		axIniParser	ini;
		
		axFilePathA	path;
		path.set( __FILE__ );
		
		axTempStringA	filename;
		filename.format( "{?}/test.ini", path.dir() );
		
		st = ini.readFile( filename );		if( !st ) return st;

		ax_log( "{?}", ini );
		
		ax_log( "getValue = '{?}'", ini.getValue( "section3", "my value") );
		
		return 0;
	}
	
	virtual	axStatus	onRun() {
		axStatus	st;
		st = test_Ini();	
		ax_log( "== program end and return {?} ==", st );
		return st;
	}
};

axCommandLineApp_program_main( "axTest_App", TestApp );
