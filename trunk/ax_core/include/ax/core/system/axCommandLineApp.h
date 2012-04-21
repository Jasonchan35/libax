//
//  axCommandLineApp.h
//  ax
//
//  Created by Jason on 24/06/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef __axCommandLineApp_h__
#define __axCommandLineApp_h__

#include "axApp.h"

class axCommandLineApp : public axApp {
public:
	axCommandLineApp();
	virtual ~axCommandLineApp();
	
			axStatus	_run( const char* name, int argc, const char* argv[] );
	virtual	axStatus	onRun() = 0;
	
	static	axCommandLineApp*	getInstance();
	
	int			getArgCount();
	const char* getArg( int index );
	
	const char* getOption( const char * opt ); //!< if opt == NULL then it return arg that not leading with '-<opt>'
	
	void	printArgs(); //!< for debug
	
//-------- OS code ------------	
	axStatus	_os_run();

#if axOS_iOS
	class	Thread : public axThread {
		virtual	void		onThreadProc();
	};
	Thread	thread_;
#endif

private:
	int argc_;
	const char** argv_;
};

//-----------------------
inline
axCommandLineApp*	axCommandLineApp::getInstance() {
	return (axCommandLineApp*) axApp::getInstance();
}


#define		axCommandLineApp_program_main( AppName, App ) \
int main( int argc, const char* argv[] ) { \
	App	app; \
	axStatus st = app._run( AppName, argc, argv ); \
	return st.code(); \
} \



#endif //__axCommandLineApp_h__
