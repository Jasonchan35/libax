#pragma once
#ifndef __ax_os_win_h__
#define __ax_os_win_h__

//! \ingroup base_platform
//@{
#ifdef axOS_WIN

#ifndef UNICODE 
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX // no min and max macro in windows.h

#include <winsock2.h> //winsock2 must include before windows.h to avoid winsock1 define
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <shellapi.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>


/*
#include "../../external/glew/glew.h"

#include <GL/gl.h>
#include <GL/glu.h>
#define	axUSE_OpenGL        1
#define	axUSE_OpenGL_ShadingProgram		1
*/



#ifdef axCOMPILER_VC
	#pragma comment( lib, "ws2_32.lib" )
#endif //axCOMPILER_VC

#undef	min
#undef	max

#ifdef	_DEBUG
	#define axRELEASE_ASSERT(_Expression) \
		{ if(!(_Expression) ) { _wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__); } }
	#define axASSERT(_Expression)	axRELEASE_ASSERT(_Expression)
#else
	#define	axASSERT	 //nothing
	#define	axRELEASE_ASSERT(_Expression)  \
		{ if(!(_Expression) ) { MessageBoxA(NULL, #_Expression, NULL, MB_ICONSTOP ); abort(); } }
#endif

#define axEXE_SUFFIX ".exe"
#define	axSO_SUFFIX  ".dll"

#endif//axOS_WIN

//@}
#endif //__ax_os_win_h__
