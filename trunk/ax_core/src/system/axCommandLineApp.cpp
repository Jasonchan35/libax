//
//  axCommandLineApp.cpp
//  ax
//
//  Created by Jason on 24/06/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/system/axCommandLineApp.h>

axCommandLineApp::axCommandLineApp() {
	argc_ = 0;
	argv_ = NULL;
	axApp::_setInstance( this );
}

axCommandLineApp::~axCommandLineApp() {
}

axStatus axCommandLineApp::_run( const char* name, int argc, const char* argv[] ) {
	axStatus st;
	st = setAppName( name );	if( !st ) return st;
	argc_ = argc;
	argv_ = argv;
	return _os_run();
}


int	axCommandLineApp::getArgCount() {
	return argc_;
}

const char* axCommandLineApp::getArg( int index ) {
	if( index < 0 || index > argc_ ) return NULL;
	return argv_[index];
}

const char* axCommandLineApp::getOption( const char * opt ) {
	for( int i=1; i<argc_; i++ ) {
		const char *c = argv_[i];
		if( c[0] != '-' ) {
			if( opt == NULL ) {
				return argv_[i];
			}
			continue;
		}
		
		i++; //skip next arg for this opt
		if( ! opt ) continue;
		if( i >= argc_ ) return NULL; // nothing after opt ?
		if( strcmp( c+1, opt ) == 0 ) {
			return argv_[i];
		}
	}
	return NULL;
}

void axCommandLineApp::printArgs() {
	ax_log("argv =");
	for( int i=0; i<argc_; i++ ) {
		ax_log( " [{?}]", argv_[i] );
	}
	ax_log("\n");
}

#if 0
#pragma mark ================= iOS ====================
#endif
#ifdef axOS_iOS


/* terminateWithSuccess rejected by apple 2.5: Apps that use non-public APIs will be rejected
@interface UIApplication( MyUIApplication ) 
// for remove compiler warning: 'UIApplication' may not respond to '-terminateWithSuccess'
- (void)terminateWithSuccess; 
@end
 */

@interface axCommandLineApp_Delegate : NSObject < UIApplicationDelegate, UIAccelerometerDelegate > {
}
@end

@implementation axCommandLineApp_Delegate
-(void)applicationDidFinishLaunching:(UIApplication *)application {
	axCommandLineApp* app = axCommandLineApp::getInstance();
	app->thread_.create();
}

- (void)applicationWillResignActive:(UIApplication *)application {

}

@end

void axCommandLineApp::Thread::onThreadProc() {
	axCommandLineApp* app = axCommandLineApp::getInstance();
	axStatus st = app->onRun();	
	
	ax_log( "\n\n===== Command Line Program Ended with exit code {?} ======", st );
}

axStatus axCommandLineApp::_os_run() {
	axScope_NSAutoreleasePool	pool;
	
	[UIApplication sharedApplication];
	setCurrentDir( [[[NSBundle mainBundle ] resourcePath] UTF8String] );	
	UIApplicationMain( 0, NULL, @"UIApplication",@"axCommandLineApp_Delegate" );
	
	axStatus st;
	return st;
}

#else //===
#if 0
#pragma mark ================= Other ====================
#endif

axStatus axCommandLineApp::_os_run() {

	return onRun();
}

#endif

