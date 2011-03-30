#pragma once
#ifndef __ax_os_win_h__
#define __ax_os_win_h__

//! \addtogroup platform
//@{
#ifdef axOS_WIN

#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX // no min and max macro in windows.h

#include <winsock2.h> //winsock2 must include before windows.h to avoid winsock1 define
#include <windows.h>

#undef	min
#undef	max

#ifdef	_DEBUG
	#define axRELEASE_ASSERT(_Expression) \
		{ if(!(_Expression) ) { _wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__); } }
	#define axASSERT(_Expression)	axRELEASE_ASSERT(_Expression)
#else
	#define	axASSERT		(_Expression) //nothing
	#define	axRELEASE_ASSERT(_Expression)  \
		{ if(!(_Expression) ) { MessageBoxA(NULL, #_Expression, NULL, MB_ICONSTOP ); abort(); } }
#endif

#define axEXE_SUFFIX ".exe"
#define	axSO_SUFFIX  ".dll"

#endif//axOS_WIN

//@}
#endif //__ax_os_win_h__
