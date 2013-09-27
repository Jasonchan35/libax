/*  Filename = axApp.h /  Project= awegame
 *  Created by Jason on 19/06/2011.
 *  Copyright 2011 Awenix. All rights reserved. */

#ifndef __axApp_h__
#define __axApp_h__

#include "../math/all_math.h"
#include "axLog.h"
#include "../other/ax_objc.h"

class axApp : public axNonCopyable {
#ifdef __OBJC__
	axScope_NSAutoreleasePool autoReleasePool__;
#endif //__OBJC__
public:		
	
	axApp();
	
			axStatus	setAppName	( const char*	sz );
	const	axStringA&	appName		();
	

	axStatus	getUserName			( axIStringA	&out );
	axStatus	getUserName			( axIStringW	&out );

	axStatus	getTempDir			( axIStringA	&out );
	axStatus	getTempDir			( axIStringW	&out );
	
	axStatus	getAppResourceDir	( axIStringA	&out );
	axStatus	getAppResourceDir	( axIStringW	&out );
	
	/*!
	!	WinXP		C:\Documents and Settings\username\Application Data
	!	WinVista	C:\User\username\Application Data
	*/
	axStatus	getUserAppDataDir		( axIStringA	&out );
	axStatus	getUserAppDataDir		( axIStringW	&out );
	
	axStatus	getUserAppDataFilename	( axIStringA	&out, const char*	 filename );	
	axStatus	getUserAppDataFilename	( axIStringW	&out, const wchar_t* filename );	
	
	/*!
	!	WinXP		C:\Documents and Settings\username\My Documents
	!	WinVista	C:\Users\username\Documents
	*/
	axStatus	getUserDocumentDir	( axIStringA	&out );
	axStatus	getUserDocumentDir	( axIStringW	&out );
	
	/*!
	!	WinXP		C:\Documents and Settings\username
	!	WinVista	C:\Users\username
	*/
	axStatus	getUserHomeDir		( axIStringA	&out );
	axStatus	getUserHomeDir		( axIStringW	&out );
	
	/*!
	!	WinXP		C:\Documents and Settings\username\Desktop
	!	WinVista	C:\Users\username\Desktop
	*/
	axStatus	getDesktopDir		( axIStringA	&out );
	axStatus	getDesktopDir		( axIStringW	&out );

	axStatus	loadFileFromAppData	( axIByteArray	&buf, const char*	 filename );
	axStatus	loadFileFromAppData	( axIByteArray	&buf, const wchar_t* filename );
	
	axStatus	saveFileToAppData	( const axIByteArray	&buf, const char*	 filename );
	axStatus	saveFileToAppData	( const axIByteArray	&buf, const wchar_t* filename );

	bool		isCtrlKeyDown	();
	bool		isAltKeyDown	();
	bool		isShiftKeyDown	();
		
protected:
	
private:

	axStringA	appName_;
};


#endif //__axApp_h__



