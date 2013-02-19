#pragma once
#ifndef __ax_compiler_vc_h__
#define __ax_compiler_vc_h__

//! \ingroup base_platform
//@{
#ifdef	_MSC_VER

#ifndef _CRT_SECURE_NO_WARNINGS
	#define	_CRT_SECURE_NO_WARNINGS	 1
#endif

#define	axCOMPILER_VC	1
#define axDLL_EXPORT		_declspec (dllexport)
#define axDLL_IMPORT		_declspec (dllimport)


#define	axFUNC_NAME			__FUNCTION__
#define axPRETTY_FUNC_NAME	__FUNCSIG__

#define axDEPRECATED( f )		__declspec( deprecated ) f
#define axPACKED_ALIGN( f )		__declspec(align(1)) f
#define	axALWAYS_INLINE( f )	__forceinline f

#define axTHREAD_LOCAL	__declspec( thread )
#define axL(_String) L ## _String

//cpu
#ifdef _M_X64
	#define axCPU_x86_64      1
#elif defined( _M_PPC )
	#define axCPU_powerpc     1
#else
	#define axCPU_x86         1
#endif

#ifdef _M_IX86_FP
	#define axCPU_sse2		  1
#endif

//os

#ifdef _WIN64
	#define axOS_WIN64     1
	#define axOS_Win64     1

#elif defined _WIN32
	#define axOS_WIN32     1
	#define axOS_Win32     1

#elif defined _WINCE
	#define axOS_WinCE     1
	#define axOS_WinCE     1

#endif


typedef	signed   char		int8_t;
typedef	signed   short		int16_t;
typedef	signed   int		int32_t;
typedef	signed   __int64	int64_t;

typedef	unsigned char		uint8_t;
typedef	unsigned short		uint16_t;
typedef	unsigned int		uint32_t;
typedef	unsigned __int64	uint64_t;

#endif //_MSC_VER

//@}
#endif //__ax_compiler_vc_h__
