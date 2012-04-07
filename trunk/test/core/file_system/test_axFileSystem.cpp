#include <ax/ax_core.h>

class TestApp : public axCommandLineApp {
public:
	axStatus test_File() {
		axStatus	st;
		axByteArray	buf;
		
		for( axSize i=0; i<400; i++ ) {
			buf.append(i);
		}
		
		axFile	file;
		st = file.openRead( "test_File_tmp.txt" );		if( !st ) return st;
		//st = file.write( buf );		if( !st ) return st;
		st = file.read( buf );		if( !st ) return st;
		
		return 0;
	}
	
	axStatus test_CSV() {
		axCSVFileParser	p;
		axStatus st;
		st = p.open(L"/Users/jason/svn/libax/1 copy.csv");		if( !st ) return st;
		
		axStringA_Array	row;
		while( p.getRow( row ) ) {
			ax_log( "{?}", row );
		}
		return 0;		
	}
	
	axStatus test_FileStream() {
		axStatus st;
		axFileStream	file;
		st = file.open( "test_file.txt", "wb" );		        if( !st ) return st;
		st = file.format( "writeString test {?}\n", 1234 );		if( !st ) return st;		
		return 0;
	}
	
#define	axTEST( ex ) \
{ \
axStatus st = ex; \
if( !st ) { assert(false); } \
}
//---------
	axStatus	test_Path() {
		axStringA	str;
		axTEST( axApp::getCurrentDir( str ) );			ax_log( "getCurrentDir      {?}", str );
		axTEST( axApp::getProcessFilename( str ) );		ax_log( "getProcessFilename {?}", str );
		axTEST( axApp::getUserAppDataDir( str ) );		ax_log( "getUserAppDataDir  {?}", str );
		axTEST( axApp::getUserHomeDir( str ) );			ax_log( "getUserHomeDir     {?}", str );
		axTEST( axApp::getDesktopDir( str ) );			ax_log( "getDesktopDir      {?}", str );
		axTEST( axApp::getUserDocumentDir( str ) );		ax_log( "getUserDocumentDir {?}", str );
		return 0;
	}
	
	class Data { 
	public:
		int8_t	b;
		float	f;
		int32_t i;
		template<class S> axStatus serialize_io( S &s ) {
			axStatus st;
			st = s.io( b );	if( !st ) return st;
			st = s.io( i );	if( !st ) return st;
			st = s.io( f );	if( !st ) return st;
			return 0;
		}
	};

	axStatus test_Dir() {
		axStatus st;
		axDir	d;
		st = d.open( "c:" );	if( !st ) return st;

		axDir::Entry	e;
		while( d.next( e ) ) {
			ax_print("filename = {?}", e.filename );
			if( e.isDir() ) ax_print(" <DIR>");
			ax_print("\n");
		}
		ax_print("---\n");
		return 0;
	}

	axStatus	test_Dir_all() {
		axStatus st;
		axArray< axDir::Entry >	a;
		st = axDir::getEntries( "c:", a );		if( !st ) return st;

		for( axSize i=0; i<a.size(); i++ ) {
			axDir::Entry &e = a[i];
			ax_print("filename = {?}", e.filename );
			if( e.isDir() ) ax_print(" <DIR>");
			ax_print("\n");
		}
		return 0;
	}
	
	virtual	axStatus	onRun() {
		axStatus	st;
//		st = test_CSV();	if( !st ) return st;
//		st = test1();
		st = test_Dir_all();
		
#if axOS_WIN
		ax_print("=== program ended with return {?} press any key to exit ====", st );
		_getch();
#endif
		return 0;
	}
};

axCommandLineApp_program_main( "axTest_App", TestApp );
