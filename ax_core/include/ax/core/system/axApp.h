/*  Filename = axApp.h /  Project= awegame
 *  Created by Jason on 19/06/2011.
 *  Copyright 2011 Awenix. All rights reserved. */

#ifndef __axApp_h__
#define __axApp_h__

#include "../../ax_base.h"
#include "../math/all_math.h"
#include "axLog.h"

class axApp : public axNonCopyable {
#ifdef _OBJ_C_
	axScope_NSAutoreleasePool autoReleasePool__;
#endif //_OBJ_C_
public:		
	
			axStatus	setAppName			( const char*	sz );
	const	axStringA&	appName				();
	
	static	axStatus	setCurrentDir		( const char*	 path );
	static	axStatus	setCurrentDir		( const wchar_t* path );
	
	static	axStatus	getCurrentDir		( axIStringA	&out );
	static	axStatus	getCurrentDir		( axIStringW	&out );
	
	static	axStatus	getProcessFilename	( axIStringA	&path_to_exe );
	static	axStatus	getProcessFilename	( axIStringW	&path_to_exe );

	static	axStatus	getProcessFileDir	( axIStringA	&path_to_exe );
	static	axStatus	getProcessFileDir	( axIStringW	&path_to_exe );

	static	axStatus	getUserName			( axIStringA	&out );
	static	axStatus	getUserName			( axIStringW	&out );

	static	axStatus	getTempDir			( axIStringA	&out );
	static	axStatus	getTempDir			( axIStringW	&out );
	
	static	axStatus	getAppResourceDir	( axIStringA	&out );
	static	axStatus	getAppResourceDir	( axIStringW	&out );
	
	/*!
	!	WinXP		C:\Documents and Settings\username\Application Data
	!	WinVista	C:\User\username\Application Data
	*/
	static	axStatus	getUserAppDataDir		( axIStringA	&out );
	static	axStatus	getUserAppDataDir		( axIStringW	&out );
	
	static	axStatus	getUserAppDataFilename	( axIStringA	&out, const char*	 filename );	
	static	axStatus	getUserAppDataFilename	( axIStringW	&out, const wchar_t* filename );	
	
	/*!
	!	WinXP		C:\Documents and Settings\username\My Documents
	!	WinVista	C:\Users\username\Documents
	*/
	static	axStatus	getUserDocumentDir	( axIStringA	&out );
	static	axStatus	getUserDocumentDir	( axIStringW	&out );
	
	/*!
	!	WinXP		C:\Documents and Settings\username
	!	WinVista	C:\Users\username
	*/
	static	axStatus	getUserHomeDir		( axIStringA	&out );
	static	axStatus	getUserHomeDir		( axIStringW	&out );
	
	/*!
	!	WinXP		C:\Documents and Settings\username\Desktop
	!	WinVista	C:\Users\username\Desktop
	*/
	static	axStatus	getDesktopDir		( axIStringA	&out );
	static	axStatus	getDesktopDir		( axIStringW	&out );

	static	axStatus	loadFileFromAppData	( axIByteArray	&buf, const char*	 filename );
	static	axStatus	loadFileFromAppData	( axIByteArray	&buf, const wchar_t* filename );
	
	static	axStatus	saveFileToAppData	( const axIByteArray	&buf, const char*	 filename );
	static	axStatus	saveFileToAppData	( const axIByteArray	&buf, const wchar_t* filename );
		
	static	axApp*	getInstance();
protected:
	void	_setInstance( axApp* p );
	
private:
	static		axApp*	_instance;
	axStringA	appName_;
};


#endif //__axApp_h__



