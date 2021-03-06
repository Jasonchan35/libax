#include <ax/core/system/axApp.h>
#include <ax/core/file_system/axFileSystem.h>
#include <ax/core/file_system/axFilePath.h>
#include <ax/core/system/axAndroid.h>

axApp::axApp() {
	setInstance( this );
}

axStatus axApp::setAppName( const char* sz ) {
	return appName_.set( sz );
}

const axStringA& axApp::appName() {
	assert( appName_.size() != 0 );
	return appName_;
}

axStatus	axApp::loadFileFromAppData	( axIByteArray	&buf, const char*	 filename ) {
	axStatus	st;
	axStringA	path;
	st = getUserAppDataFilename( path, filename ); if( !st ) return st;
	st = axFileSystem::loadFile( buf, path );	if( !st ) return st;
	return 0;
}

axStatus	axApp::loadFileFromAppData	( axIByteArray	&buf, const wchar_t* filename ) {
	axStatus	st;
	axStringW	path;
	st = getUserAppDataFilename( path, filename ); if( !st ) return st;
	st = axFileSystem::loadFile( buf, path );	if( !st ) return st;
	return 0;
}

axStatus	axApp::saveFileToAppData	( const axIByteArray	&buf, const char*	 filename ) {
	axStatus	st;
	axStringA	path;
	st = getUserAppDataFilename( path, filename );		if( !st ) return st;
	st = axFileSystem::saveFile( buf, path, true );	if( !st ) return st;
	return 0;
}

axStatus	axApp::saveFileToAppData	( const axIByteArray	&buf, const wchar_t* filename ) {
	axStatus	st;
	axStringW	path;
	
	st = getUserAppDataFilename( path, filename );		if( !st ) return st;
	st = axFileSystem::saveFile( buf, path, true );	if( !st ) return st;
	return 0;
}

axStatus	axApp::getUserAppDataFilename	( axIStringA	&out, const char*	 filename ) {
	axStatus	st;
	axStringA	path;
	st = getUserAppDataDir( path );		if( !st ) return st;
	st = path.append( '/' );		if( !st ) return st;
	st = path.append( filename );	if( !st ) return st;
	st = out.set( path );			if( !st ) return st;
	
	return 0;
}

axStatus	axApp::getUserAppDataFilename	( axIStringW	&out, const wchar_t* filename ) {
	axStatus	st;
	axStringW	path;
	st = getUserAppDataDir( path );		if( !st ) return st;
	st = path.append( '/' );		if( !st ) return st;
	st = path.append( filename );	if( !st ) return st;
	st = out.set( path );			if( !st ) return st;
	return 0;
}	

#if 0
#pragma mark ================= Windows ====================
#endif
#if axOS_WIN

#include <Shlobj.h>

template<class T>
static	axStatus	axApp_SHGetFolderPath( axIString_<T>& str, int CSIDL ) {
	wchar_t	p[MAX_PATH];
	HRESULT ret = SHGetFolderPath( NULL, CSIDL | CSIDL_FLAG_DONT_VERIFY, NULL, SHGFP_TYPE_CURRENT, p );
	if( ret != S_OK ) { str.clear(); return -1; }
	return str.set(p);
}

axStatus	axApp::getUserAppDataDir	( axIStringA &out ) { 
	axStatus st;
	st = axApp_SHGetFolderPath( out, CSIDL_APPDATA );	if( !st ) return st;
	st = out.appendFormat("/{?}", appName_ );			if( !st ) return st;
	return 0;
}

axStatus	axApp::getUserAppDataDir	( axIStringW &out ) { 
	axStatus st;
	st = axApp_SHGetFolderPath( out, CSIDL_APPDATA );	if( !st ) return st;
	st = out.appendFormat("/{?}", appName_ );			if( !st ) return st;
	return 0;
}

axStatus	axApp::getUserHomeDir		( axIStringA &out ) { return axApp_SHGetFolderPath( out, CSIDL_PROFILE ); }
axStatus	axApp::getUserHomeDir		( axIStringW &out ) { return axApp_SHGetFolderPath( out, CSIDL_PROFILE ); }

axStatus	axApp::getUserDocumentDir	( axIStringA &out ) { return axApp_SHGetFolderPath( out, CSIDL_MYDOCUMENTS ); }
axStatus	axApp::getUserDocumentDir	( axIStringW &out ) { return axApp_SHGetFolderPath( out, CSIDL_MYDOCUMENTS ); }

axStatus	axApp::getDesktopDir		( axIStringA &out ) { return axApp_SHGetFolderPath( out, CSIDL_DESKTOP ); }
axStatus	axApp::getDesktopDir		( axIStringW &out ) { return axApp_SHGetFolderPath( out, CSIDL_DESKTOP ); }

