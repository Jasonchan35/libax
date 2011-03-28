#pragma once
#ifndef __ax_compiler_vc_h__
#define __ax_compiler_vc_h__

//! \addtogroup platform
//@{


#ifdef	_MSC_VER

#define	_CRT_SECURE_NO_WARNINGS	 1

#define	axCOMPILER_VC	1
#define axEXPORT		_declspec (dllexport)
#define	axFUNC_NAME		__FUNCTION__

#pragma warning( disable : 4200 ) //warning C4200: nonstandard extension used : zero-sized array in struct/union


#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "User32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "shell32.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "Ole32.lib" )
#pragma comment( lib, "Oleaut32.lib" )
#pragma comment( lib, "Comdlg32.lib" )
#pragma comment( lib, "Rpcrt4.lib" )
#pragma comment( lib, "Advapi32.lib" )

//cpu
#ifdef _M_X64
	#define axCPU_x86_64      1
#elif defined( _M_PPC )
	#define axCPU_powerpc     1
#else
	#define axCPU_x86         1
#endif

#ifdef _M_IX86_FP
	#define axCPU_sse2		1
#endif

//os
#ifdef _WIN64
	#define axOS_WIN64        1
#elif defined _WIN32
	#define axOS_WIN32        1
#elif defined _WINCE
	#define axOS_WINCE     1
#endif

namespace ax {

typedef	signed   char		int8_t;
typedef	signed   short		int16_t;
typedef	signed   int		int32_t;
typedef	signed   __int64	int64_t;

typedef	unsigned char		uint8_t;
typedef	unsigned short		uint16_t;
typedef	unsigned int		uint32_t;
typedef	unsigned __int64	uint64_t;

} //namespace ax

#endif //_MSC_VER

//@}
#endif //__ax_compiler_vc_h__