axStatus	axApp::getTempDir			( axIStringA &out )	{
	wchar_t	p[MAX_PATH+1];
	p[MAX_PATH] = 0;
	DWORD n = GetTempPath( MAX_PATH, p );
	if( n == 0 ) return -1;
	return out.setWithLength( p, n );
}

axStatus	axApp::getTempDir			( axIStringW &out )	{
	wchar_t	p[MAX_PATH+1];
	p[MAX_PATH] = 0;
	DWORD n = GetTempPath( MAX_PATH, p );
	if( n == 0 ) return -1;
	return out.setWithLength( p, n );
}

axStatus	axApp::getAppResourceDir	( axIStringA &out ) {
	axStatus	st;
	axTempStringA	tmp;
	st = axFileSystem::getProcessFileDir( tmp );		if( !st ) return st;
	st = out.format( "{?}Resources", tmp ); if( !st ) return st;
	return 0;
}

axStatus	axApp::getAppResourceDir	( axIStringW &out ) {
	axStatus	st;
	axTempStringA	tmp;
	st = getAppResourceDir( tmp );		if( !st ) return st;
	st = out.format( "{?}Resources", tmp ); if( !st ) return st;
	return 0;
}

bool axApp::isAltKeyDown() {
	return GetAsyncKeyState( VK_MENU ) != 0;
}

bool axApp::isCtrlKeyDown() {
	return ( GetAsyncKeyState( VK_LCONTROL ) || GetAsyncKeyState( VK_RCONTROL ) );
}

bool axApp::isShiftKeyDown() {
	return ( GetAsyncKeyState( VK_LSHIFT ) || GetAsyncKeyState( VK_RSHIFT ) );
}

#endif  //axOS_Win


#if 0
#pragma mark ================= Unix Common ====================
#endif
#if axOS_UNIX


#if 0
#pragma mark ================= iOS & MacOSX ====================
#endif

#if axOS_MacOSX 

bool axApp::isAltKeyDown() {
	return (kCGEventFlagMaskAlternate == (CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState) & NSDeviceIndependentModifierFlagsMask));
}

bool axApp::isCtrlKeyDown() {
	return ( kCGEventFlagMaskControl == (CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState) & NSDeviceIndependentModifierFlagsMask));
}

bool axApp::isShiftKeyDown() {
	return ( kCGEventFlagMaskShift   == (CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState) & NSDeviceIndependentModifierFlagsMask));
}


#endif // axOS_MacOS

#if axOS_iOS || axOS_MacOSX

template<class T>
axStatus axApp_NSSearchPath( axIString_<T> &out, NSSearchPathDirectory d ) {
	axStatus st;
	NSArray *paths = NSSearchPathForDirectoriesInDomains( d, NSUserDomainMask, YES );
	if( paths == nil ) { out.clear(); return -1; }
	NSString *docu = [paths objectAtIndex:0];
	if( docu == nil ) { out.clear(); return -2; }
	st = out.set( [docu UTF8String] );	if( !st ) return st;	
	axFileSystem::makeDir( out );
	
	return 0;
}

axStatus	axApp::getUserAppDataDir	( axIStringA &out )	{
	if( appName().size() == 0 ) { out.clear(); return -1; }
	axStatus st;
	st = axApp_NSSearchPath( out, NSApplicationSupportDirectory ); if( !st ) return st;
	st = out.append( "/" );			if( !st ) return st;
	st = out.append( appName() );	if( !st ) return st;
	return 0;
}

axStatus	axApp::getUserAppDataDir	( axIStringW &out )	{
	if( appName().size() == 0 ) { out.clear(); return -1; }
	axStatus st;
	st = axApp_NSSearchPath( out, NSApplicationSupportDirectory ); if( !st ) return st;
	st = out.append( "/" );			if( !st ) return st;
	st = out.append( appName() );	if( !st ) return st;
	return 0;
}



axStatus	axApp::getUserDocumentDir	( axIStringA &out )	{ return axApp_NSSearchPath( out, NSDocumentDirectory ); }	
axStatus	axApp::getUserDocumentDir	( axIStringW &out )	{ return axApp_NSSearchPath( out, NSDocumentDirectory ); }

axStatus	axApp::getDesktopDir		( axIStringA &out )	{ return axApp_NSSearchPath( out, NSDesktopDirectory ); }	
axStatus	axApp::getDesktopDir		( axIStringW &out )	{ return axApp_NSSearchPath( out, NSDesktopDirectory ); }

#include <pwd.h>
axStatus	axApp::getUserHomeDir		( axIStringA &out )	{ 
	struct passwd	*p = ::getpwuid( ::getuid() );
	if( !p ) return -1;
	return out.set( p->pw_dir );	
}
	
axStatus	axApp::getUserHomeDir		( axIStringW &out )	{ 
	struct passwd	*p = ::getpwuid( ::getuid() );
	if( !p ) return -1;
	return out.set( p->pw_dir );	
}

axStatus	axApp::getAppResourceDir	( axIStringA &out ) {
	axStatus	st;
	axTempStringA	tmp;
	st = axFileSystem::getProcessFileDir( tmp );				if( !st ) return st;
	st = out.format( "{?}../Resources/Resources", tmp );		if( !st ) return st;
	return 0;
}

axStatus	axApp::getAppResourceDir	( axIStringW &out ) {
	axStatus	st;
	axTempStringA	tmp;
	st = getAppResourceDir( tmp );		if( !st ) return st;
	st = out.format( "{?}../Resources/Resources", tmp );	if( !st ) return st;
	return 0;
}


#if 0
#pragam mark ================= Android ====================
#endif

#elif axOS_Android

axStatus	axApp::getUserAppDataDir( axIStringA &str ) {
	return str.set( axAndroid::instance.resourcePath() );
}

axStatus	axApp::getUserAppDataDir( axIStringW &str ) {
	return str.set( axAndroid::instance.resourcePath() );
}

axStatus	axApp::getProcessFilename( axIStringW &out ) {
	//! todo !!
	return -1;
}

axStatus	axApp::getProcessFilename( axIStringA &out ) {
	//! todo !!
	return -1;
}
 
	

#else // Other UNIX
#if 0
#pragma mark ================= Other UNIX ====================
#endif

	#if 0
	#pragam mark ================= Linux ====================
	#endif
	#if axOS_Linux

	axStatus	axApp::getProcessFilename( axIStringW &out ) {
		axTempStringA	tmp;
		axStatus st = getProcessFilename( tmp );		if( !st ) return st;
		return out.set( tmp );
	}

	axStatus	axApp::getProcessFilename( axIStringA &out ) {
		char buf[ axkFilePathMax + 1];
		ssize_t ret = readlink("/proc/self/exe", buf, axkFilePathMax );
		if ( ret == -1 ) { out.clear(); return -1; }

		// if the /proc/self/exe symlink has been dropped by the kernel for
		// some reason, then readlink() could also return success but
		// "(deleted)" as link destination...
		if ( ax_strcmp(buf, "(deleted)") == 0 ) {
			out.clear(); return -2;
		}

		buf[ret] = 0;
		return out.set( buf );
	}

	#endif //axOS_Linux
	
	axStatus	axApp::getTempDir ( axIStringA &out ) { return out.set("/tmp"); }
	axStatus	axApp::getTempDir ( axIStringW &out ) { return out.set("/tmp"); }

#include <pwd.h>

axStatus	axApp::getUserAppDataDir		( axIStringA	&out ) {
	if( appName().size() == 0 ) { out.clear(); return -1; }
	axStatus st;
	st = getUserHomeDir( out );		if( !st ) return st;
	st = out.append( "/." );		if( !st ) return st;
	st = out.append( appName() );	if( !st ) return st;
	return 0;
}

axStatus	axApp::getUserAppDataDir		( axIStringW	&out ) {
	if( appName().size() == 0 ) { out.clear(); return -1; }
	
	axStatus st;
	st = getUserHomeDir( out );		if( !st ) return st;
	st = out.append( "/." );		if( !st ) return st;
	st = out.append( appName() );	if( !st ) return st;
	return 0;
}
	
axStatus	axApp::getUserDocumentDir	( axIStringA	&out ) {
	return getUserHomeDir( out );
}
axStatus	axApp::getUserDocumentDir	( axIStringW	&out ) {
	return getUserHomeDir( out );
}

axStatus	axApp::getUserName			( axIStringA	&out ) {
	struct passwd	*p = ::getpwuid( ::getuid() );
	if( !p ) return -1;
	return out.set( p->pw_name );
}

axStatus	axApp::getUserName		( axIStringW	&out ) {
	struct passwd	*p = ::getpwuid( ::getuid() );
	if( !p ) return -1;
	return out.set( p->pw_name );
}

axStatus	axApp::getUserHomeDir		( axIStringA	&out ) {
	struct passwd	*p = ::getpwuid( ::getuid() );
	if( !p ) return -1;
	return out.set( p->pw_dir );
}

axStatus	axApp::getUserHomeDir		( axIStringW	&out ) {
	struct passwd	*p = ::getpwuid( ::getuid() );
	if( !p ) return -1;
	return out.set( p->pw_dir );
}

axStatus	axApp::getDesktopDir		( axIStringA	&out ) {
	axStatus	st;
	st = getUserHomeDir( out );		if( !st ) return st;
	st = out.append( "/Desktop" );	if( !st ) { out.clear(); return st; }
	return 0;
}

axStatus	axApp::getDesktopDir		( axIStringW	&out ) {
	axStatus	st;
	st = getUserHomeDir( out );		if( !st ) return st;
	st = out.append( "/Desktop" );	if( !st ) { out.clear(); return st; }
	return 0;
}

#endif //Unix Other

#endif //axOS_UNIX






 
